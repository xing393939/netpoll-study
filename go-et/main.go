package main

import "net"

func main() {
	conn, _ := net.Dial("tcp", "localhost:80")
	n, _ := conn.Write([]byte("GET / HTTP/1.1\n\n"))
	println(n)
}

/**
strace ./main 2>&1 |grep epoll

最后两行是：
epoll_ctl(4, EPOLL_CTL_ADD, 3, {EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET, {u32=2648735592, u64=140585518268264}}) = 0
epoll_pwait(4, [{EPOLLOUT, {u32=2648735592, u64=140585518268264}}], 128, 0, NULL, 0) = 1
 */
