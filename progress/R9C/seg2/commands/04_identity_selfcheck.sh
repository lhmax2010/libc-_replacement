#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C/seg2"
ART="$REPO/artifacts/rebuild_2218_c2"
OLD="$REPO/artifacts/rebuild_2218"
TMP="$REPO/tmp/R9C/seg2/srpm_identity"
LEDGER="$OUT/commands/04_identity_selfcheck.log"
EXPECTED_SOURCE0=1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
ARCHES=(armv7l aarch64 x86_64)

: >"$LEDGER"
record() {
    local label=$1
    shift
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >>"$LEDGER" 2>&1
    local rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    return "$rc"
}

fail=0
record assert_fresh_extraction_root test ! -e "$TMP" || exit 90
record create_extraction_root mkdir -p "$TMP" || exit 91

printf 'architecture\tsrpm_file\tsrpm_sha256\tsource0_sha256\tprovenance_sha256\n' >"$OUT/identity/srpm_source_identity.tsv"
printf 'architecture\tpatch_file\tsha256\tpatch_id\n' >"$OUT/identity/srpm_patch_inventory.tsv"

for arch in "${ARCHES[@]}"; do
    srpm=$(find "$ART/$arch" -maxdepth 1 -type f -name '*.src.rpm' -print -quit)
    if [[ -z ${srpm:-} ]]; then
        printf 'missing SRPM for %s\n' "$arch" >>"$LEDGER"
        fail=1
        continue
    fi
    mkdir -p "$TMP/$arch"
    {
        printf 'LABEL=extract_srpm_%s\nPWD=%s\nCOMMAND=' "$arch" "$TMP/$arch"
        printf '%q ' rpm2cpio "$srpm"
        printf '| '
        printf '%q ' cpio -idm --quiet
        printf '\n'
    } >>"$LEDGER"
    (cd "$TMP/$arch" && rpm2cpio "$srpm" | cpio -idm --quiet) >>"$LEDGER" 2>&1
    rc=$?
    printf 'EXIT_CODE=%s\n\n' "$rc" >>"$LEDGER"
    [[ $rc -eq 0 ]] || { fail=1; continue; }

    source0="$TMP/$arch/llvm-22.1.8.tar.gz"
    provenance="$TMP/$arch/SOURCE_PROVENANCE"
    source0_sha=$(sha256sum "$source0" | cut -d' ' -f1)
    prov_sha=$(sha256sum "$provenance" | cut -d' ' -f1)
    srpm_sha=$(sha256sum "$srpm" | cut -d' ' -f1)
    printf '%s\t%s\t%s\t%s\t%s\n' "$arch" "$(basename "$srpm")" "$srpm_sha" "$source0_sha" "$prov_sha" >>"$OUT/identity/srpm_source_identity.tsv"

    for patch in "$TMP/$arch"/0*.patch; do
        printf '%s\t%s\t%s\t%s\n' "$arch" "$(basename "$patch")" \
            "$(sha256sum "$patch" | cut -d' ' -f1)" \
            "$(git patch-id --stable <"$patch" | cut -d' ' -f1)" >>"$OUT/identity/srpm_patch_inventory.tsv"
    done

    (cd "$TMP/$arch" && find . -mindepth 1 -type f -printf '%P\0' | LC_ALL=C sort -z | while IFS= read -r -d '' f; do
        printf '%s\t%s\t%s\n' "$(stat -c '%a' "$f")" "$(sha256sum "$f" | cut -d' ' -f1)" "$f"
    done) >"$OUT/identity/srpm_payload_manifest.$arch.tsv"

    record "assert_source0_${arch}" test "$source0_sha" = "$EXPECTED_SOURCE0" || fail=1
    record "assert_provenance_${arch}" cmp -s "$provenance" "$REPO/packaging/SOURCE_PROVENANCE" || fail=1
    record "assert_six_patches_${arch}" test "$(find "$TMP/$arch" -maxdepth 1 -type f -name '0*.patch' | wc -l)" -eq 6 || fail=1
done

record assert_source0_three_way bash -o pipefail -c \
    'test "$(tail -n +2 "$1" | cut -f4 | sort -u | wc -l)" -eq 1 && test "$(tail -n +2 "$1" | cut -f4 | sort -u)" = "$2"' _ \
    "$OUT/identity/srpm_source_identity.tsv" "$EXPECTED_SOURCE0" || fail=1

