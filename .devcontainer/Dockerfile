FROM mcr.microsoft.com/vscode/devcontainers/base:bullseye

RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential valgrind \
        uncrustify libxml2-utils doxygen graphviz \
        gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf \
        qemu-system-arm qemu-user gdb-multiarch \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
