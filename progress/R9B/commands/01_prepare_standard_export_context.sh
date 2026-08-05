#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
LLVM="$REPO/codes/llvm"
WORKTREE="$REPO/tmp/R9B/gbs-source-std"
REL_PACKAGING=r9b-packaging
INPUT="$REPO/packaging"
OUT="$REPO/progress/R9B"
RAW="$OUT/raw"
LEDGER="$OUT/commands/01_prepare_standard_export_context.log"
HEAD_SHA=3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b

: >"$LEDGER"
record_to() {
    local label=$1 outfile=$2
    shift 2
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    return "$rc"
}

cd "$REPO" || exit 125
overall=0
record_to provenance_diff "$RAW/provenance_diff.stdout.txt" git diff -- packaging/SOURCE_PROVENANCE || overall=1
record_to packaging_input_sha256_after "$RAW/packaging_input_sha256_after.stdout.txt" sha256sum \
    "$INPUT/libcxx-runtimes.spec" "$INPUT/libcxx-llvm22.map" \
    "$INPUT/libcxx-runtimes.manifest" "$INPUT/SOURCE_PROVENANCE" || overall=1
record_to assert_nonprovenance_inputs_unchanged "$RAW/assert_nonprovenance_inputs_unchanged.stdout.txt" bash -o pipefail -c \
    'cd "$1" && printf "%s  libcxx-runtimes.spec\\n%s  libcxx-llvm22.map\\n%s  libcxx-runtimes.manifest\\n" "$2" "$3" "$4" | sha256sum -c -' \
    _ "$INPUT" \
    a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74 \
    e806ab237e86bbe1e1d25c9bedbb129016ed67e9e72acb9259154b4b51a1304d \
    db262cd0f01ca668a019b79bf1bb1a9731a100205ff4d4b256d071b3d5304dc3 || overall=1
record_to assert_provenance_fields "$RAW/assert_provenance_fields.stdout.txt" bash -o pipefail -c \
    'rg -qx "source_tarball_sha256=16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e" "$1" && rg -qx "local_tizen_base_vcs=platform/upstream/llvm#3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b" "$1" && rg -qx "upstream_peeled_commit=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1" "$1" && ! rg -q "^platform_spec_vcs=" "$1" && rg -q "^superseded_platform_spec_vcs=.*non-authoritative" "$1"' \
    _ "$INPUT/SOURCE_PROVENANCE" || overall=1
record_to provenance_numbered "$RAW/provenance_numbered.stdout.txt" nl -ba "$INPUT/SOURCE_PROVENANCE" || overall=1

record_to assert_source_still_clean "$RAW/assert_source_still_clean.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ "$LLVM" "$HEAD_SHA" || overall=1
record_to assert_worktree_fresh "$RAW/assert_worktree_fresh.stdout.txt" test ! -e "$WORKTREE" || overall=1
if [[ $overall -eq 0 ]]; then
    record_to add_detached_worktree "$RAW/add_detached_worktree.stdout.txt" git -C "$LLVM" worktree add --detach "$WORKTREE" "$HEAD_SHA" || overall=1
    record_to create_relative_packaging "$RAW/create_relative_packaging.stdout.txt" mkdir "$WORKTREE/$REL_PACKAGING" || overall=1
    record_to copy_spec "$RAW/copy_spec.stdout.txt" cp -a "$INPUT/libcxx-runtimes.spec" "$WORKTREE/$REL_PACKAGING/" || overall=1
    record_to copy_map "$RAW/copy_map.stdout.txt" cp -a "$INPUT/libcxx-llvm22.map" "$WORKTREE/$REL_PACKAGING/" || overall=1
    record_to copy_manifest "$RAW/copy_manifest.stdout.txt" cp -a "$INPUT/libcxx-runtimes.manifest" "$WORKTREE/$REL_PACKAGING/" || overall=1
    record_to copy_provenance "$RAW/copy_provenance.stdout.txt" cp -a "$INPUT/SOURCE_PROVENANCE" "$WORKTREE/$REL_PACKAGING/" || overall=1
