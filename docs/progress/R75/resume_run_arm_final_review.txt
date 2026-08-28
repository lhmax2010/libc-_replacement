#!/usr/bin/env bash
set -e
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
sdb=/home/toolchain/.local/bin/sdb
remote=/opt/usr/home/owner/share/tmp/r75_20260828
bin="${workspace}/progress/R75/bin/armv7l"
runtime="${workspace}/tmp/R69/build-armv7l/lib"
out="${workspace}/progress/R75/raw/arm_final"
mkdir -p "${out}"

run_checked() {
  local log=$1
  shift
  {
    printf 'COMMAND:'
    printf ' %q' "$@"
    printf '\n'
  } > "${log}"
  set +e
  "$@" >> "${log}" 2>&1
  local rc=$?
  set -e
  printf 'EXIT_CODE: %s\n' "${rc}" >> "${log}"
  return "${rc}"
}

run_checked "${out}/001_devices.log" "${sdb}" devices
run_checked "${out}/002_preflight_processes.log" "${sdb}" shell "ps -ef"
run_checked "${out}/003_preflight_path.log" "${sdb}" shell "if test -e '${remote}'; then find '${remote}' -maxdepth 3 -print; else echo R75_PATH_ABSENT; fi"
run_checked "${out}/004_mkdir.log" "${sdb}" shell "mkdir -p '${remote}/bin' '${remote}/lib'"

for stdlib in libstdcxx libcxx; do
  run_checked "${out}/push_${stdlib}.log" "${sdb}" push "${bin}/shrink_to_fit_reproducer.${stdlib}" "${remote}/bin/shrink_to_fit_reproducer.${stdlib}"
done
for library in libc++.so.1.0 libc++abi.so.1.0; do
  run_checked "${out}/push_${library}.log" "${sdb}" push "${runtime}/${library}" "${remote}/lib/${library}"
done
run_checked "${out}/005_symlinks.log" "${sdb}" shell "cd '${remote}/lib' && ln -sf libc++.so.1.0 libc++.so.1 && ln -sf libc++abi.so.1.0 libc++abi.so.1"
run_checked "${out}/006_identity.log" "${sdb}" shell "sha256sum '${remote}/bin/shrink_to_fit_reproducer.libstdcxx' '${remote}/bin/shrink_to_fit_reproducer.libcxx' '${remote}/lib/libc++.so.1.0' '${remote}/lib/libc++abi.so.1.0'; ldd '${remote}/bin/shrink_to_fit_reproducer.libstdcxx'; LD_LIBRARY_PATH='${remote}/lib' ldd '${remote}/bin/shrink_to_fit_reproducer.libcxx'"

printf 'stdlib\trun\ttransport_rc\tremote_rc\ttarget_seen\tprepared_seen\n' > "${out}/matrix.tsv"
for stdlib in libstdcxx libcxx; do
  for run in $(seq 1 20); do
    stem="${out}/${stdlib}.run$(printf '%02d' "${run}")"
    if [[ "${stdlib}" == libcxx ]]; then
      remote_command="set +e; LD_LIBRARY_PATH='${remote}/lib' '${remote}/bin/shrink_to_fit_reproducer.libcxx'; rc=\$?; printf '\\n__R75_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
    else
      remote_command="set +e; '${remote}/bin/shrink_to_fit_reproducer.libstdcxx'; rc=\$?; printf '\\n__R75_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
    fi
    {
      printf 'COMMAND:'
      printf ' %q' timeout --signal=TERM --kill-after=2s 15s "${sdb}" shell "${remote_command}"
      printf '\n'
    } > "${stem}.command"
    set +e
    timeout --signal=TERM --kill-after=2s 15s "${sdb}" shell "${remote_command}" > "${stem}.stdout" 2> "${stem}.stderr"
    transport_rc=$?
    set -e
    remote_rc=$(awk -F= '/^__R75_REMOTE_RC__=/{value=$2} END{gsub(/[^0-9]/,"",value); print value}' "${stem}.stdout")
    if [[ ! "${remote_rc}" =~ ^[0-9]+$ ]]; then
      remote_rc=NOT_AVAILABLE
    fi
    target_seen=$(awk '/EVENT target_allocate/{found=1} END{print found+0}' "${stem}.stdout" "${stem}.stderr")
    prepared_seen=$(awk '/EVENT prepared/{found=1} END{print found+0}' "${stem}.stdout" "${stem}.stderr")
    printf 'TRANSPORT_EXIT_CODE: %s\nREMOTE_EXIT_CODE: %s\n' "${transport_rc}" "${remote_rc}" >> "${stem}.command"
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "${stdlib}" "${run}" "${transport_rc}" "${remote_rc}" "${target_seen}" "${prepared_seen}" >> "${out}/matrix.tsv"
  done
done

for stdlib in libstdcxx libcxx; do
  stem="${out}/${stdlib}.ld_debug"
  if [[ "${stdlib}" == libcxx ]]; then
    remote_command="set +e; LD_DEBUG=libs LD_LIBRARY_PATH='${remote}/lib' '${remote}/bin/shrink_to_fit_reproducer.libcxx'; rc=\$?; printf '\\n__R75_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
  else
    remote_command="set +e; LD_DEBUG=libs '${remote}/bin/shrink_to_fit_reproducer.libstdcxx'; rc=\$?; printf '\\n__R75_REMOTE_RC__=%s\\n' \"\$rc\"; exit 0"
  fi
  set +e
  timeout --signal=TERM --kill-after=2s 15s "${sdb}" shell "${remote_command}" > "${stem}.stdout" 2> "${stem}.stderr"
  transport_rc=$?
  set -e
  printf 'COMMAND: board LD_DEBUG identity run for %s\nTRANSPORT_EXIT_CODE: %s\n' "${stdlib}" "${transport_rc}" > "${stem}.command"
done

awk -F '\t' 'NR>1 {key=$1 ":transport=" $3 ":remote=" $4 ":target=" $5 ":prepared=" $6; count[key]++} END {for (key in count) print key "\t" count[key]}' \
  "${out}/matrix.tsv" | sort > "${out}/summary.tsv"

run_checked "${out}/007_cleanup.log" "${sdb}" shell "rm -rf '${remote}'"
run_checked "${out}/008_cleanup_verify.log" "${sdb}" shell "if test -e '${remote}'; then echo R75_PATH_REMAINS; exit 1; else echo R75_PATH_REMOVED; fi"
run_checked "${out}/009_process_verify.log" "${sdb}" shell "ps -ef"
