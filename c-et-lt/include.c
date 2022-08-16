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
#include <errno.h>
#include <string.h>

int epfd;

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

ssize_t lib_write(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            // 被信号中断，重写
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                perror("write error");
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft);
}

ssize_t lib_read(int fd, void *vptr, size_t n) {
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            // 被信号中断，重读
            if (errno == EINTR)
                nread = 0;
            else
                perror("read error");
        } else if (nread == 0)
            break;

        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}