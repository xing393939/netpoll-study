#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>

void lib_connect(int fd, char *addr, int port);

void lib_bind_listen(int fd, char *addr, int port);

void lib_epoll_create1();

void lib_epoll_ctl(int op, int fd, int events);

int lib_epoll_wait(struct epoll_event *events, int num, int timeout);

int lib_accept(int fd);

int main(int argc, char *argv[]) {
    char *str = "+PONG\r\n";
    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_bind_listen(listen_fd, "localhost", 6381);

    lib_epoll_create1();
    lib_epoll_ctl(EPOLL_CTL_ADD, listen_fd, EPOLLIN);

    struct epoll_event *events = calloc(64, sizeof(struct epoll_event));
    while (1) {
        int num = lib_epoll_wait(events, 64, 1000);
        char *buf = calloc(512, sizeof(char));
        for (int i = 0; i < num; i ++) {
            if (events[i].data.fd == listen_fd) {
                int conn = lib_accept(listen_fd);
                lib_epoll_ctl(EPOLL_CTL_ADD, conn, EPOLLIN);
            } else {
                int len = read(events[i].data.fd, buf, 512);
                if (len == 0 || write(events[i].data.fd, str, strlen(str)) < 0) {
                    lib_epoll_ctl(EPOLL_CTL_DEL, events[i].data.fd, 0);
                    close(events[i].data.fd);
                }
            }
        }
    }
}

/*
gcc redis_server.c include.c
 */