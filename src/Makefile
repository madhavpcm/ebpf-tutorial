LIBBPF_DIR = libbpf
LIBBPF_SRC = $(LIBBPF_DIR)/src
LIBBPF_REPO = https://github.com/libbpf/libbpf.git

BPFTDIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BPFTARGET ?= /sys/kernel/btf/vmlinux

DESTDIR = root
CC = gcc
CFLAGS = -I./usr/include/
LDFLAGS = -L./$(LIBBPF_SRC)/ -Wl,-rpath=./$(LIBBPF_SRC)/ -lbpf -lelf
LOADER_SRC = loader.c
LOADER_BIN = ebpf_loader

CLANG = clang
LLC = llc
BPF_CFLAGS = -target bpf -S -D __BPF_TRACING__ -I/usr/include/ \
             -Wall -Werror -O2 -emit-llvm -c -g

.PHONY: all clean install uninstall loader ebpfobj

all: libbpf install loader

libbpf:
	git submodule update --init --depth 1 $(LIBBPF_DIR)
	make --directory=$(LIBBPF_SRC) all
	bpftool btf dump file $(BPFTARGET) format c > vmlinux.h

install: libbpf
	DESTDIR=$(DESTDIR) make --directory=$(LIBBPF_SRC) install_headers

loader: install
	$(CC) $(CFLAGS) $(LOADER_SRC) $(LDFLAGS) -o $(LOADER_BIN)

ebpfobj: install
	@if [ -z "$(EBPF_PROG)" ]; then \
		echo "Usage: make ebpfobj EBPF_PROG=kill.c"; \
		exit 1; \
	fi
	$(CLANG) $(BPF_CFLAGS) -o $(EBPF_PROG:.c=.ll) $(EBPF_PROG)
	$(LLC) -march=bpf -filetype=obj -o $(EBPF_PROG:.c=.o) $(EBPF_PROG:.c=.ll)
	@echo "Generated eBPF object: $(EBPF_PROG:.c=.o)"

uninstall:
	rm -rf $(DESTDIR)/usr/include/bpf

clean:
	rm -rf $(LIBBPF_DIR) $(LOADER_BIN) *.o *.ll
	rm vmlinux.h
