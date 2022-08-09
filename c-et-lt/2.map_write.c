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
#include <time.h>
#include <signal.h>
#include <string.h>

void lib_connect(int fd, char *addr, int port);

void lib_epoll_create1();

void lib_epoll_ctl(int op, int fd, int events);

void lib_epoll_mod(int op, int fd);

int lib_epoll_wait(struct epoll_event *events, int num, int timeout);

// gcc 2.map_write.c include.c -o 2.out && strace -c ./2.out
// 1.07s
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_connect(client_fd, "127.0.0.1", 6379);

    int f = open("ping.txt", O_RDONLY);
    char *str;

    for (int i = 0; i < 10000; i++) {
        str = mmap(0, 10, PROT_READ, MAP_SHARED, f, 0);
        write(client_fd, str, strlen(str));
    }
}