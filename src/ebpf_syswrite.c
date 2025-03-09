#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

char LICENSE[] SEC("license") = "GPL";

int uid_inpt;
int fd_inpt;
int len_inpt;

SEC("tp/syscalls/sys_enter_write")
int handle_write(struct trace_event_raw_sys_enter *ctx) {
  int uid = bpf_get_current_uid_gid() >> 32;
  int pid = bpf_get_current_pid_tgid() >> 32;
  char *write_str = (char *)ctx->args[1];
  bpf_printk("BPF triggered from PID:UID [%d:%d] buff %s\n", pid,uid, write_str);
  return 0;
}
