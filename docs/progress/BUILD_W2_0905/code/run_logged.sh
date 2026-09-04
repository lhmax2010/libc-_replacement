#!/usr/bin/env bash
set -u

if [ "$#" -lt 3 ]; then
  echo "usage: $0 OUTPUT_DIR SEQUENCE COMMAND [ARG ...]" >&2
  exit 2
fi

output_dir=$1
sequence=$2
shift 2

mkdir -p "$output_dir"
command_file="$output_dir/${sequence}.command.txt"
stdout_file="$output_dir/${sequence}.stdout.txt"
stderr_file="$output_dir/${sequence}.stderr.txt"
exitcode_file="$output_dir/${sequence}.exitcode"

{
  first=1
  for argument in "$@"; do
    if [ "$first" -eq 0 ]; then
      printf ' '
    fi
    printf '%q' "$argument"
    first=0
  done
  printf '\n'
} >"$command_file"

"$@" >"$stdout_file" 2>"$stderr_file"
rc=$?
printf '%s\n' "$rc" >"$exitcode_file"
exit "$rc"
