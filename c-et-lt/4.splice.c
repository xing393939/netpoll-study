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

void create_file(char *name, int size);

void lib_connect(int fd, char *addr, int port);

void lib_epoll_create1();

void lib_epoll_ctl(int op, int fd, int events);

void lib_epoll_mod(int op, int fd);

int lib_epoll_wait(struct epoll_event *events, int num, int timeout);

ssize_t lib_read(int fd, void *vptr, size_t n);

ssize_t lib_write(int fd, const void *vptr, size_t n);

// gcc -g 4.splice.c include.c -o 4.out && strace -c ./4.out 1024
// echo info commandstats|redis-cli|grep ping
// 添加流量规则：iptables -A INPUT -p tcp --dport 6379
// 查看监控数据：iptables -L INPUT -v -n -x
// 重置监控数据：iptables -Z INPUT
// 删除监控规则：iptables -D INPUT -p tcp --dport 6379
// 数据包头部 = EthernetHeader(14B) + IPHeader(20B) + TCPHeader(20B)，但iptables的统计不包含EthernetHeader
int main(int argc, char *argv[]) {
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    lib_connect(client_fd, "192.168.2.119", 6379);

    int size = atoi(argv[1]);
    create_file("temp.txt", size);
    int f = open("temp.txt", O_RDONLY);

    int pfd[2];
    pipe(pfd);

    for (int i = 0; i < 10000; i++) {
        loff_t tmp = 0;
        splice(f, &tmp, pfd[1], NULL, size, SPLICE_F_MOVE);
        splice(pfd[0], NULL, client_fd, NULL, size, SPLICE_F_MOVE | SPLICE_F_MORE);
    }
    // wait for kernel sent success
    sleep(2);
    shutdown(client_fd, SHUT_WR);
    unlink("temp.txt");
}