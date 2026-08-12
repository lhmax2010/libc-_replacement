#!/usr/bin/env bash
set -euo pipefail

before=$(realpath "${1:?before RPM directory}")
after=$(realpath "${2:?after RPM directory}")
output=$(realpath -m "${3:?output TSV}")
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

printf 'rpm\tpath\ttype\tbaseline_sha_or_target\tcandidate_sha_or_target\tstatus\n' >"$output"
for oldrpm in "$before"/*.rpm; do
    name=$(basename "$oldrpm")
    newrpm="$after/$name"
    if [[ ! -f "$newrpm" ]]; then
        printf '%s\t-\tRPM\tPRESENT\tMISSING\tMISSING_CANDIDATE_RPM\n' "$name" >>"$output"
        continue
    fi
    rm -rf "$work/old" "$work/new"
    mkdir -p "$work/old" "$work/new"
    (cd "$work/old" && rpm2cpio "$oldrpm" | cpio -idmu --quiet)
    (cd "$work/new" && rpm2cpio "$newrpm" | cpio -idmu --quiet)
    (
        cd "$work/old"
        find . -type f -print0 | sort -z | xargs -0 -r sha256sum
        find . -type l -printf 'LINK %p %l\n'
    ) | sort -k2,2 >"$work/old.tsv"
    (
        cd "$work/new"
        find . -type f -print0 | sort -z | xargs -0 -r sha256sum
        find . -type l -printf 'LINK %p %l\n'
    ) | sort -k2,2 >"$work/new.tsv"
    awk -v rpm="$name" '
        FNR==NR { old[$2]=$1; paths[$2]=1; next }
        { new[$2]=$1; paths[$2]=1 }
        END {
            for (p in paths) {
                o=(p in old)?old[p]:"MISSING";
                n=(p in new)?new[p]:"MISSING";
                s=(o==n)?"MATCH":"DIFF";
                print rpm "\t" p "\tPAYLOAD\t" o "\t" n "\t" s;
            }
        }
    ' "$work/old.tsv" "$work/new.tsv" | sort -t $'\t' -k2,2 >>"$output"
done
