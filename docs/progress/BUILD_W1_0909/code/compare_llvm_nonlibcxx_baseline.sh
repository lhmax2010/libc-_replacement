#!/bin/bash
set -euo pipefail

if [ "$#" -ne 3 ]; then
    echo "usage: $0 BASELINE_LIBLLVM_RPM CANDIDATE_LIBLLVM_RPM OUTPUT_DIR" >&2
    exit 2
fi

baseline_rpm=$1
candidate_rpm=$2
output_dir=$3
mkdir -p "$output_dir/baseline" "$output_dir/candidate"

sha256sum "$baseline_rpm" "$candidate_rpm" >"$output_dir/rpm_sha256.txt"
rpm -qlp "$baseline_rpm" | LC_ALL=C sort >"$output_dir/baseline_file_list.txt"
rpm -qlp "$candidate_rpm" | LC_ALL=C sort >"$output_dir/candidate_file_list.txt"
diff -u "$output_dir/baseline_file_list.txt" "$output_dir/candidate_file_list.txt" \
    >"$output_dir/file_list.diff" || true

rpm -qp --requires "$baseline_rpm" | LC_ALL=C sort >"$output_dir/baseline_requires.txt"
rpm -qp --requires "$candidate_rpm" | LC_ALL=C sort >"$output_dir/candidate_requires.txt"
diff -u "$output_dir/baseline_requires.txt" "$output_dir/candidate_requires.txt" \
    >"$output_dir/requires.diff" || true

for side in baseline candidate; do
    if [ "$side" = baseline ]; then
        rpm_path=$baseline_rpm
    else
        rpm_path=$candidate_rpm
    fi
    (
        cd "$output_dir/$side"
        rpm2cpio "$OLDPWD/$rpm_path" | cpio -idm --quiet
    )
    llvm_so=$(find "$output_dir/$side" -type f -name 'libLLVM.so.22.1' -print -quit)
    test -n "$llvm_so"
    readelf -d "$llvm_so" >"$output_dir/${side}_dynamic.txt"
    readelf --dyn-syms --wide "$llvm_so" \
        | awk 'NF >= 8 && $7 != "UND" {print $8}' \
        | LC_ALL=C sort -u >"$output_dir/${side}_defined_dynsymbols.txt"
    nm -D -C "$llvm_so" \
        | grep -E '_xla_|tensorflow::XlaCompiledCpuFunction|InlinerSizeModel' \
        | sed -E 's/^[[:xdigit:]]+[[:space:]]+[[:alpha:]][[:space:]]+//' \
        | LC_ALL=C sort -u >"$output_dir/${side}_mlgo_interface.txt" || true
done

diff -u "$output_dir/baseline_defined_dynsymbols.txt" \
    "$output_dir/candidate_defined_dynsymbols.txt" \
    >"$output_dir/defined_dynsymbols.diff" || true
diff -u "$output_dir/baseline_mlgo_interface.txt" \
    "$output_dir/candidate_mlgo_interface.txt" \
    >"$output_dir/mlgo_interface.diff" || true

printf 'file_list_diff_lines=%s\n' "$(wc -l <"$output_dir/file_list.diff")"
printf 'requires_diff_lines=%s\n' "$(wc -l <"$output_dir/requires.diff")"
printf 'defined_dynsymbols_diff_lines=%s\n' "$(wc -l <"$output_dir/defined_dynsymbols.diff")"
printf 'mlgo_interface_diff_lines=%s\n' "$(wc -l <"$output_dir/mlgo_interface.diff")"
