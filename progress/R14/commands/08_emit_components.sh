#!/usr/bin/env bash
set -euo pipefail
set -x

workspace=/home/toolchain/development/libc++_replacement
python3 "$workspace/progress/R14/tools/emit_components.py"
