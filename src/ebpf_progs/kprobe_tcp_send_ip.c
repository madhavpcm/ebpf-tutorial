#include "../vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
// tcp_kprobe.bpf.c

char LICENSE[] SEC( "license" ) = "GPL";

SEC( "kprobe/tcp_connect" )
int BPF_KPROBE( trace_tcp_connect, struct sock* sk ) {

	__u32 src_ip	= BPF_CORE_READ( sk, __sk_common.skc_rcv_saddr );
	__u32 dest_ip	= BPF_CORE_READ( sk, __sk_common.skc_daddr );
	__u16 src_port	= BPF_CORE_READ( sk, __sk_common.skc_num );
	__u16 dest_port = BPF_CORE_READ( sk, __sk_common.skc_dport );

	bpf_printk( "TCP: %pI4:%d -> %pI4:%d\n", &src_ip, src_port, &dest_ip, dest_port );
	return 0;
}
