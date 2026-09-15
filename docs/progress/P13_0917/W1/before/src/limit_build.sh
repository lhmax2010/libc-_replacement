#!/bin/bash
set -euo pipefail
# 4 GiB virtual address limit per child, below half of the observed 32 GB host.
# Serial build runner invokes this for each compile/link; no global setting.
ulimit -v 4194304
exec nice -n 19 ionice -c 3 "$@"
