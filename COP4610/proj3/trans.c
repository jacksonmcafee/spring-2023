#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 4096

const char * shm_buff_name = "/my_shm";

struct{
    int b_sent;
    int b_size;
    int b_return;
}HEADER;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: trans <input-file> <output-file>\n\n");
        return 1;
    }

    // get input and output file
    char *in_file = argv[1];
    char *out_file = argv[2];
    FILE *in_ptr;
    FILE *out_ptr;

    // open input file, if it doesn't exist, create it with "r" flag
    in_ptr = fopen(in_file, "r");

    // this block doesn't make any sense, why would I be explaining that the output
    // function doesn't exist when I'm opening the input file? How would I know that?
    if (in_ptr == NULL) {
        fprintf(stdout, "\n Output file already exists. Would you like to overwrite it(y|n)?");
        char choice = fgetc(stdin);
        if (choice == 'n') return 1;
    }

    // open output file, if it doesn't exist, create it with "w" flag
    out_ptr = fopen(out_file, "w");
    if (out_ptr == NULL) {
        fprintf(stderr, "\nOutput file failed to open. Exiting. \n\n");
        return 1;
    }


    // get file size of input file
    struct stat st;
    fstat(in_ptr, &st);
    int in_size = st.st_size;

    // determine how many times you need to loop through the file
    int rem = 0;
    int loops = in_size / BUFFER_SIZE;
    rem = in_size % BUFFER_SIZE;
    if (rem != 0) { loops++; }

    int pid;
    int toChild[2]; // pipe to Child
    int toParent[2]; // pipe to Parent
    void *shm_ptr;
    void *shm_base;
    int shm_fd;

    // create pipe to Child process
    if (pipe(toChild) == -1) {
        fprintf(stderr, "\ntoChild pipe creation failed. Exiting. \n\n");
        return 1;
    }

    // create pipe to Parent process
    if (pipe(toParent) == -1) {
        fprintf(stderr, "\ntoParent pipe creation failed. Exiting. \n\n");
        return 1;
    }

    // initialize HEADER
    HEADER.b_sent = 0;
    HEADER.b_size = 0;
    HEADER.b_return = -1;
	
	// local variables
    int blocks_read = 0;
    int expected_size = 0;

    // spilt into two processes
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "\nfork() failed. Exiting.\n\n");
        return 1;
    }

    // Parent Process
    if (pid > 0) {
        close(toChild[READ_END]);
        close(toParent[WRITE_END]);

        shm_fd = shm_open(shm_buff_name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            fprintf(stderr, "\nParent shared mem creation failed. Exiting. \n\n");
            return 1;
        }
        ftruncate(shm_fd, BUFFER_SIZE);
        shm_ptr = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == MAP_FAILED) {
            fprintf(stderr, "\nParent mapping creation failed. Exiting. \n\n");
            return 1;
        }
        shm_base = shm_ptr;

        while (loops > 0) {
            HEADER.b_sent += 1;
            HEADER.b_size = BUFFER_SIZE;
            shm_ptr = shm_base; //Buffer used for failsafe purposes
            if (loops > 1 || (loops == 1 && remainder == 0)) {
                expected_size = BUFFER_SIZE;
                blocks_read = fread(shm_ptr, 1, BUFFER_SIZE, in_ptr);
            } else {
                expected_size = remainder;
                HEADER.b_size = remainder;
                blocks_read = fread(shm_ptr, 1, remainder, in_ptr);
            }
            if (expected_size != blocks_read) {
                fprintf(stderr, "\nThe expected number of read blocks doesn't match what was actually read\n\n");
                fprintf(stderr, "Expected: %d\nActual: %d\n\n", expected_size, blocks_read);
                exit(EXIT_FAILURE);
            }
            write(toChild[WRITE_END], &HEADER, sizeof(HEADER)); //notify
            read(toParent[READ_END], &HEADER, sizeof(HEADER)); //wait

            if (HEADER.b_sent != HEADER.b_return) {
                fprintf(stderr, "\nBlock retun does not match block number that was sent\n\n");
                fprintf(stderr, "Block number sent: %d\nBlock number returned: %d\n\n", HEADER.b_sent, HEADER.b_return);
                exit(EXIT_FAILURE);
            }
        }
    } else { // End Parent Process

        // Beginning Child Process

        close(toChild[WRITE_END]);
        close(toParent[READ_END]);

        shm_fd = shm_open(shm_buff_name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            fprintf(stderr, "\nChild shared mem creation failed. Exiting. \n\n");
            return 1;
        }
        ftruncate(shm_fd, BUFFER_SIZE);
        shm_ptr = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == MAP_FAILED) {
            fprintf(stderr, "\nChild mapping creation failed. Exiting. \n\n");
            return 1;
        }
        shm_base = shm_ptr; 

        while (loops > 0) {
         
            shm_ptr = shm_base; //Buffer used for failsafe purposes
            if (loops > 1 || (loops == 1 && remainder == 0)) {
                expected_size = BUFFER_SIZE;
                blocks_read = fread(shm_ptr, 1, BUFFER_SIZE, in_ptr);
            } else {
                expected_size = remainder;
                HEADER.b_size = remainder;
                blocks_read = fread(shm_ptr, 1, remainder, in_ptr);
            }
            if (expected_size != blocks_read) {
                fprintf(stderr, "\nThe expected number of read blocks doesn't match what was actually read\n\n");
                fprintf(stderr, "Expected: %d\nActual: %d\n\n", expected_size, blocks_read);
                exit(EXIT_FAILURE);
            }
            write(toChild[READ_END], &HEADER, sizeof(HEADER)); //notify
            read(toParent[WRITE_END], &HEADER, sizeof(HEADER)); //wait

            if (HEADER.b_sent != HEADER.b_return) {
                fprintf(stderr, "\nBlock retun does not match block number that was sent\n\n");
                fprintf(stderr, "Block number sent: %d\nBlock number returned: %d\n\n", HEADER.b_sent, HEADER.b_return);
                exit(EXIT_FAILURE);
            }
        }
    }
}
//End Child Process