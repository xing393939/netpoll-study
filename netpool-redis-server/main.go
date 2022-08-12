package main

import (
	"context"
	"netpoll-study/netpoll026"
)

func main() {
	listener, _ := netpoll.CreateListener("tcp", "localhost:6382")
	eventLoop, _ := netpoll.NewEventLoop(
		func(ctx context.Context, connection netpoll.Connection) error {
			reader, writer := connection.Reader(), connection.Writer()
			msg, _ := reader.ReadString(reader.Len())
			if msg != "" {
				_ = reader.Release()
				_, _ = writer.WriteString("+PONG\r\n")
				_ = writer.Flush()
			}
			return nil
		},
	)
	_ = eventLoop.Serve(listener)
}
