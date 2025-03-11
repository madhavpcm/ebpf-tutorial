#include "common.h"

void* read_trace_pipe( void* arg ) {
	int fd = open( "/sys/kernel/debug/tracing/trace_pipe", O_RDONLY );
	if ( fd < 0 ) {
		perror( "Failed to open trace_pipe" );
		return NULL;
	}

	char buffer[ 4096 ];
	while ( keep_running ) {
		ssize_t bytes_read = read( fd, buffer, sizeof( buffer ) - 1 );
		if ( bytes_read > 0 ) {
			buffer[ bytes_read ] = '\0';
			fputs( buffer, stdout );
			fflush( stdout );
		}
	}

	close( fd );
	return NULL;
}
