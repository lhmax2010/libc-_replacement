#!/usr/bin/env bash
set -u
set -o pipefail
sdb=/home/toolchain/.local/bin/sdb
serial=192.168.108.26:26101
remote=/var/tmp/r53_20260820
run_cell() {
  local scenario=$1 suite=$2 path=$3 exe=$4 arg=${5-}
  command="set +e; LD_LIBRARY_PATH='$path' '$exe' '$arg' & pid=\$!; (sleep 15; kill -TERM \$pid 2>/dev/null; sleep 1; kill -KILL \$pid 2>/dev/null) & guard=\$!; wait \$pid; rc=\$?; kill \$guard 2>/dev/null; wait \$guard 2>/dev/null; printf 'REMOTE_CELL_EXIT_CODE=%d\\n' \"\$rc\"; if test \"\$rc\" -gt 128; then printf 'REMOTE_CELL_SIGNAL=%d\\n' \"\$((rc-128))\"; else echo REMOTE_CELL_SIGNAL=0; fi; exit 0"
  printf '\nCELL_BEGIN arch=armv7l scenario=%s suite=%s carrier=direct_executable\nSDB_COMMAND=' "$scenario" "$suite"
  printf '%q ' /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"; printf '\n'
  /usr/bin/timeout --preserve-status 25s "$sdb" -s "$serial" shell "$command"
  carrier=$?; printf 'SDB_CARRIER_EXIT_CODE=%d\nCELL_END arch=armv7l scenario=%s suite=%s\n' "$carrier" "$scenario" "$suite"
  ((carrier == 0)) || return 90
}
identity="set +e; for s in unpatched layer_ab full; do echo LDD_SUITE=\$s; LD_LIBRARY_PATH='$remote/'\$s ldd '$remote/exe/istream_stale_exe'; echo LDD_EXIT_CODE=\$?; done; LD_DEBUG=libs LD_LIBRARY_PATH='$remote/full' '$remote/exe/failbit_stale_exe'; echo LD_DEBUG_EXIT_CODE=\$?; exit 0"
"$sdb" -s "$serial" shell "$identity" || exit 90
for item in cout:cout failbit: getline_member:getline_member read_member:read_member; do
  scenario=${item%%:*}; arg=${item#*:}; base=${scenario%%_*}; [[ $base == getline || $base == read ]] && base=istream
  run_cell "$scenario" unpatched "$remote/unpatched" "$remote/exe/${base}_stale_exe" "$arg" || exit $?
  run_cell "$scenario" layer_ab "$remote/layer_ab" "$remote/exe/${base}_stale_exe" "$arg" || exit $?
  run_cell "$scenario" library_batch_stale_consumer "$remote/full" "$remote/exe/${base}_stale_exe" "$arg" || exit $?
  run_cell "$scenario" library_batch_rebuilt_consumer "$remote/full" "$remote/exe/${base}_rebuilt_exe" "$arg" || exit $?
  run_cell "$scenario" libstdcxx_control /lib:/usr/lib "$remote/exe/${base}_libstdcxx_exe" "$arg" || exit $?
done
exit 0
