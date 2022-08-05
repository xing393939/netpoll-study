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
#include <arpa/inet.h>

int epfd;

int lib_epoll_create1() {
    epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(1);
    }
    return epfd;
}

void lib_connect(int fd, char *addr, int port) {
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, addr, &servaddr.sin_addr);
    if (connect(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
    }
}

void lib_bind_listen(int fd, char *addr, int port) {
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, addr, &servaddr.sin_addr);
    if (bind(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
    }
    if (listen(fd, 128)) {
        perror("listen error");
    }
}

int lib_accept(int fd) {
    struct sockaddr in_addr;
    socklen_t in_len = sizeof(in_addr);
    int conn = accept(fd, &in_addr, &in_len);
    if (conn <= 0) {
        perror("accept error");
    }
    return conn;
}

void lib_epoll_ctl(int op, int fd, int events) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    int ret = epoll_ctl(epfd, op, fd, &event);
    if (ret != 0) {
        perror("epoll_ctl error");
    }
}

int lib_epoll_wait(struct epoll_event *events, int num, int timeout) {
    return epoll_wait(epfd, events, num, timeout);
}
