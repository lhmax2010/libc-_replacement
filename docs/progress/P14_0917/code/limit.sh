#!/bin/bash
set -euo pipefail
ulimit -c 0
ulimit -v 4194304
exec nice -n 19 ionice -c 3 "$@"
