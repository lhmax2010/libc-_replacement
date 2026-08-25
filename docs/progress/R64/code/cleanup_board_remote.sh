#!/bin/sh
set +e

target_root=/var/tmp/r64_20260825
archive_original=/opt/usr/home/owner/share/tmp/r64-original.tar
archive_deferred=/opt/usr/home/owner/share/tmp/r64-deferred.tar
self=$$
parent=$PPID

scan_processes() {
  ps -ef | awk -v self="$self" -v parent="$parent" '
    $2 != self && $2 != parent &&
    $0 ~ /r64_20260825|m2_deferred_(fixed|reference)|m2_(fixed|reference)/ &&
    $0 !~ /awk/ {print}'
}

echo BEFORE_PROCESSES
scan_processes
for pattern in "$target_root" m2_deferred_fixed m2_deferred_reference; do
  pids=$(ps -ef | awk -v self="$self" -v parent="$parent" -v pattern="$pattern" \
    '$2 != self && $2 != parent && index($0,pattern) && $0 !~ /awk/ {print $2}')
  if [ -n "$pids" ]; then
    echo "TERMINATING_PATTERN=$pattern PIDS=$pids"
    kill -TERM $pids 2>/dev/null
  fi
done
sleep 1
for pattern in "$target_root" m2_deferred_fixed m2_deferred_reference; do
  pids=$(ps -ef | awk -v self="$self" -v parent="$parent" -v pattern="$pattern" \
    '$2 != self && $2 != parent && index($0,pattern) && $0 !~ /awk/ {print $2}')
  if [ -n "$pids" ]; then
    echo "KILLING_PATTERN=$pattern PIDS=$pids"
    kill -KILL $pids 2>/dev/null
  fi
done

rm -rf "$target_root"
root_rc=$?
rm -f "$archive_original"
original_rc=$?
rm -f "$archive_deferred"
deferred_rc=$?

echo "ROOT_REMOVE_EXIT=$root_rc"
echo "ORIGINAL_ARCHIVE_REMOVE_EXIT=$original_rc"
echo "DEFERRED_ARCHIVE_REMOVE_EXIT=$deferred_rc"
for path in "$target_root" "$archive_original" "$archive_deferred"; do
  if [ -e "$path" ]; then echo "PRESENT=$path"; else echo "ABSENT=$path"; fi
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
[ "$root_rc" -eq 0 ] && [ "$original_rc" -eq 0 ] && [ "$deferred_rc" -eq 0 ] && [ "$process_rc" -eq 0 ]
