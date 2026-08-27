#!/usr/bin/env bash
set -euo pipefail

compiler=/home/toolchain/development/libc++_replacement/progress/R36/tools/armv7l-clang++
exec "$compiler" -Wno-unused-command-line-argument "$@"
