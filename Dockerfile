FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    clang \
    llvm \
    gcc \
    sudo \
    vim \
    make \
    libelf-dev \
    libbfd-dev \
    libcap-dev \
    libbpf-dev \
    iproute2 \
    iputils-ping \
    git \
    wget \
    curl \
    pkg-config \
    python3 \
    python3-pip \
    python3-setuptools \
    python3-wheel \
    jq \
    && rm -rf /var/lib/apt/lists/*

# Install bpftool
RUN apt-get update && apt-get install -y linux-tools-common linux-tools-generic && \
    ln -s /usr/lib/linux-tools/*/bpftool /usr/local/bin/bpftool || true

# Clone and build libbpf
#WORKDIR /opt
#RUN git clone --depth 1 https://github.com/libbpf/libbpf.git && \
#    cd libbpf/src && make && make install_headers && cd ../..

# Set environment variables
#ENV LIBBPF_DIR=/opt/libbpf
#ENV PATH=$LIBBPF_DIR/src:$PATH

# Create working directory
WORKDIR /workspace
COPY src .

CMD ["/bin/bash"]
