#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/progress/R74/raw/raw_compile"
bin="$workspace/progress/R74/bin/raw"
mkdir -p "$out" "$bin"

gnu_cxx=/usr/bin/g++
libcxx_cxx="$workspace/progress/R33/tools/tizen-clang++"

printf 'block\tstdlib\texit_code\tbinary\n' > "$out/matrix.tsv"

for source in "$workspace"/progress/R74/extracted/[0-9][0-9]_*.cpp; do
  block=$(basename "$source" .cpp)
  for stdlib in libstdcxx libcxx; do
    stdout="$out/${block}.${stdlib}.stdout"
    stderr="$out/${block}.${stdlib}.stderr"
    command_file="$out/${block}.${stdlib}.command.txt"
    binary="$bin/${block}.${stdlib}"
    if [[ "$stdlib" == libstdcxx ]]; then
      command=("$gnu_cxx" -std=c++17 -pthread "$source" -o "$binary")
    else
      command=("$libcxx_cxx" -std=c++17 -stdlib=libc++ -pthread "$source" -o "$binary")
    fi
    {
      printf 'COMMAND:'
      printf ' %q' "${command[@]}"
      printf '\n'
    } > "$command_file"
    set +e
    nice -n 15 ionice -c 3 "${command[@]}" > "$stdout" 2> "$stderr"
    rc=$?
    set -e
    printf 'EXIT_CODE: %s\n' "$rc" >> "$command_file"
    printf '%s\t%s\t%s\t%s\n' "$block" "$stdlib" "$rc" "$binary" >> "$out/matrix.tsv"
  done
done

cat "$out/matrix.tsv"
