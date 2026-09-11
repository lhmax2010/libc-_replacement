#!/bin/bash
set -euo pipefail

if [ "$#" -ne 4 ]; then
    echo "usage: $0 CANDIDATE_RPM_DIR BASELINE_RPM_DIR ARCH OUTPUT_DIR" >&2
    exit 2
fi

candidate_dir=$1
baseline_dir=$2
arch=$3
output_dir=$4
mkdir -p "$output_dir/candidate" "$output_dir/baseline"

collect_set() {
    local side=$1
    local rpm_dir=$2
    local side_dir="$output_dir/$side"
    local work
    work=$(mktemp -d)
    trap 'rm -rf "$work"' RETURN

    : >"$side_dir/packages.tsv"
    : >"$side_dir/file_lists.tsv"
    : >"$side_dir/stdlib_requires.tsv"
    : >"$side_dir/elf_needed.tsv"
    : >"$side_dir/dynamic_symbols.tsv"

    while IFS= read -r rpm_path; do
        local package_key
        rpm_path=$(readlink -f "$rpm_path")
        package_key=$(rpm -qp --qf '%{NAME}.%{ARCH}' "$rpm_path")
        printf '%s\t%s\n' "$package_key" "$(basename "$rpm_path")" \
            >>"$side_dir/packages.tsv"
        rpm -qlp "$rpm_path" | LC_ALL=C sort | while IFS= read -r item; do
            printf '%s\t%s\n' "$package_key" "$item"
        done >>"$side_dir/file_lists.tsv"
        rpm -qp --requires "$rpm_path" \
            | grep -E '(^|[^[:alnum:]_])(libstdc\+\+|libc\+\+|libc\+\+abi)' \
            | LC_ALL=C sort | while IFS= read -r item; do
                printf '%s\t%s\n' "$package_key" "$item"
            done >>"$side_dir/stdlib_requires.tsv" || true

        rm -rf "$work/content"
        mkdir -p "$work/content"
        (
            cd "$work/content"
            rpm2cpio "$rpm_path" | cpio -idm --quiet
        )
        while IFS= read -r elf_path; do
            local relative
            relative=${elf_path#"$work/content"/}
            readelf -d "$elf_path" 2>/dev/null \
                | sed -n 's/.*Shared library: \[\([^]]*\)\].*/\1/p' \
                | LC_ALL=C sort -u | while IFS= read -r needed; do
                    printf '%s\t%s\t%s\n' "$package_key" "$relative" "$needed"
                done >>"$side_dir/elf_needed.tsv"
            readelf --dyn-syms --wide "$elf_path" 2>/dev/null \
                | awk -v pkg="$package_key" -v path="$relative" \
                    'NF >= 8 && $1 ~ /^[0-9]+:$/ {print pkg "\t" path "\t" $4 "\t" $5 "\t" $6 "\t" $7 "\t" $8}' \
                >>"$side_dir/dynamic_symbols.tsv" || true
        done < <(find "$work/content" -type f -exec sh -c \
            'readelf -h "$1" >/dev/null 2>&1 && printf "%s\n" "$1"' sh {} \;)
    done < <(find "$rpm_dir" -maxdepth 1 -type f -name "*.$arch.rpm" -print | LC_ALL=C sort)

    LC_ALL=C sort -u -o "$side_dir/packages.tsv" "$side_dir/packages.tsv"
    LC_ALL=C sort -u -o "$side_dir/file_lists.tsv" "$side_dir/file_lists.tsv"
    LC_ALL=C sort -u -o "$side_dir/stdlib_requires.tsv" "$side_dir/stdlib_requires.tsv"
    LC_ALL=C sort -u -o "$side_dir/elf_needed.tsv" "$side_dir/elf_needed.tsv"
    LC_ALL=C sort -u -o "$side_dir/dynamic_symbols.tsv" "$side_dir/dynamic_symbols.tsv"
    rm -rf "$work"
    trap - RETURN
}

collect_set candidate "$candidate_dir"
collect_set baseline "$baseline_dir"

result=0
for item in packages file_lists stdlib_requires elf_needed dynamic_symbols; do
    diff -u "$output_dir/baseline/$item.tsv" "$output_dir/candidate/$item.tsv" \
        >"$output_dir/$item.diff" || result=1
    printf '%s_diff_lines=%s\n' "$item" "$(wc -l <"$output_dir/$item.diff")"
done

if [ "$result" -ne 0 ]; then
    echo "SAME_ENVIRONMENT_RPM_COMPARE=DIFFERENT"
    exit 20
fi
echo "SAME_ENVIRONMENT_RPM_COMPARE=PASS"
