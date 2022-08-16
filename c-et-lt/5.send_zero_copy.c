#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <time.h>
#include <signal.h>
#include <string.h>

void lib_connect(int fd, char *addr, int port);

void lib_epoll_create1();

void lib_epoll_ctl(int op, int fd, int events);

void lib_epoll_mod(int op, int fd);

ssize_t lib_read(int fd, void *vptr, size_t n);

ssize_t lib_write(int fd, const void *vptr, size_t n);

int lib_epoll_wait(struct epoll_event *events, int num, int timeout);

void create_file(char *name, int size) {
    FILE *f = fopen(name, "w+");
    char *str = malloc(size);
    strcpy(str, "set a ");
    for (int i = 6; i < size; i++) {
        str[i] = 97;
    }
    str[size - 3] = '\r';
    str[size - 2] = '\n';
    str[size - 1] = 0;
    fputs(str, f);
    fclose(f);
}

// gcc -g 5.send_zero_copy.c include.c -o 5.out && strace -c ./5.out 1024
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_connect(client_fd, "192.168.2.119", 6379);

    int size = atoi(argv[1]);
    create_file("temp.txt", size);
    int f = open("temp.txt", O_RDONLY);
    int one;
    setsockopt(client_fd, SOL_SOCKET, SO_ZEROCOPY, &one, sizeof(one));
    char *str = malloc(size);

    for (int i = 0; i < 10000; i++) {
        lib_read(f, str, size);
        send(client_fd, str, strlen(str), 0);
    }
    // wait for kernel sent success
    sleep(2);
    shutdown(client_fd, SHUT_WR);
    unlink("temp.txt");
}