#!/usr/bin/env bash
set -u
set -o pipefail

REPO=/home/toolchain/development/libc++_replacement
OUT="$REPO/progress/R9C"
RAW="$OUT/raw"
INPUTS="$OUT/inputs"
FEATURES="$OUT/tarball_features"
LEDGER="$OUT/commands/02_tarball_feature_comparison.log"
PLATFORM="$REPO/tmp/R9B/input/canonical/llvm-22.1.8.tar.gz"
LOCAL="$REPO/tmp/GBS-ROOT/LIBCXX-2218-preflight-20260805-std-r2/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/llvm-22.1.8.tar.gz"
TMP="$REPO/tmp/R9C/official"
OFFICIAL="$TMP/llvm-project-22.1.8.src.tar.xz"
API_URL=https://api.github.com/repos/llvm/llvm-project/releases/tags/llvmorg-22.1.8
ASSET_URL=https://github.com/llvm/llvm-project/releases/download/llvmorg-22.1.8/llvm-project-22.1.8.src.tar.xz

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
record_allowed() {
    local allowed=$1 label=$2 outfile=$3
    shift 3
    {
        printf 'LABEL=%s\nPWD=%s\nCOMMAND=' "$label" "$PWD"
        printf '%q ' "$@"
        printf '\n'
    } >>"$LEDGER"
    "$@" >"$outfile" 2>"$RAW/$label.stderr.txt"
    local rc=$?
    printf 'EXIT_CODE=%s\nALLOWED_EXIT_CODES=%s\n\n' "$rc" "$allowed" >>"$LEDGER"
    case " $allowed " in
        *" $rc "*) return 0 ;;
        *) return 1 ;;
    esac
}

cd "$REPO" || exit 125
overall=0
record_to create_official_download_directory "$RAW/create_official_download_directory.stdout.txt" mkdir -p "$TMP" || overall=1
record_to verify_local_inputs "$RAW/verify_local_inputs.stdout.txt" bash -o pipefail -c \
    'for f in "$@"; do test -f "$f" && printf "%s\\t%s\\n" "$f" "$(stat -c %s "$f")"; done' _ \
    "$PLATFORM" "$LOCAL" || overall=1
record_to github_release_api "$RAW/github_release_22.1.8.json" curl -fL --retry 3 --retry-delay 2 \
    -H 'Accept: application/vnd.github+json' -H 'X-GitHub-Api-Version: 2022-11-28' "$API_URL" || overall=1
record_to github_release_asset_identity "$FEATURES/official_release_asset.tsv" jq -r \
    '.html_url as $release | .tag_name as $tag | .assets[] | select(.name == "llvm-project-22.1.8.src.tar.xz") | [$release,$tag,.name,.size,.digest,.browser_download_url] | @tsv' \
    "$RAW/github_release_22.1.8.json" || overall=1
record_to official_asset_download "$RAW/official_asset_download.stdout.txt" curl -fL --retry 3 --retry-delay 2 \
    -o "$OFFICIAL" "$ASSET_URL" || overall=1
record_to official_tag_remote_identity "$FEATURES/official_tag_remote_identity.txt" git ls-remote \
    https://github.com/llvm/llvm-project.git refs/tags/llvmorg-22.1.8 'refs/tags/llvmorg-22.1.8^{}' || overall=1
record_to archive_sha256 "$FEATURES/archive_sha256.txt" sha256sum "$PLATFORM" "$LOCAL" "$OFFICIAL" || overall=1
record_to archive_file_identification "$FEATURES/archive_file_identification.txt" file "$PLATFORM" "$LOCAL" "$OFFICIAL" || overall=1
record_to gzip_header_hex "$FEATURES/gzip_header_hex.txt" bash -o pipefail -c \
    'for f in "$@"; do printf "FILE=%s\\n" "$f"; xxd -g 1 -l 32 "$f"; done' _ "$PLATFORM" "$LOCAL" || overall=1
record_to gzip_header_fields "$FEATURES/gzip_header_fields.tsv" bash -o pipefail -c \
    'printf "archive\\tmagic\\tcm\\tflg\\tmtime_le\\txfl\\tos\\n"; for f in "$@"; do set -- $(od -An -t u1 -N 10 "$f"); mtime=$(( $5 + ($6 << 8) + ($7 << 16) + ($8 << 24) )); printf "%s\\t%02x%02x\\t%s\\t%s\\t%s\\t%s\\t%s\\n" "$f" "$1" "$2" "$3" "$4" "$mtime" "$9" "${10}"; done' _ \
    "$PLATFORM" "$LOCAL" || overall=1
record_to gzip_listing "$FEATURES/gzip_listing.txt" gzip -lv "$PLATFORM" "$LOCAL" || overall=1
record_to official_xz_metadata "$FEATURES/official_xz_metadata.txt" xz --robot -lv "$OFFICIAL" || overall=1
record_to official_xz_header_hex "$FEATURES/official_xz_header_hex.txt" xxd -g 1 -l 32 "$OFFICIAL" || overall=1

record_to platform_top_directories "$FEATURES/platform_top_directories.txt" bash -o pipefail -c \
    'tar -tzf "$1" | awk -F/ "NF {print \$1}" | sort -u' _ "$PLATFORM" || overall=1
