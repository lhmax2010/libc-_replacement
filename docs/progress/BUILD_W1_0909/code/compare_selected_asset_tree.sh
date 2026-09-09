#!/bin/bash
set -euo pipefail

if [ "$#" -ne 3 ]; then
    echo "usage: $0 ORIGINAL_TARBALL SELECTED_TREE OUTPUT_DIR" >&2
    exit 2
fi

original_tarball=$1
selected_tree=$2
output_dir=$3
mkdir -p "$output_dir/original"
tar -C "$output_dir/original" -xzf "$original_tarball"
(
    cd "$output_dir/original"
    find . -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum
) >"$output_dir/original.sha256"
(
    cd "$selected_tree"
    find . -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum
) >"$output_dir/selected.sha256"
cmp -s "$output_dir/original.sha256" "$output_dir/selected.sha256"
printf 'original_files=%s\n' "$(wc -l <"$output_dir/original.sha256")"
printf 'selected_files=%s\n' "$(wc -l <"$output_dir/selected.sha256")"
printf 'ASSET_TREE_IDENTICAL\n'
