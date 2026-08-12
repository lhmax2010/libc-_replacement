#!/usr/bin/env bash
set -euo pipefail
before=$(realpath "${1:?before RPM directory}")
after=$(realpath "${2:?after RPM directory}")
output=$(realpath -m "${3:?output TSV}")
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
printf 'rpm\tpath\tsection\tbaseline_sha\tcandidate_sha\tstatus\n' >"$output"
for oldrpm in "$before"/*.rpm; do
    name=$(basename "$oldrpm")
    [[ "$name" == *-debuginfo-* || "$name" == *-debugsource-* ]] && continue
    newrpm="$after/$name"
    [[ -f "$newrpm" ]] || continue
    rm -rf "$work/old" "$work/new"; mkdir -p "$work/old" "$work/new"
    (cd "$work/old" && rpm2cpio "$oldrpm" | cpio -idmu --quiet)
    (cd "$work/new" && rpm2cpio "$newrpm" | cpio -idmu --quiet)
    while IFS= read -r -d '' oldfile; do
        rel=${oldfile#"$work/old/"}; newfile="$work/new/$rel"
        [[ -f "$newfile" ]] || continue
        file "$oldfile" | grep -q 'ELF' || continue
        for section in .text .rodata .data; do
            objcopy -O binary --only-section="$section" "$oldfile" "$work/a.bin" 2>/dev/null || : >"$work/a.bin"
            objcopy -O binary --only-section="$section" "$newfile" "$work/b.bin" 2>/dev/null || : >"$work/b.bin"
            a=$(sha256sum "$work/a.bin" | cut -d' ' -f1); b=$(sha256sum "$work/b.bin" | cut -d' ' -f1)
            status=DIFF; [[ "$a" == "$b" ]] && status=MATCH
            printf '%s\t/%s\t%s\t%s\t%s\t%s\n' "$name" "$rel" "$section" "$a" "$b" "$status" >>"$output"
        done
    done < <(find "$work/old" -type f -print0)
done
