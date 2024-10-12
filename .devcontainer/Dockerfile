# dev image
FROM ubuntu:22.04 AS dev

ARG DEBIAN_FRONTEND=noninteractive

# install apt packages
RUN apt-get update && apt-get -y install \
    bash \
    build-essential \
    cmake \
    curl \
    gdb \
    git-all \
    python3 \
    vim

ENV PROJECT_SOURCE_DIR /workspaces/passman

