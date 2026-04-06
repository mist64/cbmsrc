# Alpine Linux based dev container with build dependencies for 'cbmsrc' repository.
# (See https://github.com/devcontainers/templates/tree/main/src/alpine)
FROM mcr.microsoft.com/devcontainers/base:alpine

# Install build dependencies for 'srecord'
RUN apk add --no-cache \
    boost-dev \
    libtool \
    libgcrypt-dev \
    ghostscript \
    groff

# Build 'srecord' from source and install to '/usr/local/bin'
RUN cd /tmp && \
    wget https://sourceforge.net/projects/srecord/files/srecord/1.64/srecord-1.64.tar.gz && \
    tar xf srecord-1.64.tar.gz && \
    cd srecord-1.64 && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    cd /tmp && \
    rm -rf srecord-1.64*

# Build 'kernalemu' from source and install to '/opt/kernalemu/bin'
RUN cd /tmp && \
    git clone https://github.com/mist64/kernalemu.git && \
    cd kernalemu && \
    make -j$(nproc) && \
    mkdir -p /opt/kernalemu/bin && \
    cp build/kernalemu /opt/kernalemu/bin && \
    cd /tmp && \
    rm -rf kernalemu

# Set 'KERNALEMU' environment variable to the path of the 'kernalemu' binary
ENV KERNALEMU=/opt/kernalemu/bin/kernalemu

# Build 'cbm6502asm' from source and install to '/opt/cbm6502asm/bin'
RUN cd /tmp && \
    git clone https://github.com/mist64/cbm6502asm.git && \
    cd cbm6502asm && \
    make -j$(nproc) && \
    mkdir -p /opt/cbm6502asm/bin && \
    cp build/asm /opt/cbm6502asm/bin && \
    cd /tmp && \
    rm -rf cbm6502asm

# Set 'CBM6502ASM' environment variable to the path of the 'cbm6502asm' binary
ENV CBM6502ASM=/opt/cbm6502asm/bin/asm

# Download 'assembler64.prg' from the 'kernalemu' repository and copy to '/opt/assembler64'
RUN mkdir -p /opt/assembler64 && \
    cd /opt/assembler64 && \
    wget https://github.com/mist64/kernalemu/raw/refs/heads/main/demo/assembler64.prg

# Set 'ASSEMBLER64' environment variable to the path of the 'assembler64.prg' file
ENV ASSEMBLER64=/opt/assembler64/assembler64.prg
