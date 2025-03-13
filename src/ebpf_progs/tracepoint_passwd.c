#include "../vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

// Define a constant to track the target file
const char target_file[] = "/etc/passwd";

SEC( "tracepoint/syscalls/sys_enter_openat" )
int trace_openat( struct trace_event_raw_sys_enter* ctx ) {
	u64 id				 = bpf_get_current_pid_tgid();
	u32 pid				 = id >> 32;
	const char* filename = ( const char* )ctx->args[ 1 ];

	// Read this and put in a finite char array
	char file[ 32 ];
	if ( bpf_probe_read_user_str( file, sizeof( file ), filename ) < 0 )
		return 0;

	// Check if the opened file is "/etc/passwd"
	if ( __builtin_strcmp( file, target_file ) == 0 ) {
		bpf_printk( "PID %d opened %s\n", pid, file );
	}

	return 0;
}

char LICENSE[] SEC( "license" ) = "GPL";
