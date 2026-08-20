#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
compiler="$workspace/progress/R33/tools/tizen-clang++"
source_dir="$workspace/progress/R52/type_matrix"
output_dir="$workspace/tmp/R52/type_matrix"
raw_dir="$workspace/progress/R52/raw/type_matrix"
libcxx_include="$workspace/tmp/R51/build-product-x86_64/include/c++/v1"

mkdir -p "$output_dir" "$raw_dir"

overall=0
for stdlib in libstdcxx libcxx; do
  for case_name in const_ref nonconst_ref by_value derive sizeof new delete; do
    source_file="$source_dir/$case_name.cpp"
    object_file="$output_dir/${stdlib}_${case_name}.o"
    log_file="$raw_dir/${stdlib}_${case_name}.log"
    if [[ "$stdlib" == libcxx ]]; then
      command=("$compiler" -std=c++17 -nostdinc++ -I "$libcxx_include" -DUSE_LIBCXX -Wall -Wextra -Werror -c "$source_file" -o "$object_file")
    else
      command=("$compiler" -std=c++17 -Wall -Wextra -Werror -c "$source_file" -o "$object_file")
    fi

    (
      printf '$'
      printf ' %q' "${command[@]}"
      printf '\n'
      "${command[@]}"
      rc=$?
      printf 'EXIT_CODE=%d\n' "$rc"
      exit "$rc"
    ) >"$log_file" 2>&1
    rc=$?
    printf '%s\t%s\t%d\t%s\n' "$stdlib" "$case_name" "$rc" "$log_file"
  done
done

exit "$overall"
