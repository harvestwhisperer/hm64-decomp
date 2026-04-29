FROM debian:bookworm

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        binutils-mips-linux-gnu \
        git \
        make \
        python3 \
        python3-pip \
        python3-venv \
        wget \
    && rm -rf /var/lib/apt/lists/*

COPY tools/requirements.txt /tmp/hm64-requirements.txt
RUN python3 -m venv /opt/hm64-venv \
    && /opt/hm64-venv/bin/python -m pip install -U pip \
    && /opt/hm64-venv/bin/python -m pip install -U -r /tmp/hm64-requirements.txt

RUN mkdir -p /opt/gcc-2.7.2 \
    && wget -c "https://github.com/decompals/mips-gcc-2.7.2/releases/download/main/gcc-2.7.2-linux.tar.gz" -O - \
        | tar -xz -C /opt/gcc-2.7.2 \
    && wget -c "https://github.com/decompals/mips-binutils-2.6/releases/latest/download/binutils-2.6-linux.tar.gz" -O - \
        | tar -xz -C /opt/gcc-2.7.2 \
    && chmod -R +x /opt/gcc-2.7.2

COPY tools/docker-entrypoint.sh /usr/local/bin/hm64-docker-entrypoint
RUN chmod +x /usr/local/bin/hm64-docker-entrypoint

ENV PYTHON=/opt/hm64-venv/bin/python
ENV KMC_PATH=/opt/gcc-2.7.2/
ENV MODERN_GCC=0

WORKDIR /repo
ENTRYPOINT ["/usr/local/bin/hm64-docker-entrypoint"]
CMD ["bash"]
