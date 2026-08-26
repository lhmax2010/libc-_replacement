#!/usr/bin/env bash
# Verify the final Layer C 20/30 split and the two basic_ostream invariants.

set -u
set -o pipefail

if (( $# != 3 )); then
  printf 'usage: %s HEADER_PATCH LIBRARY_PATCH REPLAY_TREE\n' "$0" >&2
  exit 10
fi

header_patch=$1
library_patch=$2
replay_tree=$3
ostream_header=$replay_tree/libcxx/include/__ostream/basic_ostream.h

for path in "$header_patch" "$library_patch" "$ostream_header"; do
  if [[ ! -r $path ]]; then
    printf 'AUDIT_ERROR unreadable=%s\n' "$path" >&2
    exit 10
  fi
done

header_handlers=$(grep -Ec '^\+.*catch \(const __cxxabiv1::__forced_unwind&\)' "$header_patch")
library_handlers=$(grep -Ec '^\+.*catch \(const __cxxabiv1::__forced_unwind&\)' "$library_patch")
library_ios_handlers=$(awk '
  /^diff --git a\/libcxx\/src\/ios.cpp / {inside=1; next}
  /^diff --git / {inside=0}
  inside && /^\+.*catch \(const __cxxabiv1::__forced_unwind&\)/ {count++}
  END {print count+0}
' "$library_patch")
library_direct_handlers=$((library_handlers - library_ios_handlers))

printf 'header_typed_handlers=%s\n' "$header_handlers"
printf 'library_typed_handlers=%s\n' "$library_handlers"
printf 'library_ios_aggregation_handlers=%s\n' "$library_ios_handlers"
printf 'library_direct_handlers=%s\n' "$library_direct_handlers"
printf 'library_site_total=%s\n' "$((library_direct_handlers + 16))"

status=0
if (( header_handlers != 20 )); then
  printf 'AUDIT_FAIL header handler count expected=20 actual=%s\n' "$header_handlers"
  status=1
fi
if (( library_ios_handlers != 2 || library_direct_handlers != 14 )); then
  printf 'AUDIT_FAIL library shape expected=14_direct+2_aggregators actual=%s_direct+%s_aggregators\n' \
    "$library_direct_handlers" "$library_ios_handlers"
  status=1
fi
if grep -q '^diff --git a/libcxx/include/__ostream/basic_ostream.h ' "$library_patch"; then
  printf 'AUDIT_FAIL library patch unexpectedly changes basic_ostream.h\n'
  status=1
else
  printf 'library_patch_basic_ostream_h=ABSENT\n'
fi
if grep -q '^+#  include <cxxabi.h>$' "$header_patch"; then
  printf 'header_patch_explicit_cxxabi_include=PRESENT\n'
else
  printf 'AUDIT_FAIL header patch does not add explicit cxxabi.h include\n'
  status=1
fi

put_num_block=$(sed -n '/basic_ostream& __put_num(_Tp __value)/,/basic_ostream& __put_num_integer_promote/p' "$ostream_header")
if grep -q 'catch (const __cxxabiv1::__forced_unwind&)' <<<"$put_num_block"; then
  printf 'AUDIT_FAIL __put_num still contains a direct forced-unwind handler\n'
  status=1
else
  printf '__put_num_direct_forced_handler=ABSENT\n'
fi
if grep -q 'this->__set_badbit_and_consider_rethrow();' <<<"$put_num_block"; then
  printf '__put_num_aggregation_call=PRESENT\n'
else
  printf 'AUDIT_FAIL __put_num aggregation call is absent\n'
  status=1
fi
if grep -q '^#  include <cxxabi.h>$' "$ostream_header"; then
  printf 'replay_explicit_cxxabi_include=PRESENT\n'
else
  printf 'AUDIT_FAIL replay tree lacks explicit cxxabi.h include\n'
  status=1
fi

if (( status == 0 )); then
  printf 'semantic_split_audit=PASS\n'
else
  printf 'semantic_split_audit=FAIL\n'
fi
exit "$status"
