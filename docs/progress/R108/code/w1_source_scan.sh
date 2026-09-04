#!/bin/bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
  echo "usage: $0 ROOT_LIST OUTPUT" >&2
  exit 2
fi

roots_file=$1
output=$2
mapfile -t roots <"$roots_file"
if [ "${#roots[@]}" -eq 0 ]; then
  echo "empty root list" >&2
  exit 2
fi

# The deliberately lexical scan retains every textual occurrence for manual
# classification.  It covers qualified uses, using declarations, typedefs,
# aliases and template arguments as long as the token survives in source.
set +e
rg --no-ignore --hidden --threads 2 -n -H --color never \
  --glob '*.{c,cc,cpp,cxx,C,h,hh,hpp,hxx,ipp,tcc,inl,inc,m,mm,cu,cuh,hip,sycl,i,ii}' \
  --glob '*.spec' --glob '*.cmake' --glob 'CMakeLists.txt' \
  '(^|[^[:alnum:]_])wbuffer_convert([^[:alnum:]_]|$)' \
  "${roots[@]}" >"$output"
rc=$?
set -e
if [ "$rc" -eq 1 ]; then
  : >"$output"
  rc=0
fi
printf 'roots=%s\n' "${#roots[@]}"
printf 'matches=%s\n' "$(wc -l <"$output")"
exit "$rc"
