# dev image
FROM ubuntu:22.04 AS dev

ARG DEBIAN_FRONTEND=noninteractive

# install apt packages
RUN apt-get update && apt-get -y install \
    bash \
    vim \
    build-essential \
    gdb \
    git-all \
    curl
