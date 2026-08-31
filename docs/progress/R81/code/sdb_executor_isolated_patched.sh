#!/usr/bin/env bash
set -euo pipefail

exec /home/toolchain/development/libc++_replacement/docs/progress/R77/code/sdb_executor.py \
  --env TMPDIR=/opt/usr/r81_fixture/patched/tmp \
        TMP=/opt/usr/r81_fixture/patched/tmp \
        TEMP=/opt/usr/r81_fixture/patched/tmp \
  "$@"
