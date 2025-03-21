#include <stdio.h>
#include <stdlib.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "common.h"

static struct bpf_object* obj;
void sigint_handler() {
	bpf_object__close( obj );
	printf( "Closed bpf object." );
	keep_running = 0; // Signal the thread to stop
	exit( 0 );
}

int main( int argc, char** argv ) {
	if ( signal( SIGINT, sigint_handler ) == SIG_ERR ) {
		perror( "Error setting up signal handler" );
		return 1;
	}
	pthread_t thread;
	if ( pthread_create( &thread, NULL, read_trace_pipe, NULL ) != 0 ) {
		perror( "Failed to create output thread" );
		return EXIT_FAILURE;
	}

	if ( argc != 2 ) {
		fprintf( stderr, "Usage: %s <ebpf_object_file>\n", argv[ 0 ] );
		return EXIT_FAILURE;
	}

	const char* bpf_obj_file = argv[ 1 ];
	int prog_fd;

	// Load eBPF object file
	obj = bpf_object__open_file( bpf_obj_file, NULL );
	if ( !obj ) {
		fprintf( stderr, "Failed to open eBPF object file: %s\n", bpf_obj_file );
		return EXIT_FAILURE;
	}

	// Load eBPF program into the kernel
	if ( bpf_object__load( obj ) ) {
		fprintf( stderr, "Failed to load eBPF object\n" );
		bpf_object__close( obj );
		return EXIT_FAILURE;
	}

	// Find first program in the object
	struct bpf_program* prog = NULL;
	bpf_object__for_each_program( prog, obj ) {
		prog_fd = bpf_program__fd( prog );
		if ( prog_fd < 0 ) {
			fprintf( stderr, "Failed to get eBPF program FD\n" );
			bpf_object__close( obj );
			return EXIT_FAILURE;
		}
		struct bpf_link* bpf_link = NULL;
		bpf_link				  = bpf_program__attach( prog );
		if ( libbpf_get_error( bpf_link ) ) {
			fprintf( stderr, " attach failed \n" );
			bpf_link = NULL;
		}
		printf( "eBPF program loaded successfully: %s\n", bpf_program__name( prog ) );
	}

	// Attach the program to the tracepoint
	printf( "Program(s) loaded successfully, tail the output in /sys/kernel/debug/tracing/trace_pipe\n" );

	// Keep program running
	printf( "Press Ctrl+C to exit...\n" );
	while ( 1 ) {
		sleep( 10 );
	}

	return EXIT_SUCCESS;
}
