package main

import (
	"context"
	"netpoll-study/netpoll026"
)

// dlv --listen=:2345 --headless=true --api-version=2 --accept-multiclient exec ./demo.exe
func main() {
	listener, _ := netpoll.CreateListener("tcp", ":6382")
	eventLoop, _ := netpoll.NewEventLoop(
		func(ctx context.Context, connection netpoll.Connection) error {
			reader, writer := connection.Reader(), connection.Writer()
			msg, _ := reader.Next(reader.Len())
			if msg != nil {
				_ = reader.Release()
				_, _ = writer.WriteString("+PONG\r\n")
				_ = writer.Flush()
			}
			return nil
		},
	)
	_ = eventLoop.Serve(listener)
}
