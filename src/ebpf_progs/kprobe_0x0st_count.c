#include "../vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
// tcp_kprobe.bpf.c

char LICENSE[] SEC( "license" ) = "GPL";

__u32 match_count SEC( ".bss" ) = 0;

SEC( "kprobe/tcp_connect" )
int BPF_KPROBE( trace_tcp_connect, struct sock* sk ) {

	__u32 src_ip	= BPF_CORE_READ( sk, __sk_common.skc_rcv_saddr );
	__u32 dest_ip	= BPF_CORE_READ( sk, __sk_common.skc_daddr );
	__u16 src_port	= BPF_CORE_READ( sk, __sk_common.skc_num );
	__u16 dest_port = BPF_CORE_READ( sk, __sk_common.skc_dport );

	bpf_printk( "%x\n", dest_ip );
	// 0x0.st IP
	if ( dest_ip == 0x759177A8 ) {
		match_count++;
		bpf_printk( "0x0.st MATCHED, count=%d\n", match_count );
	} else {
		bpf_printk( "TCP: %pI4:%d -> %pI4:%d\n", &src_ip, src_port, &dest_ip, dest_port );
	}

	bpf_printk( "TCP: %pI4:%d -> %pI4:%d\n", &src_ip, src_port, &dest_ip, dest_port );
	return 0;
}
