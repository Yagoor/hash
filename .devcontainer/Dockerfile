ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/base:${VARIANT}

RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential \
        valgrind libcmocka-dev \
        uncrustify libxml2-utils \
        doxygen graphviz \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*