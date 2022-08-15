package main

import (
	"syscall"
	"testing"
	"unsafe"
)

func BenchmarkWrite(b *testing.B) {
	b.StopTimer()
	r, w := GetSysFdPairs()

	// benchmark
	buffer := make([]byte, 128)
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		syscall.RawSyscall(syscall.SYS_WRITE, uintptr(w), uintptr(unsafe.Pointer(&buffer[0])), uintptr(len(buffer)))
		syscall.RawSyscall(syscall.SYS_READ, uintptr(r), uintptr(unsafe.Pointer(&buffer[0])), uintptr(len(buffer)))
	}
}

func BenchmarkWritev(b *testing.B) {
	b.StopTimer()
	r, w := GetSysFdPairs()
	buffer := make([]byte, 128)

	// benchmark
	var br = barrier{}
	br.bs = make([][]byte, 2)
	br.bs[0] = make([]byte, 128)
	br.bs[1] = make([]byte, 0, 128)
	br.ivs = make([]syscall.Iovec, len(br.bs))
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		writev(w, br.bs, br.ivs)
		syscall.RawSyscall(syscall.SYS_READ, uintptr(r), uintptr(unsafe.Pointer(&buffer[0])), uintptr(len(buffer)))
	}
}

func BenchmarkSendmsg(b *testing.B) {
	b.StopTimer()
	r, w := GetSysFdPairs()
	buffer := make([]byte, 128)

	syscall.SetsockoptInt(w, syscall.SOL_SOCKET, MSG_ZEROCOPY, 1)

	// benchmark
	var br = barrier{}
	br.bs = make([][]byte, 2)
	br.bs[0] = make([]byte, 128)
	br.bs[1] = make([]byte, 0, 128)
	br.ivs = make([]syscall.Iovec, len(br.bs))
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		sendmsg(w, br.bs, br.ivs, true)
		syscall.RawSyscall(syscall.SYS_READ, uintptr(r), uintptr(unsafe.Pointer(&buffer[0])), uintptr(len(buffer)))
	}
}

func BenchmarkRead(b *testing.B) {
	b.StopTimer()
	r, w := GetSysFdPairs()
	go func() {
		wmsg := make([]byte, 128*1024)
		for {
			syscall.Write(w, wmsg)
		}
	}()

	// benchmark
	buffer := make([]byte, 128)
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		syscall.RawSyscall(syscall.SYS_READ, uintptr(r), uintptr(unsafe.Pointer(&buffer[0])), uintptr(len(buffer)))
	}
}

func BenchmarkReadv(b *testing.B) {
	b.StopTimer()
	r, w := GetSysFdPairs()
	go func() {
		wmsg := make([]byte, 128*1024)
		for {
			syscall.Write(w, wmsg)
		}
	}()

	// benchmark
	var br = barrier{}
	br.bs = make([][]byte, 2)
	br.bs[0] = make([]byte, 128)
	br.bs[1] = make([]byte, 0, 128)
	br.ivs = make([]syscall.Iovec, len(br.bs))
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		readv(r, br.bs, br.ivs)
	}
}
