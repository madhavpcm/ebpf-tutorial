#include <stdio.h>
#include <stdlib.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ebpf_object_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *bpf_obj_file = argv[1];
    struct bpf_object *obj;
    int prog_fd;
    
    // Load eBPF object file
    obj = bpf_object__open_file(bpf_obj_file, NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open eBPF object file: %s\n", bpf_obj_file);
        return EXIT_FAILURE;
    }

    // Load eBPF program into the kernel
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load eBPF object\n");
        bpf_object__close(obj);
        return EXIT_FAILURE;
    }

    // Find first program in the object
    struct bpf_program *prog = NULL;
    bpf_object__for_each_program(prog, obj) {
        prog_fd = bpf_program__fd(prog);
        if (prog_fd < 0) {
            fprintf(stderr, "Failed to get eBPF program FD\n");
            bpf_object__close(obj);
            return EXIT_FAILURE;
        }
        break; // Attach the first program found
    }

    printf("eBPF program loaded successfully (FD: %d)\n", prog_fd);

    // Keep program running
    printf("Press Ctrl+C to exit...\n");
    while (1) {
        sleep(10);
    }

    bpf_object__close(obj);
    return EXIT_SUCCESS;
}
