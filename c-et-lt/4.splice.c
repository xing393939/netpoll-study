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

int lib_epoll_wait(struct epoll_event *events, int num, int timeout);

// gcc 4.splice.c include.c -o 4.out && strace -c ./4.out
// echo info commandstats|redis-cli|grep ping
// 1.30s
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_connect(client_fd, "127.0.0.1", 6379);

    int f = open("ping.txt", O_RDONLY);
    int len = 10;

    int pfd[2];
    pipe(pfd);

    for (int i = 0; i < 10000; i++) {
        loff_t tmp = 0;
        splice(f, &tmp, pfd[1], NULL, len, SPLICE_F_MOVE);
        splice(pfd[0], NULL, client_fd, NULL, len, SPLICE_F_MOVE | SPLICE_F_MORE);
    }
}