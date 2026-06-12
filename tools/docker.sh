#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd -- "$script_dir/.." && pwd)"

image="${HM64_DOCKER_IMAGE:-hm64-decomp:bookworm}"
platform="${HM64_DOCKER_PLATFORM:-linux/amd64}"
host_uid="${SUDO_UID:-$(id -u)}"
host_gid="${SUDO_GID:-$(id -g)}"

docker build --platform "$platform" -t "$image" -f "$repo_root/Dockerfile" "$repo_root"

run_args=(
    --rm
    --platform "$platform"
    -v "$repo_root:/repo"
    -w /repo
    -e "HOST_UID=$host_uid"
    -e "HOST_GID=$host_gid"
)

if [ -t 0 ] && [ -t 1 ]; then
    run_args+=(-it)
fi

docker run "${run_args[@]}" "$image" "$@"
