FROM ubuntu:22.04

RUN apt update \
    && apt-install -y \
        build-essential \
        make \
        gcc \
        libm-dev \
        bash \
        valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /demo
COPY ./ ./
RUN make compile

CMD ["./demo", "-n", "15"]