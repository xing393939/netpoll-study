package main

import (
	"golang.org/x/sync/errgroup"
	"net"
	"time"
)

func main() {
	eg := errgroup.Group{}
	for i := 0; i < 50000; i++ {
		eg.Go(func() error {
			_, err := net.Dial("tcp", "172.18.152.142:6382")
			if err != nil {
				println(err.Error())
			}
			time.Sleep(time.Minute)
			return err
		})
	}
	_ = eg.Wait()
}
