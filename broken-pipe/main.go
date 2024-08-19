package main

import (
	"fmt"
	"net"
	"syscall"
)

func http10(conn net.Conn, url string) {
	str := fmt.Sprintf("GET %s HTTP/1.1\nHost: opapi2.bookan.com.cn\nUser-Agent: curl/7.58.0\nAccept: */*\n\n", url)
	request := []byte(str)
	_, err := conn.Write(request)
	if err != nil {
		println(err.Error())
	}
}

func main() {
	_, err := net.Dial("tcp", "127.0.0.1:6380")
	if err != nil {
		println(err.Error())
	}
	syscall.Close(3)
	select {}
}
