FROM ubuntu:focal

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends build-essential \
    cmake supervisor autoconf automake libtool git wget ca-certificates curl libcurl4-openssl-dev && mkdir -p /deps && cd /deps

RUN cd /deps && \
    curl -L https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz -o libevent-2.1.8-stable.tar.gz && \
    tar xvf libevent-2.1.8-stable.tar.gz && \
    cd libevent-2.1.8-stable && \
    ./configure && \
    make -j8 && \
    make -j8 install

RUN cd /deps && \
    wget https://github.com/google/glog/archive/v0.3.5.tar.gz && \
    tar xvf v0.3.5.tar.gz && cd glog-0.3.5 && \
    ./configure && \
    make -j8 && \
    make -j8 install

RUN cd /deps && \
    git clone https://github.com/gperftools/gperftools.git && \
    cd gperftools && \
    git checkout gperftools-2.7 && \
    (./autogen.sh || ./autogen.sh) && \
    ./configure && \
    make -j8 && \
    make -j8 install

RUN cd /deps && \
    git clone https://github.com/corehacker/ch-cpp-utils.git && \
    cd ch-cpp-utils && \
    ./autogen.sh && \
    ./configure && \
    make -j8 && \
    make -j8 install

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends pkg-config

RUN cd /deps && \
    git clone https://github.com/corehacker/ch-ifconfig-mailer.git && \
    cd ch-ifconfig-mailer && \
    ./autogen.sh && \
    ./configure && \
    make -j8 && \
    make -j8 install

