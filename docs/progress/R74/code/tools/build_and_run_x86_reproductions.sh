#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out="$workspace/progress/R74/raw/x86_runs"
bin="$workspace/progress/R74/bin/x86"
mkdir -p "$out" "$bin"

gnu_cxx=/usr/bin/g++
libcxx_cxx="$workspace/progress/R33/tools/tizen-clang++"
libcxx_lib="$workspace/tmp/R68/build-baseline-x86_64/lib"

declare -A sources=(
  [problem1]="$workspace/progress/R74/extracted/04_unlabelled_unlabelled.cpp"
  [problem2]="$workspace/progress/R74/supplemented/06_problem2_cout.cpp"
  [problem3]="$workspace/progress/R74/supplemented/09_problem3_waiter_cancel.cpp"
  [problem5]="$workspace/progress/R74/extracted/10_unlabelled_unlabelled.cpp"
)

printf 'case\tstdlib\tcompile_rc\n' > "$out/compile_matrix.tsv"
for case_name in problem1 problem2 problem3 problem5; do
  source=${sources[$case_name]}
  for stdlib in libstdcxx libcxx; do
    binary="$bin/${case_name}.${stdlib}"
    log="$out/${case_name}.${stdlib}.compile.log"
    if [[ "$stdlib" == libstdcxx ]]; then
      command=("$gnu_cxx" -std=c++17 -pthread "$source" -o "$binary")
    else
      command=("$libcxx_cxx" -std=c++17 -stdlib=libc++ -pthread "$source" -o "$binary")
    fi
    {
      printf 'COMMAND:'
      printf ' %q' "${command[@]}"
      printf '\n'
    } > "$log"
    set +e
    nice -n 15 ionice -c 3 "${command[@]}" >> "$log" 2>&1
    rc=$?
    set -e
    printf 'EXIT_CODE: %s\n' "$rc" >> "$log"
    printf '%s\t%s\t%s\n' "$case_name" "$stdlib" "$rc" >> "$out/compile_matrix.tsv"
  done
done

printf 'case\tstdlib\trun\texit_code\n' > "$out/run_matrix.tsv"
for case_name in problem1 problem2 problem3 problem5; do
  for stdlib in libstdcxx libcxx; do
    binary="$bin/${case_name}.${stdlib}"
    for run in 1 2 3 4 5; do
      stdout="$out/${case_name}.${stdlib}.run${run}.stdout"
      stderr="$out/${case_name}.${stdlib}.run${run}.stderr"
      command_file="$out/${case_name}.${stdlib}.run${run}.command.txt"
      if [[ "$stdlib" == libstdcxx ]]; then
        command=(timeout --signal=TERM --kill-after=2s 10s "$binary")
      else
        command=(timeout --signal=TERM --kill-after=2s 10s env "LD_LIBRARY_PATH=$libcxx_lib" "$binary")
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
      printf '%s\t%s\t%s\t%s\n' "$case_name" "$stdlib" "$run" "$rc" >> "$out/run_matrix.tsv"
    done
  done
done

cat "$out/compile_matrix.tsv"
cat "$out/run_matrix.tsv"
