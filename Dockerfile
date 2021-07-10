FROM archlinux:latest
LABEL maintainer="vilhelm.engstrom@tuta.io"

RUN pacman -Syu --needed --noconfirm make clang gcc

ENV CXX=g++

COPY . /statvec
WORKDIR /statvec
