FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    curl \
    vim \
    git \
    build-essential \
    python3 \
    python3-pip \
    man-db \
    && apt-get clean

SHELL ["/bin/bash", "-c"]

WORKDIR /home/ubuntu