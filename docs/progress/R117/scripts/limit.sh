#!/bin/bash
set -euo pipefail
ulimit -v 4194304
ulimit -c 0
exec nice -n 19 ionice -c 3 "$@"
