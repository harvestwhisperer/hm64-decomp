FROM debian:bookworm

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        binutils-mips-linux-gnu \
        build-essential \
        ca-certificates \
        gcc-mips-linux-gnu \
        git \
        make \
        python3 \
        python3-pip \
        python3-venv \
        wget \
    && rm -rf /var/lib/apt/lists/*

COPY tools/requirements.txt /tmp/hm64-requirements.txt
RUN python3 -m venv /opt/hm64-venv \
    && /opt/hm64-venv/bin/python -m pip install --no-cache-dir -U pip \
    && /opt/hm64-venv/bin/python -m pip install --no-cache-dir -U \
        -r /tmp/hm64-requirements.txt \
        "splat64[mips]<0.40.0"

RUN mkdir -p /opt/gcc-2.7.2 \
    && wget -O /tmp/gcc-2.7.2-linux.tar.gz \
        "https://github.com/decompals/mips-gcc-2.7.2/releases/download/main/gcc-2.7.2-linux.tar.gz" \
    && tar -xzf /tmp/gcc-2.7.2-linux.tar.gz -C /opt/gcc-2.7.2 \
    && wget -O /tmp/binutils-2.6-linux.tar.gz \
        "https://github.com/decompals/mips-binutils-2.6/releases/latest/download/binutils-2.6-linux.tar.gz" \
    && tar -xzf /tmp/binutils-2.6-linux.tar.gz -C /opt/gcc-2.7.2 \
    && rm /tmp/gcc-2.7.2-linux.tar.gz /tmp/binutils-2.6-linux.tar.gz \
    && chmod -R +x /opt/gcc-2.7.2

COPY tools/docker-entrypoint.sh /usr/local/bin/hm64-docker-entrypoint
RUN chmod +x /usr/local/bin/hm64-docker-entrypoint

ENV PYTHON=/opt/hm64-venv/bin/python
ENV KMC_PATH=/opt/gcc-2.7.2/
ENV MODERN_GCC=0
ENV MAKEFLAGS="PYTHON=/opt/hm64-venv/bin/python KMC_PATH=/opt/gcc-2.7.2/ MODERN_GCC=0"

WORKDIR /repo
ENTRYPOINT ["/usr/local/bin/hm64-docker-entrypoint"]
CMD ["bash"]
