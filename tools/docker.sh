#!/usr/bin/env bash
set -euo pipefail

image="${HM64_DOCKER_IMAGE:-hm64-decomp:bookworm}"
host_uid="${SUDO_UID:-$(id -u)}"
host_gid="${SUDO_GID:-$(id -g)}"

docker build -t "$image" -f Dockerfile .
docker run --rm -it \
    -v "$PWD":/repo \
    -w /repo \
    -e HOST_UID="$host_uid" \
    -e HOST_GID="$host_gid" \
    "$image" "$@"
