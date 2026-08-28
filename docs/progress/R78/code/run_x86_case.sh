#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "usage: $0 normal|exception|cancel" >&2
  exit 2
fi

root=/home/toolchain/development/libc++_replacement/progress/R78/build/x86_64
export LD_LIBRARY_PATH="$root/runtime:/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu"
export LD_DEBUG=libs
cd "$root"
exec ./new_main ./libold_plugin.so "$1"
