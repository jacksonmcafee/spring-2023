#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MEM_SIZE 4096
#define MEM_NAME "/jmm20g_cop4610"
#define READING_END 0
#define WRITING_END 1

void handle_child(FILE *dst_file, int parent_to_child[], int child_to_parent[]) {
    // enter the child process
    close(parent_to_child[WRITING_END]);
    close(child_to_parent[READING_END]);

    // create shared memory object, give read/write perms if needed
    int shared_mem_fd = shm_open(MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_mem_fd < 0) {
        perror("Error creating shared memory creation object.");
        exit(1);
    }

    // map shared memory object
    void *shared_mem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Error mapping shared memory.");
        exit(1);
    }

    /* In office hours, I saw your example where the loop # was calculated before the actual
     * process began. When I initially wrote my version, I used that loop/remainder 
     * setup but I ended up with a huge mess (only somewhat related). When I rewrote it, 
     * I ended up going with the following setup. 
     *
     * block_len is updated by fread() in a while loop in the parent process. Within the while
     * loop, block_num and block_len are used by the child process where data is written to destination.
     * The block number is acknowledged by the child process and block_num is incremented. 
     * Cycle repeats.
     */

    
    int current_block_num, current_block_len;
    while (1) {
    	// get the block number and length from the parent process
        read(parent_to_child[READING_END], &current_block_num, sizeof(int));
        read(parent_to_child[READING_END], &current_block_len, sizeof(int));

	// if block_num == 0, end loop
        if (current_block_num == 0) {
            write(child_to_parent[WRITING_END], &current_block_num, sizeof(int));
            break;
        }

	// write data from shared memory to destination file
        fwrite(shared_mem, 1, current_block_len, dst_file);

	// send acknowledgement back to parent process
        write(child_to_parent[WRITING_END], &current_block_num, sizeof(int));
    }

    // clean up allocated resources, starting with shared memory
    munmap(shared_mem, MEM_SIZE);
    shm_unlink(MEM_NAME);

    // close pipes and destination file
    close(parent_to_child[READING_END]);
    close(child_to_parent[WRITING_END]);
    fclose(dst_file);
}

void handle_parent(FILE *src_file, int parent_to_child[], int child_to_parent[]) {
    close(parent_to_child[READING_END]);
    close(child_to_parent[WRITING_END]);

    // create shared memory object, give read/write perms if needed
    int shared_mem_fd = shm_open(MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_mem_fd < 0) {
        perror("Error creating shared memory object.");
        exit(1);
    }

    // use ftruncate() to ensure that 4KB blocks are allocated
    ftruncate(shared_mem_fd, MEM_SIZE);

    // map shared memory object
    void *shared_mem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Error mapping shared memory.");
        exit(1);
    }

    int current_block_num = 1;
    int current_block_len;
    
    // read data from source in 4KB chunks
    while ((current_block_len = fread(shared_mem, 1, MEM_SIZE, src_file)) > 0) {
     	// send current_block_num and current_block_len to child
	write(parent_to_child[WRITING_END], &current_block_num, sizeof(int));
        write(parent_to_child[WRITING_END], &current_block_len, sizeof(int));

        int received_block_num;
	// receieve acknowledgement from child process
        read(child_to_parent[READING_END], &received_block_num, sizeof(int));

	// check for block_num inconsistencies
        if (received_block_num != current_block_num) {
            fprintf(stderr, "Error: block number mismatch\n");
            break;
        }

	// increment current_block_num
        current_block_num++;
    }

    // when there are no more chunks to send, send zeroes to child to mark end
    int zero = 0;
    write(parent_to_child[WRITING_END], &zero, sizeof(int));
    write(parent_to_child[WRITING_END], &zero, sizeof(int));

    // receieve acknowledgement from child process
    int received_block_num;
    read(child_to_parent[READING_END], &received_block_num, sizeof(int));

    // check for block_num inconsistencies
    if (received_block_num != 0) {
        fprintf(stderr, "Error: block number mismatch\n");
    }

    // clean up resources, starting with shared memory
    munmap(shared_mem, MEM_SIZE);
    shm_unlink(MEM_NAME);

    // close pipes and source file
    close(parent_to_child[WRITING_END]);
    close(child_to_parent[READING_END]);
    fclose(src_file);
}

int main(int argc, char *argv[]) {
    // check for the right number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: trans source_file destination_file\n");
        return 1;
    }

    // open the source file
    FILE *src_file = fopen(argv[1], "rb");
    if (!src_file) {
	// if the source file fails to open, exit gracefully
        perror("Failed to open input file");
        return 1;
    }

    // check for the destination file
    FILE *dst_file = fopen(argv[2], "rb");
    if (dst_file) {
	// if the destination file already exists.
        fclose(dst_file);
        char confirm;
        printf("Output file already exists. Do you want to overwrite it? (y/n): ");
        scanf(" %c", &confirm);
        if (confirm != 'y') {
            printf("Operation aborted.\n");
            fclose(src_file);
            return 1;
        }
    }

    // open the destination file
    dst_file = fopen(argv[2], "wb");
    if (!dst_file) {
        perror("Failed to open output file");
        fclose(src_file);
        return 1;
    }

    // create pipes
    int parent_to_child[2];
    int child_to_parent[2];
    
    // if either pipe creation instance fails, clean up and exit gracefully
    if (pipe(parent_to_child) < 0) {
        perror("Error creating parent_to_child.");
        fclose(src_file);
        fclose(dst_file);
        return 1;
    }
  
    if (pipe(child_to_parent) < 0) {
        perror("Error creating child_to_parent.");
        close(parent_to_child[READING_END]);
        close(parent_to_child[WRITING_END]);
        fclose(src_file);
        fclose(dst_file);
        return 1;
    }    

    pid_t pid = fork();

    /*
     * This is where I kinda break off from the examples provided by Dr. Uh in our lectures but
     * shouldn't be too far off. Thank GOD for the shm_open and pipe man pages. 
     *
     * The setup for the shm_open and mmap are the same, but the actual writing is 
     * different. I tried to explain in a few places so I hope it helps. 
     */

    if (pid < 0) {
        perror("Forking error");
    } else if (pid == 0) {
        handle_child(dst_file, parent_to_child, child_to_parent);
        exit(0);
    } else {
        handle_parent(src_file, parent_to_child, child_to_parent);
    }

    return 0;
}
