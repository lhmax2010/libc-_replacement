#!/usr/bin/env bash
set -euo pipefail

exec /home/toolchain/development/libc++_replacement/docs/progress/R77/code/sdb_executor.py \
  --env TMPDIR=/opt/usr/r81_phase2/root_patched/tmp \
        TMP=/opt/usr/r81_phase2/root_patched/tmp \
        TEMP=/opt/usr/r81_phase2/root_patched/tmp \
  "$@"
