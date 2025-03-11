#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static int keep_running = 1;

// Tail the output from trace_pipe
void* read_trace_pipe( void* arg );