for arch in "${ARCHES[@]}"; do
    awk -F '\t' -v a="$arch" 'NR==1 {next} $1==a {print $2"\t"$3"\t"$4}' "$OUT/identity/srpm_patch_inventory.tsv" >"$OUT/identity/srpm_patch_inventory.$arch.tsv"
done
record compare_patch_arm_aarch cmp -s "$OUT/identity/srpm_patch_inventory.armv7l.tsv" "$OUT/identity/srpm_patch_inventory.aarch64.tsv" || fail=1
record compare_patch_arm_x86 cmp -s "$OUT/identity/srpm_patch_inventory.armv7l.tsv" "$OUT/identity/srpm_patch_inventory.x86_64.tsv" || fail=1
record compare_patch_frozen cmp -s "$OUT/identity/srpm_patch_inventory.armv7l.tsv" <(cut -f1-3 "$OUT/identity/frozen_patch_inventory.tsv") || fail=1

record compare_srpm_payload_arm_aarch cmp -s "$OUT/identity/srpm_payload_manifest.armv7l.tsv" "$OUT/identity/srpm_payload_manifest.aarch64.tsv" || fail=1
record compare_srpm_payload_arm_x86 cmp -s "$OUT/identity/srpm_payload_manifest.armv7l.tsv" "$OUT/identity/srpm_payload_manifest.x86_64.tsv" || fail=1

printf 'architecture\tfile\tnew_sha256\told_sha256\tcomparison\tattribution\n' >"$OUT/comparisons/new_vs_old_rpm_sha256.tsv"
for arch in "${ARCHES[@]}"; do
    for new in "$ART/$arch"/*.rpm; do
        base=$(basename "$new")
        old="$OLD/$arch/$base"
        new_sha=$(sha256sum "$new" | cut -d' ' -f1)
        if [[ -f "$old" ]]; then
            old_sha=$(sha256sum "$old" | cut -d' ' -f1)
            if [[ $new_sha == "$old_sha" ]]; then result=MATCH; else result=DIFFERENT; fi
        else
            old_sha=NOT_FOUND
            result=NO_OLD_COUNTERPART
        fi
        if [[ $base == *.src.rpm ]]; then
            attribution='STANDARD_PATCH_EXPORT_AND_PROVENANCE_DELTA'
        elif [[ $arch == armv7l ]]; then
            attribution='R6_WHOLE_GIT_TREE_TO_STANDARD_PATCH_EXPORT_AND_TOOLCHAIN_19.1_DELTA'
        else
            attribution='PRISTINE_PLATFORM_TARBALL_TO_STANDARD_SIX_PATCH_EXPORT_AND_TOOLCHAIN_19.1_DELTA'
        fi
        printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$arch" "$base" "$new_sha" "$old_sha" "$result" "$attribution" >>"$OUT/comparisons/new_vs_old_rpm_sha256.tsv"
    done
done

record assert_authoritative_llvm_unchanged bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse --abbrev-ref HEAD)" = tizen_base && test "$(git -C "$1" rev-parse HEAD)" = "$2" && test -z "$(git -C "$1" status --porcelain=v1)"' _ \
    "$REPO/codes/llvm" 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b || fail=1

if [[ $fail -eq 0 ]]; then
    printf 'IDENTITY_SELFCHECK=PASS\nSOURCE0_THREE_WAY=%s\nPATCHES_THREE_WAY=IDENTICAL_6\nSRPM_PAYLOAD_THREE_WAY=IDENTICAL\nSOURCE_PROVENANCE_THREE_WAY=IDENTICAL_AND_MATCHES_PACKAGING\n' "$EXPECTED_SOURCE0" >"$OUT/identity/identity_selfcheck.status"
    printf 'OVERALL_EXIT_CODE=0\n' >>"$LEDGER"
    exit 0
fi
printf 'IDENTITY_SELFCHECK=RED\n' >"$OUT/identity/identity_selfcheck.status"
printf 'OVERALL_EXIT_CODE=1\n' >>"$LEDGER"
exit 1
