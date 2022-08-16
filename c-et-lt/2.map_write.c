#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
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

ssize_t lib_read(int fd, void *vptr, size_t n);

ssize_t lib_write(int fd, const void *vptr, size_t n);

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

void wait_kernel_success(int client_fd) {
    int pending = 1;
    while (pending > 0) {
        ioctl(client_fd, TIOCOUTQ, &pending);
        struct tcp_info info;
        socklen_t info_len = sizeof(info);
        getsockopt(client_fd, IPPROTO_TCP, TCP_INFO, (void *)&info, &info_len);
        pending += info.tcpi_unacked;
    }
}

// gcc -g 2.map_write.c include.c -o 2.out && strace -c ./2.out 1024
// 1.07s
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    int rsize;
    socklen_t len = sizeof(rsize);
    getsockopt(client_fd, SOL_SOCKET, SO_RCVBUF, &rsize, &len);
    printf("SO_RCVBUF:%d\n", rsize);
    getsockopt(client_fd, SOL_SOCKET, SO_SNDBUF, &rsize, &len);
    printf("SO_SNDBUF:%d\n", rsize);

    lib_connect(client_fd, "192.168.2.119", 6379);

    int size = atoi(argv[1]);
    create_file("temp.txt", size);
    int f = open("temp.txt", O_RDONLY);

    for (int i = 0; i < 10000; i++) {
        char *str = (void *) -1;
        while (str == (void *) -1) {
            str = mmap(0, size, PROT_READ, MAP_SHARED, f, 0);
        }
        lib_write(client_fd, str, strlen(str));
    }
    // wait_kernel_success(client_fd);
    // wait for kernel sent success
    sleep(2);
    shutdown(client_fd, SHUT_WR);
    unlink("temp.txt");
}