#!/usr/bin/env bash
set -e
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
out="${workspace}/progress/R75/raw/x86_final"
commands="${workspace}/progress/R75/commands"
bin="${workspace}/progress/R75/bin/x86_64"
runtime="${workspace}/tmp/R69/build-x86_64/lib"
mkdir -p "${out}" "${commands}"

printf 'stdlib\trun\texit_code\ttarget_seen\tprepared_seen\n' > "${out}/matrix.tsv"

for stdlib in libstdcxx libcxx; do
  binary="${bin}/shrink_to_fit_reproducer.${stdlib}"
  for run in $(seq 1 20); do
    stdout="${out}/${stdlib}.run$(printf '%02d' "${run}").stdout"
    stderr="${out}/${stdlib}.run$(printf '%02d' "${run}").stderr"
    command_log="${out}/${stdlib}.run$(printf '%02d' "${run}").command"
    if [[ "${stdlib}" == libcxx ]]; then
      command=(timeout --signal=TERM --kill-after=2s 10s env "LD_LIBRARY_PATH=${runtime}" "${binary}")
    else
      command=(timeout --signal=TERM --kill-after=2s 10s "${binary}")
    fi
    {
      printf 'COMMAND:'
      printf ' %q' "${command[@]}"
      printf '\n'
    } > "${command_log}"
    set +e
    nice -n 15 ionice -c 3 "${command[@]}" > "${stdout}" 2> "${stderr}"
    rc=$?
    set -e
    printf 'EXIT_CODE: %s\n' "${rc}" >> "${command_log}"
    target_seen=$(awk '/EVENT target_allocate/{found=1} END{print found+0}' "${stderr}")
    prepared_seen=$(awk '/EVENT prepared/{found=1} END{print found+0}' "${stderr}")
    printf '%s\t%s\t%s\t%s\t%s\n' "${stdlib}" "${run}" "${rc}" "${target_seen}" "${prepared_seen}" >> "${out}/matrix.tsv"
  done
done

{
  printf 'COMMAND: sha256sum binaries and runtime libraries\n'
  sha256sum \
    "${bin}/shrink_to_fit_reproducer.libstdcxx" \
    "${bin}/shrink_to_fit_reproducer.libcxx" \
    "${runtime}/libc++.so.1.0" \
    "${runtime}/libc++abi.so.1.0"
  printf 'EXIT_CODE: 0\n'
} > "${out}/identity.sha256"

{
  printf 'COMMAND: ldd %q\n' "${bin}/shrink_to_fit_reproducer.libstdcxx"
  ldd "${bin}/shrink_to_fit_reproducer.libstdcxx"
  printf 'EXIT_CODE: 0\n'
} > "${out}/libstdcxx.ldd" 2>&1

{
  printf 'COMMAND: env LD_LIBRARY_PATH=%q ldd %q\n' "${runtime}" "${bin}/shrink_to_fit_reproducer.libcxx"
  env "LD_LIBRARY_PATH=${runtime}" ldd "${bin}/shrink_to_fit_reproducer.libcxx"
  printf 'EXIT_CODE: 0\n'
} > "${out}/libcxx.ldd" 2>&1

set +e
LD_DEBUG=libs timeout --signal=TERM --kill-after=2s 10s \
  "${bin}/shrink_to_fit_reproducer.libstdcxx" \
  > "${out}/libstdcxx.ld_debug.stdout" 2> "${out}/libstdcxx.ld_debug.stderr"
rc=$?
set -e
printf 'COMMAND: LD_DEBUG=libs timeout ... shrink_to_fit_reproducer.libstdcxx\nEXIT_CODE: %s\n' "${rc}" > "${out}/libstdcxx.ld_debug.command"

set +e
LD_DEBUG=libs LD_LIBRARY_PATH="${runtime}" timeout --signal=TERM --kill-after=2s 10s \
  "${bin}/shrink_to_fit_reproducer.libcxx" \
  > "${out}/libcxx.ld_debug.stdout" 2> "${out}/libcxx.ld_debug.stderr"
rc=$?
set -e
printf 'COMMAND: LD_DEBUG=libs LD_LIBRARY_PATH=%q timeout ... shrink_to_fit_reproducer.libcxx\nEXIT_CODE: %s\n' "${runtime}" "${rc}" > "${out}/libcxx.ld_debug.command"

awk -F '\t' 'NR>1 {key=$1 ":rc=" $3 ":target=" $4 ":prepared=" $5; count[key]++} END {for (key in count) print key "\t" count[key]}' \
  "${out}/matrix.tsv" | sort > "${out}/summary.tsv"
