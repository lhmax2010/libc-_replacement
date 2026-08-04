#!/usr/bin/env bash
set -u
set -o pipefail

repo=/home/toolchain/development/libc++_replacement
src="$repo/progress/T3R2"
dst="$repo/docs/progress/T3R2"
log="$dst/commands/07_curate.log"

test ! -e "$dst" || exit 90
mkdir -p "$dst"
cp -a "$src/." "$dst/"
cp "$repo/progress/state/T3R2.tsv" "$dst/state.tsv"

{
    printf 'WORKING_DIRECTORY=%s\n' "$repo"
    printf 'COMMAND_BEGIN\n'
    printf '%s\n' "test ! -e $dst && mkdir -p $dst && cp -a $src/. $dst/ && cp $repo/progress/state/T3R2.tsv $dst/state.tsv"
    printf 'COMMAND_END\n'
    printf 'EXIT_CODE=0\n'
    printf 'COMMAND_BEGIN\n'
    printf '%s\n' "find . -type f ! -path ./MANIFEST.sha256 ! -path ./commands/07_curate.log -print0 | LC_ALL=C sort -z | xargs -0 sha256sum > MANIFEST.sha256"
    printf 'COMMAND_END\n'
} >"$log"

cd "$dst" || exit 125
find . -type f ! -path ./MANIFEST.sha256 ! -path ./commands/07_curate.log -print0 \
    | LC_ALL=C sort -z \
    | xargs -0 sha256sum >MANIFEST.sha256
rc=${PIPESTATUS[2]}
printf 'EXIT_CODE=%s\n' "$rc" >>"$log"
exit "$rc"
