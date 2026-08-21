#!/bin/sh
set +e

target_root=/var/tmp/w2_r62_20260821
target_archive=/opt/usr/home/owner/share/tmp/w2-r62-payload.tar
self=$$

scan_processes() {
  ps -ef | awk -v self="$self" '
    $2 != self &&
    $0 ~ /w2_r62|r61_fixed|r61_reference|m2_fixed|m2_reference|m1_concurrency|facility_(fixed|reference)|async_(fixed|reference)|noncancel_(fixed|reference)|normal_(fixed|reference)/ &&
    $0 !~ /awk/ {print}'
}

echo BEFORE_PROCESSES
scan_processes

for pattern in "$target_root" r61_fixed r61_reference m2_fixed m2_reference \
  m1_concurrency_fixed m1_concurrency_reference facility_fixed facility_reference \
  async_fixed async_reference noncancel_fixed noncancel_reference normal_fixed normal_reference; do
  pids=$(ps -ef | awk -v self="$self" -v p="$pattern" \
    '$2 != self && index($0,p) && $0 !~ /awk/ {print $2}')
  if [ -n "$pids" ]; then
    echo "TERMINATING_PATTERN=$pattern PIDS=$pids"
    kill -TERM $pids 2>/dev/null
  fi
done
sleep 1

for pattern in "$target_root" r61_fixed r61_reference m2_fixed m2_reference \
  m1_concurrency_fixed m1_concurrency_reference facility_fixed facility_reference \
  async_fixed async_reference noncancel_fixed noncancel_reference normal_fixed normal_reference; do
  pids=$(ps -ef | awk -v self="$self" -v p="$pattern" \
    '$2 != self && index($0,p) && $0 !~ /awk/ {print $2}')
  if [ -n "$pids" ]; then
    echo "KILLING_PATTERN=$pattern PIDS=$pids"
    kill -KILL $pids 2>/dev/null
  fi
done

rm -rf "$target_root"
root_rc=$?
rm -f "$target_archive"
archive_rc=$?

echo "REMOTE_REMOVE_EXIT=$root_rc"
echo "ARCHIVE_REMOVE_EXIT=$archive_rc"
for path in "$target_root" "$target_archive"; do
  if [ -e "$path" ]; then
    echo "PRESENT=$path"
  else
    echo "ABSENT=$path"
  fi
done

echo AFTER_PROCESSES
remaining=$(scan_processes)
if [ -n "$remaining" ]; then
  printf '%s\n' "$remaining"
  process_rc=1
else
  process_rc=0
fi
echo "AFTER_PROCESS_SCAN_EXIT=$process_rc"

[ "$root_rc" -eq 0 ] && [ "$archive_rc" -eq 0 ] && [ "$process_rc" -eq 0 ]
