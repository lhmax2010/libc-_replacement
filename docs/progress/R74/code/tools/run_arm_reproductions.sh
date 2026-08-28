#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
remote=/opt/usr/home/owner/share/tmp/r74_20260828
local_bin="$workspace/progress/R74/bin/armv7l"
runtime="$workspace/tmp/R68/build-baseline-armv7l/lib"
out="$workspace/progress/R74/raw/arm_runs"
mkdir -p "$out"

run_checked() {
  local log=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "$log"
  set +e
  "$@" >> "$log" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "$rc" >> "$log"
  if [[ "$rc" -ne 0 ]]; then
    return "$rc"
  fi
}

run_checked "$out/001_mkdir.log" "$sdb" shell "mkdir -p '$remote/bin' '$remote/lib'"

for file in "$local_bin"/problem{1,2,3}.{libstdcxx,libcxx}; do
  base=$(basename "$file")
  run_checked "$out/push_${base}.log" "$sdb" push "$file" "$remote/bin/$base"
done
for name in libc++.so.1.0 libc++abi.so.1.0; do
  run_checked "$out/push_${name}.log" "$sdb" push "$runtime/$name" "$remote/lib/$name"
done
run_checked "$out/010_symlinks.log" "$sdb" shell \
  "cd '$remote/lib' && ln -s libc++.so.1.0 libc++.so.1 && ln -s libc++abi.so.1.0 libc++abi.so.1"

run_checked "$out/011_remote_sha256.log" "$sdb" shell \
  "sha256sum '$remote'/bin/* '$remote'/lib/libc++.so.1.0 '$remote'/lib/libc++abi.so.1.0"

run_checked "$out/012_ldd_libstdcxx.log" "$sdb" shell "ldd '$remote/bin/problem1.libstdcxx'"
run_checked "$out/013_ldd_libcxx.log" "$sdb" shell \
  "LD_LIBRARY_PATH='$remote/lib' ldd '$remote/bin/problem1.libcxx'"

printf 'case\tstdlib\trun\texit_code\n' > "$out/run_matrix.tsv"
for case_name in problem1 problem2 problem3; do
  for stdlib in libstdcxx libcxx; do
    for run in 1 2 3 4 5; do
      stdout="$out/${case_name}.${stdlib}.run${run}.stdout"
      stderr="$out/${case_name}.${stdlib}.run${run}.stderr"
      command_file="$out/${case_name}.${stdlib}.run${run}.command.txt"
      if [[ "$stdlib" == libcxx ]]; then
        remote_command="set +e; LD_LIBRARY_PATH='$remote/lib' '$remote/bin/${case_name}.${stdlib}'; rc=\$?; printf '\\n__R74_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
      else
        remote_command="set +e; '$remote/bin/${case_name}.${stdlib}'; rc=\$?; printf '\\n__R74_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
      fi
      command=(timeout --signal=TERM --kill-after=2s 15s "$sdb" shell "$remote_command")
      {
        printf 'COMMAND:'
        printf ' %q' "${command[@]}"
        printf '\n'
      } > "$command_file"
      set +e
      nice -n 15 ionice -c 3 "${command[@]}" > "$stdout" 2> "$stderr"
      transport_rc=$?
      set -e
      printf 'TRANSPORT_EXIT_CODE: %s\n' "$transport_rc" >> "$command_file"
      if [[ "$transport_rc" -ne 0 ]]; then
        echo "transport failure for $case_name $stdlib run $run" >&2
        return "$transport_rc" 2>/dev/null || exit "$transport_rc"
      fi
      remote_rc=$(sed -n 's/^__R74_REMOTE_RC__=//p' "$stdout")
      if [[ -z "$remote_rc" || "$remote_rc" == *$'\n'* ]]; then
        echo "missing or duplicate remote rc for $case_name $stdlib run $run" >&2
        exit 10
      fi
      printf 'REMOTE_EXIT_CODE: %s\n' "$remote_rc" >> "$command_file"
      printf '%s\t%s\t%s\t%s\n' "$case_name" "$stdlib" "$run" "$remote_rc" >> "$out/run_matrix.tsv"
    done
  done
done

cat "$out/run_matrix.tsv"
