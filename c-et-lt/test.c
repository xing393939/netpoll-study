#include <stdio.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <linux/errqueue.h>
#include <unistd.h>

#define MAX 1024*4
#define PORT 8081
#define SA struct sockaddr

int funcwaitzc(int sockfd, int zeroCopyCounter) {
    int currentProgress = 0;
    int ret;

    while (currentProgress < zeroCopyCounter - 1) {
        struct msghdr msg = {};
        char msgData[512] = {};
        msg.msg_control = msgData;
        msg.msg_controllen = sizeof(msgData) - sizeof(msg);
        ret = recvmsg(sockfd, &msg, MSG_ERRQUEUE);
        if (ret == -1 && errno == EAGAIN) {
            printf("keep going\n");
            usleep(100);
        } else if (ret == -1) {
            printf("error recvmsg notification\n");
            return -1;
        } else if (msg.msg_flags & MSG_CTRUNC) {
            printf("error recvmsg notification: truncated\n");
            return -1;
        } else {
            struct cmsghdr *cm = CMSG_FIRSTHDR(&msg);
            if (cm->cmsg_level != SOL_IP) {
                printf("error SOL_IP\n");
                return -1;
            }
            if (cm->cmsg_type != IP_RECVERR) {
                printf("error IP_RECVERR\n");
                return -1;
            }

            struct sock_extended_err *serr = (struct sock_extended_err *) CMSG_DATA(cm);
            if (serr->ee_errno != 0) {
                printf("error serr->ee_errno\n");
                return -1;
            }
            if (serr->ee_origin != SO_EE_ORIGIN_ZEROCOPY) {
                printf("error SO_EE_ORIGIN_ZEROCOPY\n");
                return -1;
            }

            printf("Waiting (%i, %i, %i, %i)\n", currentProgress, serr->ee_data, serr->ee_info, zeroCopyCounter);
            currentProgress = currentProgress > serr->ee_data ? currentProgress : serr->ee_data;
        }
    }

    return 0;
}

// Function designed for chat between client and server.
void funcwrite(int sockfd) {
    char *buff = (char *) malloc(MAX);
    int counter = 0;

    for (int i = 0; i < 10000; i++) {
        printf("Send...\n");
        if (send(sockfd, buff, MAX, MSG_DONTWAIT | MSG_ZEROCOPY) < 0) {
            printf("Send error %s\n", strerror(errno));
            return;
        }

        counter++;

        if (funcwaitzc(sockfd, counter) < 0) {
            printf("ZC counter error");
            return;
        }
    }
}

// Driver function
int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    int optValue = 1;

    setbuf(stdout, NULL);

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_ZEROCOPY, &optValue, sizeof(optValue)) != 0) {
        printf("zero copy flag failed...\n");
        exit(0);
    } else {
        printf("zero copy enabled...\n");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if ((bind(sockfd, (SA *) &servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed (%s)...\n", strerror(errno));
        exit(0);
    } else {
        printf("Socket successfully binded..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening..\n");
    }

    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (SA *) &cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    } else {
        printf("server acccept the client...\n");
    }
    funcwrite(connfd);
}
