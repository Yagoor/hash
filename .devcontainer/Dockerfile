FROM mcr.microsoft.com/vscode/devcontainers/base:bullseye

RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential \
        valgrind libcmocka-dev \
        uncrustify libxml2-utils \
        doxygen graphviz \
        uuid-dev valgrind \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*