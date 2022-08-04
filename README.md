# netpoll-study

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

### Netpoll 模型的抽象和问题-曹大
* [Netpoll 模型的抽象和问题-曹大](https://www.bilibili.com/video/BV1Lt4y1h7Zu)