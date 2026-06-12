#!/usr/bin/env bash
set -euo pipefail

if [ "$(id -u)" -eq 0 ]; then
    host_uid="${HOST_UID:-1000}"
    host_gid="${HOST_GID:-1000}"

    if ! getent group "$host_gid" >/dev/null; then
        groupadd -g "$host_gid" hm64
    fi

    if ! getent passwd "$host_uid" >/dev/null; then
        useradd -m -u "$host_uid" -g "$host_gid" -s /bin/bash hm64
    fi

    user_name="$(getent passwd "$host_uid" | cut -d: -f1)"
    export HOME="$(getent passwd "$host_uid" | cut -d: -f6)"
    cd /repo

    # Host-built binaries may not run inside the Debian image.
    rm -f tools/build/mkldscript
    runuser -u "$user_name" -- \
        "${HOST_CC:-gcc}" \
        -o tools/build/mkldscript \
        tools/build/mkldscript.c \
        tools/build/spec.c \
        tools/build/util.c

    makeflags="${MAKEFLAGS:-}"
    makeflags="$makeflags PYTHON=${PYTHON:-/opt/hm64-venv/bin/python}"
    makeflags="$makeflags KMC_PATH=${KMC_PATH:-/opt/gcc-2.7.2/}"
    makeflags="$makeflags MODERN_GCC=${MODERN_GCC:-0}"

    exec runuser -u "$user_name" -- env \
        HOME="$HOME" \
        PATH="$PATH" \
        PYTHON="${PYTHON:-/opt/hm64-venv/bin/python}" \
        KMC_PATH="${KMC_PATH:-/opt/gcc-2.7.2/}" \
        MODERN_GCC="${MODERN_GCC:-0}" \
        MAKEFLAGS="$makeflags" \
        "$@"
fi

exec "$@"
