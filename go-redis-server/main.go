package main

import (
	"net"
)

func main() {
	listener, _ := net.Listen("tcp", "localhost:6380")
	for {
		conn, _ := listener.Accept()
		go func() {
			buf := make([]byte, 512, 512)
			res := []byte("+PONG\r\n")
			for {
				n, err := conn.Read(buf)
				if err != nil {
					_ = conn.Close()
					break
				}
				if n > 0 {
					_, _ = conn.Write(res)
				}
			}
		}()
	}
}

/**
redis-benchmark -p 6379 -c 100 -n 90000 ping
redis-benchmark -p 6380 -c 100 -n 90000 ping
*/
