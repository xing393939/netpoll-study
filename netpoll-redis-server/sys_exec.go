package main

import (
	"syscall"
	"unsafe"
)

func GetSysFdPairs() (r, w int) {
	fds, _ := syscall.Socketpair(syscall.AF_UNIX, syscall.SOCK_STREAM, 0)
	return fds[0], fds[1]
}

type barrier struct {
	bs  [][]byte
	ivs []syscall.Iovec
}

func writev(fd int, bs [][]byte, ivs []syscall.Iovec) (n int, err error) {
	var r uintptr
	var e syscall.Errno

	iovLen := iovecs(bs, ivs)
	if iovLen > 0 {
		r, _, e = syscall.RawSyscall(syscall.SYS_WRITEV, uintptr(fd), uintptr(unsafe.Pointer(&ivs[0])), uintptr(iovLen))
	}
	if e != 0 {
		return int(r), syscall.Errno(e)
	}
	return int(r), nil
}

func readv(fd int, bs [][]byte, ivs []syscall.Iovec) (n int, err error) {
	var r uintptr
	var e syscall.Errno

	iovLen := iovecs(bs, ivs)
	if iovLen > 0 {
		r, _, e = syscall.RawSyscall(syscall.SYS_READV, uintptr(fd), uintptr(unsafe.Pointer(&ivs[0])), uintptr(iovLen))
	}
	if e != 0 {
		return int(r), syscall.Errno(e)
	}
	return int(r), nil
}

// TODO: read from sysconf(_SC_IOV_MAX)? The Linux default is
//  1024 and this seems conservative enough for now. Darwin's
//  UIO_MAXIOV also seems to be 1024.
func iovecs(bs [][]byte, ivs []syscall.Iovec) (iovLen int) {
	for i := 0; i < len(bs); i++ {
		if len(bs[i]) == 0 {
			continue
		}
		// append
		ivs[iovLen].Base = &bs[i][0]
		ivs[iovLen].SetLen(len(bs[i]))
		iovLen++
	}
	return iovLen
}

func sendmsg(fd int, bs [][]byte, ivs []syscall.Iovec, zerocopy bool) (n int, err error) {
	iovLen := iovecs(bs, ivs)
	if iovLen == 0 {
		return 0, nil
	}
	var flags uintptr
	if zerocopy {
		// MSG_ZEROCOPY flag
		flags = 0x4000000
	}
	var msghdr = syscall.Msghdr{
		Iov:    &ivs[0],
		Iovlen: uint64(iovLen),
	}
	r, _, e := syscall.RawSyscall(syscall.SYS_SENDMSG, uintptr(fd), uintptr(unsafe.Pointer(&msghdr)), flags)
	if e != 0 {
		return int(r), syscall.Errno(e)
	}
	return int(r), nil
}
