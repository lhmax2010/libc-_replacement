#!/bin/bash
set -euo pipefail

if [ "$#" -ne 3 ]; then
    echo "usage: $0 ARCH GBS_ROOT OUTPUT_DIR" >&2
    exit 64
fi

arch=$1
gbs_root=$2
out=$3
repo="$gbs_root/local/repos/tizen_unified_standard/$arch"

mkdir -p "$out/artifacts" "$out/extracted" "$out/evidence"

find "$repo/RPMS" -maxdepth 1 -type f -name '*21.1.1-2.*.rpm' \
    -exec cp -a {} "$out/artifacts/" \;
find "$repo/SRPMS" -maxdepth 1 -type f -name '*21.1.1-2.src.rpm' \
    -exec cp -a {} "$out/artifacts/" \;

{
    printf 'filename\tname\tepoch_version_release\tarch\tpayload_size_bytes\tsha256\n'
    find "$out/artifacts" -maxdepth 1 -type f -name '*.rpm' -print0 |
        sort -z |
        while IFS= read -r -d '' rpm_file; do
            meta=$(rpm -qp --qf '%{NAME}\t%{EPOCHNUM}:%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SIZE}' "$rpm_file")
            digest=$(sha256sum "$rpm_file" | awk '{print $1}')
            printf '%s\t%s\t%s\n' "$(basename "$rpm_file")" "$meta" "$digest"
        done
} > "$out/rpm_manifest.tsv"

find "$out/artifacts" -maxdepth 1 -type f \
    \( -name 'libc++-21.1.1-2.*.rpm' \
       -o -name 'libc++-devel-21.1.1-2.*.rpm' \
       -o -name 'libc++abi-21.1.1-2.*.rpm' \
       -o -name 'libc++abi-devel-21.1.1-2.*.rpm' \) \
    -print0 |
    sort -z |
    while IFS= read -r -d '' rpm_file; do
        (cd "$out/extracted" && rpm2cpio "$rpm_file" | cpio -idmuv)
    done > "$out/evidence/rpm_extract.log" 2>&1

mapfile -t libraries < <(
    find "$out/extracted" -type f \
        \( -name 'libc++.so.1.0' -o -name 'libc++abi.so.1.0' \) |
        sort
)

if [ "${#libraries[@]}" -ne 2 ]; then
    printf 'expected 2 runtime DSOs, found %s\n' "${#libraries[@]}" >&2
    exit 2
fi

printf 'library\tdefined_exports\tdefault_LLVM_22\tsingle_at_LLVM_22\tunversioned_or_other\tcoverage_percent\tverdict\n' \
    > "$out/versioning_gate.tsv"
printf 'library\tdynsym_line\n' > "$out/unversioned_exports.tsv"
printf 'library\tdynsym_line\n' > "$out/single_at_exports.tsv"
printf 'library\tneeded_soname\n' > "$out/direct_needed.tsv"

for library in "${libraries[@]}"; do
    base=$(basename "$library")
    stem=${base//+/x}
    readelf --dyn-syms -W "$library" > "$out/evidence/${stem}_dynsyms.log"
    readelf --version-info -W "$library" > "$out/evidence/${stem}_version_info.log"
    readelf -d -W "$library" > "$out/evidence/${stem}_dynamic.log"
    readelf -h -W "$library" > "$out/evidence/${stem}_elf_header.log"

    awk '$5 ~ /^(GLOBAL|WEAK)$/ && $6 ~ /^(DEFAULT|PROTECTED)$/ &&
         $7 != "UND" && $7 != "ABS" {print}' \
        "$out/evidence/${stem}_dynsyms.log" \
        > "$out/evidence/${stem}_defined_exports.log"

    total=$(wc -l < "$out/evidence/${stem}_defined_exports.log")
    default=$(awk '$8 ~ /@@LLVM_22$/ {n++} END {print n+0}' \
        "$out/evidence/${stem}_defined_exports.log")
    single=$(awk '$8 ~ /@LLVM_22$/ && $8 !~ /@@LLVM_22$/ {n++} END {print n+0}' \
        "$out/evidence/${stem}_defined_exports.log")
    other=$((total - default - single))
    coverage=$(awk -v n="$default" -v d="$total" \
        'BEGIN {if (d == 0) print "0.000"; else printf "%.3f", 100*n/d}')
    verdict=PASS
    if [ "$single" -ne 0 ] || [ "$other" -ne 0 ] || [ "$total" -eq 0 ]; then
        verdict=FAIL
    fi
    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
        "$base" "$total" "$default" "$single" "$other" "$coverage" "$verdict" \
        >> "$out/versioning_gate.tsv"

    awk -v lib="$base" \
        '$5 ~ /^(GLOBAL|WEAK)$/ && $6 ~ /^(DEFAULT|PROTECTED)$/ &&
         $7 != "UND" && $7 != "ABS" && $8 !~ /@LLVM_22$/ {
             print lib "\t" $0
         }' "$out/evidence/${stem}_dynsyms.log" \
        >> "$out/unversioned_exports.tsv"
    awk -v lib="$base" \
        '$5 ~ /^(GLOBAL|WEAK)$/ && $6 ~ /^(DEFAULT|PROTECTED)$/ &&
         $7 != "UND" && $7 != "ABS" &&
         $8 ~ /@LLVM_22$/ && $8 !~ /@@LLVM_22$/ {
             print lib "\t" $0
         }' "$out/evidence/${stem}_dynsyms.log" \
        >> "$out/single_at_exports.tsv"
    awk -v lib="$base" '/\(NEEDED\)/ {
        soname=$NF
        gsub(/^\[/, "", soname)
        gsub(/\]$/, "", soname)
        print lib "\t" soname
    }' "$out/evidence/${stem}_dynamic.log" >> "$out/direct_needed.tsv"
done

{
    printf 'arch\tmachine\n'
    for library in "${libraries[@]}"; do
        machine=$(readelf -h -W "$library" | awk -F: '/Machine:/ {
            sub(/^[[:space:]]+/, "", $2)
            print $2
        }')
        printf '%s\t%s\n' "$arch" "$machine"
    done
} > "$out/elf_identity.tsv"
