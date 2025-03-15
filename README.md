
# Tutorial repo for Introduction to eBPF workshop


## Directory Structure

```
├── Dockerfile # Docker image to run files in src
├── monitoring # Prometheus/Monitoring for exporter testing
│   ├── docker-compose.yml 
│   ├── grafana.yaml
│   └── prometheus.yaml
├── README.md
└── src
    ├── common.c 
    ├── common.h
    ├── ebpf_objs/ # ebpf objs (compiled programs)
    ├── ebpf_progs/ # ebpf programs
    ├── libbpf/ # submodule / libbpf depenency
    ├── loader
    ├── loader.c
    ├── Makefile # has scripts to build
    └── vmlinux.h # generated file for your machine
```

```
docker run -ti -v /usr/src:/usr/src:ro \
        -v /lib/modules/:/lib/modules:ro \
        -v /sys/kernel/debug/:/sys/kernel/debug:rw \
        --net=host --pid=host --privileged \
        -d --dns=1.1.1.1 --name ebpf ebpf-tutorial:0.2
```