fi

record_to worktree_identity "$RAW/worktree_identity.stdout.txt" git -C "$WORKTREE" rev-parse HEAD || overall=1
record_to worktree_status "$RAW/worktree_status.stdout.txt" git -C "$WORKTREE" status --porcelain=v1 || overall=1
record_to assert_worktree_shape "$RAW/assert_worktree_shape.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = "$2" && test "$(git -C "$1" status --porcelain=v1)" = "?? r9b-packaging/" && test -z "$(git -C "$1" diff --name-only)" && test "$(find "$1/r9b-packaging" -maxdepth 1 -type f -name "*.spec" | wc -l)" -eq 1 && rg -q "^Name:[[:space:]]+libcxx-runtimes$" "$1/r9b-packaging/libcxx-runtimes.spec"' \
    _ "$WORKTREE" "$HEAD_SHA" || overall=1
record_to copied_input_sha256 "$RAW/copied_input_sha256.stdout.txt" sha256sum "$WORKTREE/$REL_PACKAGING"/* || overall=1
record_to assert_copies_identical "$RAW/assert_copies_identical.stdout.txt" bash -o pipefail -c \
    'cmp -s "$1/libcxx-runtimes.spec" "$2/libcxx-runtimes.spec" && cmp -s "$1/libcxx-llvm22.map" "$2/libcxx-llvm22.map" && cmp -s "$1/libcxx-runtimes.manifest" "$2/libcxx-runtimes.manifest" && cmp -s "$1/SOURCE_PROVENANCE" "$2/SOURCE_PROVENANCE"' \
    _ "$INPUT" "$WORKTREE/$REL_PACKAGING" || overall=1
record_to worktree_list "$RAW/worktree_list.stdout.txt" git -C "$LLVM" worktree list --porcelain || overall=1

{
    printf 'field\tvalue\n'
    printf 'source_repo\t%s\n' "$LLVM"
    printf 'source_branch_authoritative\ttizen_base\n'
    printf 'source_commit\t%s\n' "$HEAD_SHA"
    printf 'build_worktree\t%s\n' "$WORKTREE"
    printf 'packaging_dir\t%s\n' "$REL_PACKAGING"
    printf 'spec_sha256\t%s\n' "$(sha256sum "$INPUT/libcxx-runtimes.spec" | cut -d' ' -f1)"
    printf 'map_sha256\t%s\n' "$(sha256sum "$INPUT/libcxx-llvm22.map" | cut -d' ' -f1)"
    printf 'manifest_sha256\t%s\n' "$(sha256sum "$INPUT/libcxx-runtimes.manifest" | cut -d' ' -f1)"
    printf 'provenance_sha256\t%s\n' "$(sha256sum "$INPUT/SOURCE_PROVENANCE" | cut -d' ' -f1)"
    printf 'gbs_conf\t%s\n' "$REPO/progress/T3R/config/gbs_llvm.conf"
    printf 'gbs_conf_sha256\t%s\n' "$(sha256sum "$REPO/progress/T3R/config/gbs_llvm.conf" | cut -d' ' -f1)"
    printf 'export_mode\tGBS_STANDARD_NON_NATIVE_PATCH_EXPORT\n'
    printf 'forbidden_option\t--no-patch-export ABSENT\n'
} >"$OUT/build_input_identity.tsv"

{
    printf 'dimension\tT3R_T3R2\tR6\tR9B\n'
    printf 'source\tpreexported_platform_tarball\tgit_worktree_HEAD\tgit_worktree_HEAD_3d3e3da\n'
    printf 'style\tobs\tgit\tgit\n'
    printf 'export\tnone_preexported\t--no-patch-export\tstandard_patch_export_no_no-patch-option\n'
    printf 'packaging\tobs_input/packaging\tr6-packaging\tr9b-packaging\n'
    printf 'Source0\tplatform_16a932e1\twhole_tree_7d7f2105\tupstream_tag_expected_16a932e1\n'
} >"$OUT/invocation_delta.tsv"

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
