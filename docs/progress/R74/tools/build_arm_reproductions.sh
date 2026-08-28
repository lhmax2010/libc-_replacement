#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/progress/R74/raw/arm_build"
bin="$workspace/progress/R74/bin/armv7l"
mkdir -p "$out" "$bin"

cxx="$workspace/progress/R36/tools/armv7l-clang++"
headers="$workspace/tmp/R68/build-baseline-armv7l/include/c++/v1"
runtime="$workspace/tmp/R68/build-baseline-armv7l/lib"

declare -A sources=(
  [problem1]="$workspace/progress/R74/extracted/04_unlabelled_unlabelled.cpp"
  [problem2]="$workspace/progress/R74/supplemented/06_problem2_cout.cpp"
  [problem3]="$workspace/progress/R74/supplemented/09_problem3_waiter_cancel.cpp"
)

printf 'case\tstdlib\tcompile_rc\tlink_rc\n' > "$out/matrix.tsv"
for case_name in problem1 problem2 problem3; do
  source=${sources[$case_name]}
  for stdlib in libstdcxx libcxx; do
    binary="$bin/${case_name}.${stdlib}"
    object="$bin/${case_name}.${stdlib}.o"
    compile_log="$out/${case_name}.${stdlib}.compile.log"
    link_log="$out/${case_name}.${stdlib}.link.log"
    if [[ "$stdlib" == libstdcxx ]]; then
      compile=("$cxx" -std=c++17 -O0 -g -fexceptions -pthread -c "$source" -o "$object")
      link=("$cxx" "$object" -pthread -Wl,-z,now -o "$binary")
    else
      compile=("$cxx" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ -I"$headers" -c "$source" -o "$object")
      link=("$cxx" "$object" -nostdlib++ -L"$runtime" -Wl,-z,now -lc++ -lc++abi -lpthread -ldl -lm -latomic -lc -lgcc_s -lgcc -o "$binary")
    fi
    {
      printf 'COMMAND:'
      printf ' %q' "${compile[@]}"
      printf '\n'
    } > "$compile_log"
    set +e
    nice -n 15 ionice -c 3 "${compile[@]}" >> "$compile_log" 2>&1
    compile_rc=$?
    set -e
    printf 'EXIT_CODE: %s\n' "$compile_rc" >> "$compile_log"
    link_rc=125
    if [[ "$compile_rc" -eq 0 ]]; then
      {
        printf 'COMMAND:'
        printf ' %q' "${link[@]}"
        printf '\n'
      } > "$link_log"
      set +e
      nice -n 15 ionice -c 3 "${link[@]}" >> "$link_log" 2>&1
      link_rc=$?
      set -e
      printf 'EXIT_CODE: %s\n' "$link_rc" >> "$link_log"
    else
      printf 'NOT_RUN: compile failed\nEXIT_CODE: 125\n' > "$link_log"
    fi
    printf '%s\t%s\t%s\t%s\n' "$case_name" "$stdlib" "$compile_rc" "$link_rc" >> "$out/matrix.tsv"
  done
done

file "$bin"/* > "$out/file_identity.txt"
sha256sum "$bin"/problem*.libstdcxx "$bin"/problem*.libcxx > "$out/sha256.txt"
cat "$out/matrix.tsv"
