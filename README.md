# netpoll-study

### 一些细节
* epoll_create1(EPOLL_CLOEXEC)比之epoll_create1(0)，fork出来的子进程这些pd是关闭的，[参考](https://learnku.com/articles/13580)
* epoll_pwait除了接收fd的读写事件，还能接收signalfd的信号，[参考](https://cloud.tencent.com/developer/ask/sof/223634)

### 高性能 NIO 网络库 Netpoll-开源说
* [高性能 NIO 网络库 Netpoll-开源说](https://www.bilibili.com/video/BV17U4y1K7TC)
* If you can't measure it, you can't improve it.
* Netpoll为什么去掉了 TCP 的合并包却自己实现合并？

### Go 原生同步网络模型解析-Go夜读
* [Go 原生同步网络模型解析-Go夜读](https://www.bilibili.com/video/BV13E411B721)
* 介绍了gnet框架的设计

```
netFD    // net包的网络描述符
pollDesc // 运行时的一个poller的描述符

// 运行时网络轮询器的接口
func netpollinit()
func netpollopen(fd uintptr, pd *pollDesc) int32
func netpoll(delta int64) gList
 
// 运行时网络轮询器在linux下的底层系统调用
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_pwait(int epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask);
```

### 实例浅析epoll的LT和ET模式，ET模式为何要使用非阻塞IO
* [实例浅析epoll的LT和ET模式，ET模式为何要使用非阻塞IO](https://learnku.com/articles/51861)

```
epool的lt是只要事件没有被处理就一直触发，et则只触发一次，导致et必须每次要循环处理完所有的事件，六个测试场景如下：

1. sockfd是lt模式+非阻塞：能正常accept所有请求
2. sockfd是et模式+非阻塞：会漏处理accept一些请求，因为每次epoll_wait返回时只处理了一个请求
3. connfd是lt模式+阻塞：能正常处理所有连接的read
4. connfd是lt模式+非阻塞：能正常处理所有连接的read
5. connfd是et模式+阻塞：如果epoll_wait返回时只处理一次read，那么数据可能没有读完；如果循环处理read，当没有读事件时会阻塞线程。
6. connfd是et模式+非阻塞：如果epoll_wait返回时只处理一次read，那么数据可能没有读完；如果循环处理read，能正常处理所有连接的read
```

### Netpoll 模型的抽象和问题-曹大
* [Netpoll 模型的抽象和问题-曹大](https://www.bilibili.com/video/BV1Lt4y1h7Zu)
* pull.FD的Read和Write方法是有锁的，保证同一时间只有一个协程读或者写，[见源码](https://github.com/golang/go/blob/go1.16.10/src/internal/poll/fd_unix.go#L142)
* 社区的netpoll的努力：改变Go的1个连接1个协程，在有读事件的时候才开启协程

![img](images/read_write_block_mode.jpg)
![img](images/netpollinit.jpg)
![img](images/net_listen.jpg)
![img](images/net_accept.jpg)
![img](images/net_read.jpg)
![img](images/net_write.jpg)

