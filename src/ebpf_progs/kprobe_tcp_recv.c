#include "../vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

SEC( "kprobe/tcp_v4_rcv" )
int trace_tcp_rcv( struct pt_regs* ctx ) {
	struct sk_buff* skb = ( struct sk_buff* )PT_REGS_PARM1( ctx );
	bpf_printk( "TCP packet received: skb=%p\n", skb );
	return 0;
}

char _license[] SEC( "license" ) = "GPL";
