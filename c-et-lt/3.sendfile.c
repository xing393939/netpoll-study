#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
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

// gcc 3.sendfile.c include.c -o 3.out && strace -c ./3.out
// 0.66s
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_connect(client_fd, "127.0.0.1", 6379);

    int size = atoi(argv[1]);
    create_file("temp.txt", size);
    int f = open("temp.txt", O_RDONLY);

    for (int i = 0; i < 10000; i++) {
        off_t tmp = 0;
        sendfile(client_fd, f, &tmp, size);
    }
    // wait for kernel sent success
    sleep(2);
    shutdown(client_fd, SHUT_WR);
    unlink("temp.txt");
}