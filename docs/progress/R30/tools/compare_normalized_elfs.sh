#!/usr/bin/env bash
set -euo pipefail

before=$(realpath "${1:?before RPM directory}")
after=$(realpath "${2:?after RPM directory}")
output=$(realpath -m "${3:?output TSV}")
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
printf 'rpm\tpath\tbaseline_normalized_sha\tcandidate_normalized_sha\tstatus\n' >"$output"

for oldrpm in "$before"/*.rpm; do
    name=$(basename "$oldrpm")
    newrpm="$after/$name"
    [[ -f "$newrpm" ]] || continue
    rm -rf "$work/old" "$work/new"
    mkdir -p "$work/old" "$work/new"
    (cd "$work/old" && rpm2cpio "$oldrpm" | cpio -idmu --quiet)
    (cd "$work/new" && rpm2cpio "$newrpm" | cpio -idmu --quiet)
    while IFS= read -r -d '' oldfile; do
        rel=${oldfile#"$work/old/"}
        newfile="$work/new/$rel"
        [[ -f "$newfile" ]] || continue
        file "$oldfile" | grep -q 'ELF' || continue
        cp "$oldfile" "$work/a.elf"
        cp "$newfile" "$work/b.elf"
        objcopy --strip-debug --remove-section=.note.gnu.build-id \
            --remove-section=.comment "$work/a.elf" 2>/dev/null || true
        objcopy --strip-debug --remove-section=.note.gnu.build-id \
            --remove-section=.comment "$work/b.elf" 2>/dev/null || true
        a=$(sha256sum "$work/a.elf" | cut -d' ' -f1)
        b=$(sha256sum "$work/b.elf" | cut -d' ' -f1)
        status=DIFF
        [[ "$a" == "$b" ]] && status=MATCH
        printf '%s\t/%s\t%s\t%s\t%s\n' "$name" "$rel" "$a" "$b" "$status" >>"$output"
    done < <(find "$work/old" -type f -print0)
done
