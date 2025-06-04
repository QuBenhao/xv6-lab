# Dockerfile for MIT 6.S081 xv6 lab
FROM ubuntu:24.04

# Install essential dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    git \
    build-essential \
    gdb-multiarch \
    qemu-system-misc \
    gcc-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    vim \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set up working directory
ADD . /xv6
WORKDIR /xv6

# Set default command to bash
CMD ["/bin/bash"]