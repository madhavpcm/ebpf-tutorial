#include "vmlinux.h"
#include <bpf/bpf_helpers.h>


SEC("tracepoint/syscalls/sys_enter_write")
int trace_sys_enter_write(struct trace_event_raw_sys_enter *ctx)
{
    u64 pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = pid_tgid >> 32; // Extract PID

    bpf_printk("sys_enter_write: PID = %d\n", pid);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
