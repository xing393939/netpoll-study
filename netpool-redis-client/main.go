package main

import (
	"fmt"
	"netpoll-study/netpoll026"
	"time"
)

// dlv --listen=:2345 --headless=true --api-version=2 --accept-multiclient exec ./demo.exe
func main() {
	dialer := netpoll.NewDialer()
	connection, _ := dialer.DialConnection("tcp", "localhost:6379", time.Second)
	msg := make([]byte, 512)
	_, err2 := connection.Write([]byte("PING\r\n"))
	n, err3 := connection.Read(msg)
	fmt.Println(err2, err3, string(msg[0:n]))
}
