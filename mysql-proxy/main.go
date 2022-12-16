package main

import (
	"context"
	"io"
	"netpoll-study/netpoll026"
	"time"
)

// dlv --listen=:2345 --headless=true --api-version=2 --accept-multiclient exec ./demo.exe
func main() {
	listener, _ := netpoll.CreateListener("tcp", ":3307")
	eventLoop, _ := netpoll.NewEventLoop(
		func(ctx context.Context, upstream netpoll.Connection) error {
			downstream := ctx.Value(upstream).(netpoll.Connection)
			_, err := io.Copy(downstream, upstream)
			return err
		},
		netpoll.WithOnConnect(func(ctx context.Context, upstream netpoll.Connection) context.Context {
			// 创建downstream
			dialer := netpoll.NewDialer()
			connection, err := dialer.DialConnection("tcp", "rm-m5ey1j08ddn877eg46o.mysql.rds.aliyuncs.com:3306", time.Second)
			if err != nil {
				panic("mysql error " + err.Error())
			}
			_ = connection.SetOnRequest(func(ctx context.Context, downstream netpoll.Connection) error {
				_, err := io.Copy(upstream, downstream)
				return err
			})
			return context.WithValue(ctx, upstream, connection)
		}),
	)
	_ = eventLoop.Serve(listener)
}
