#!/bin/bash
set -euo pipefail
if [ "$#" -ne 2 ]; then
  exit 2
fi
tmp=$(mktemp)
trap 'rm -f "$tmp"' EXIT
nm -C -- "$1" | awk '/atomic/ && /wait/ {print}' >"$tmp"
cp "$tmp" "$2"
printf 'matching_symbols=%s\n' "$(wc -l <"$tmp")"
