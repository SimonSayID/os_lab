//
// Created by simon on 17-12-10.
//

/*
 * linear mmap
 *
 * Linux can only map complete 4 kB pages (4 MB if your kernel uses large pages)
 * - this is a feature of the hardware, and it is impossible to circumvent in software.
 * In other words, even though you only requested one byte,
 * Linux has been forced to round this up to the nearest page size;
 * there is no way to get around this.
 *
 * url:
 * "http://www.linuxforums.org/forum/programming-scripting/
 * mmap-question-length-parameter-mmap-not-serving-its-purpose-print-192206.html"
 * */
#include "unix_mmap.h"

void unix_mmap() {
    int fd[2];
    pid_t p1;
    pipe(fd);
    p1 = fork();
    if (p1 == -1) {
        perror("fork");
    } else if (p1 == 0) {
        int file;
        ssize_t n;
        char *name;
        void *address;

        close(fd[1]);
        name = (char *)malloc(18);
        n = read(fd[0], name, 18);
        if (n == -1) {
            perror("child:read");
            exit(EXIT_FAILURE);
        }
        file = open(name, O_RDONLY | O_APPEND);
        if (file == -1) {
            perror("child:open");
            exit(EXIT_FAILURE);
        }
        address = mmap(NULL, 1, PROT_READ, MAP_SHARED, file, 0);
        if (address == MAP_FAILED) {
            perror("child:mmap");
            exit(EXIT_FAILURE);
        }
        printf("child:share memory content:%s\n", (char *)address);
        free(name);
    } else {
        int file;
        void *address;
        ssize_t n;
        close(fd[0]);
        file = open("/tmp/demo_file.txt", O_CREAT | O_RDWR | O_TRUNC, OBJ_PERM);
        if (file == -1) {
            perror("parent:open");
            exit(EXIT_FAILURE);
        }
        n = write(file, "Hello", 5);
        if (n == -1) {
            perror("parent:write");
            exit(EXIT_FAILURE);
        }
        address = mmap(NULL, 5, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
        if (address == MAP_FAILED) {
            perror("parent:mmap");
            exit(EXIT_FAILURE);
        }
        printf("parent:share memory origin content%s\n", (char *)address);
        strncpy(address, "World", 5);
        n = msync(address, 5, MS_SYNC);
        if (n == -1) {
            perror("parent:msync");
            exit(EXIT_FAILURE);
        }
        printf("parent:share memory new content:%s\n", (char *)address);
        write(fd[1], "/tmp/demo_file.txt", 18);
        wait(&p1);
        n = remove("/tmp/demo_file.txt");
        if (n == -1) {
            perror("parent:remove");
            exit(EXIT_FAILURE);
        }
    }
}