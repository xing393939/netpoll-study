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
#include <strings.h>

int epfd;

int lib_epoll_create1() {
    epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(1);
    }
    return epfd;
}

void lib_connect(int fd) {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(80);
    if (connect(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
    }
}

void lib_epoll_add(int op, int fd) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = op;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if (ret != 0) {
        perror("epoll_ctl");
    }
}

int lib_epoll_wait(struct epoll_event *events, int num) {
    return epoll_wait(epfd, events, num, 1000);
}
