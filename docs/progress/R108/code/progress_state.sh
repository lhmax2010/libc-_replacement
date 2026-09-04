#!/bin/bash
set -euo pipefail

phase=${1:-w1}
case "$phase" in
  w1)
    ready=0
    for number in 012 015; do
      if compgen -G "docs/progress/R108/raw/${number}_*.exitcode" >/dev/null; then
        ready=$((ready + 1))
      fi
    done
    printf 'phase=1 ready=%d total=2\n' "$ready"
    ;;
  *)
    printf 'phase=unknown\n'
    exit 2
    ;;
esac
