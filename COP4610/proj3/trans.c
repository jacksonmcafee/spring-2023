#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shm"

int main(int argc, char *argv[])
{
    int src_fd, dst_fd;
    struct stat src_stat;
    off_t src_size;
    void *shm_ptr;
    
    // check if passed arguments are valid
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // open source file
    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror("open source file");
        exit(EXIT_FAILURE);
    }
    
    // get source file size
    if (fstat(src_fd, &src_stat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    src_size = src_stat.st_size;
    
    // open destination file
    dst_fd = open(argv[2], O_CREAT | O_WRONLY, 0644);
    if (dst_fd == -1) {
        perror("open destination file");
        exit(EXIT_FAILURE);
    }
    
    // create shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // set shared memory object size
    if (ftruncate(shm_fd, src_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    // map shared memory object into process address space
    shm_ptr = mmap(NULL, src_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    // read source file into shared memory
    if (read(src_fd, shm_ptr, src_size) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    
    // write shared memory into destination file
    if (write(dst_fd, shm_ptr, src_size) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    
    // unmap shared memory object from process address space
    if (munmap(shm_ptr, src_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    
    // close file descriptors and shared memory object
    close(src_fd);
    close(dst_fd);
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
