#!/bin/bash

sudo apt-get update && sudo apt install -y python3 binutils-mips-linux-gnu gcc-mips-linux-gnu build-essential

mkdir tools/gcc-2.7.2
mkdir tools/gcc-2.7.2-sn

wget -c "https://github.com/decompals/mips-gcc-2.7.2/releases/download/main/gcc-2.7.2-linux.tar.gz" -O - | tar -xz -C tools/gcc-2.7.2

wget -c "https://github.com/decompals/mips-binutils-2.6/releases/latest/download/binutils-2.6-linux.tar.gz" -O - | tar -xz -C tools/gcc-2.7.2

sudo chmod -R +x tools/gcc-2.7.2

pip3 install -U -r tools/splat/requirements.txt