record_to local_top_directories "$FEATURES/local_top_directories.txt" bash -o pipefail -c \
    'tar -tzf "$1" | awk -F/ "NF {print \$1}" | sort -u' _ "$LOCAL" || overall=1
record_to official_top_directories "$FEATURES/official_top_directories.txt" bash -o pipefail -c \
    'tar -tJf "$1" | awk -F/ "NF {print \$1}" | sort -u' _ "$OFFICIAL" || overall=1
record_to archive_member_counts "$FEATURES/archive_member_counts.tsv" bash -o pipefail -c \
    'printf "archive\\tmembers\\n"; printf "platform\\t%s\\n" "$(tar -tzf "$1" | wc -l)"; printf "local_standard_export\\t%s\\n" "$(tar -tzf "$2" | wc -l)"; printf "official_release\\t%s\\n" "$(tar -tJf "$3" | wc -l)"' _ \
    "$PLATFORM" "$LOCAL" "$OFFICIAL" || overall=1
record_to platform_timestamp_distribution "$FEATURES/platform_timestamp_distribution.txt" bash -o pipefail -c \
    'LC_ALL=C tar --numeric-owner --full-time -tvzf "$1" | awk "{print \$4,\$5}" | sort | uniq -c' _ "$PLATFORM" || overall=1
record_to local_timestamp_distribution "$FEATURES/local_timestamp_distribution.txt" bash -o pipefail -c \
    'LC_ALL=C tar --numeric-owner --full-time -tvzf "$1" | awk "{print \$4,\$5}" | sort | uniq -c' _ "$LOCAL" || overall=1
record_to official_timestamp_distribution "$FEATURES/official_timestamp_distribution.txt" bash -o pipefail -c \
    'LC_ALL=C tar --numeric-owner --full-time -tvJf "$1" | awk "{print \$4,\$5}" | sort | uniq -c' _ "$OFFICIAL" || overall=1
record_to timestamp_distribution_summary "$FEATURES/timestamp_distribution_summary.tsv" bash -o pipefail -c \
    'printf "archive\\tdistinct_timestamps\\tfirst_distribution_row\\tlast_distribution_row\\n"; for pair in "platform:$1" "local_standard_export:$2" "official_release:$3"; do name=${pair%%:*}; f=${pair#*:}; printf "%s\\t%s\\t%s\\t%s\\n" "$name" "$(wc -l < "$f")" "$(head -n1 "$f")" "$(tail -n1 "$f")"; done' _ \
    "$FEATURES/platform_timestamp_distribution.txt" "$FEATURES/local_timestamp_distribution.txt" "$FEATURES/official_timestamp_distribution.txt" || overall=1
record_to archive_listing_samples "$FEATURES/archive_listing_samples.txt" bash -o pipefail -c \
    'for kind in platform local official; do case "$kind" in platform) f=$1; mode=z;; local) f=$2; mode=z;; official) f=$3; mode=J;; esac; printf "ARCHIVE=%s\\n" "$kind"; LC_ALL=C tar --numeric-owner --full-time -tv${mode}f "$f" | sed -n "1,5p; \$p"; done' _ \
    "$PLATFORM" "$LOCAL" "$OFFICIAL" || overall=1

record_to official_archival_member "$FEATURES/official_archival_member.txt" bash -o pipefail -c \
    'matches=$(tar -tJf "$1" | rg "/clang/bindings/python/\\.git_archival\\.txt$"); printf "%s\\n" "$matches"; test "$(printf "%s\\n" "$matches" | sed "/^$/d" | wc -l)" -eq 1' _ "$OFFICIAL" || overall=1
if [[ -s "$FEATURES/official_archival_member.txt" ]]; then
    official_member=$(sed -n '1p' "$FEATURES/official_archival_member.txt")
    record_to official_archival_content "$INPUTS/official_release.git_archival.txt" tar -xOJf "$OFFICIAL" "$official_member" || overall=1
    record_to archival_file_sha256_all "$FEATURES/archival_file_sha256_all.txt" sha256sum \
        "$INPUTS/platform.git_archival.txt" "$INPUTS/local_standard_export.git_archival.txt" "$INPUTS/official_release.git_archival.txt" || overall=1
    record_allowed '0 1' platform_vs_official_archival_diff "$INPUTS/platform_vs_official.git_archival.diff" diff -u \
        "$INPUTS/platform.git_archival.txt" "$INPUTS/official_release.git_archival.txt" || overall=1
    record_allowed '0 1' local_vs_official_archival_diff "$INPUTS/local_vs_official.git_archival.diff" diff -u \
        "$INPUTS/local_standard_export.git_archival.txt" "$INPUTS/official_release.git_archival.txt" || overall=1
else
    printf 'official .git_archival.txt member was not found; extraction skipped\n' >"$FEATURES/official_archival_NOT_AVAILABLE.txt"
fi

record_to assert_llvm_unchanged "$RAW/assert_llvm_unchanged_after_tar_features.stdout.txt" bash -o pipefail -c \
    'test "$(git -C "$1" rev-parse HEAD)" = 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b && test -z "$(git -C "$1" status --porcelain=v1)"' _ \
    "$REPO/codes/llvm" || overall=1

printf 'OVERALL_EXIT_CODE=%s\n' "$overall" >>"$LEDGER"
exit "$overall"
