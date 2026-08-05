#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LLVM="$ROOT/codes/llvm"
OUT="$ROOT/progress/R9/generated"
SRPM="$ROOT/tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm"
PLATFORM_COMMIT=c12cbe887ff08f04628e1b1165f8d02ce69a9146
UPSTREAM_COMMIT=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1
PATCH_COMMIT=bdfb133d1383f1c7e04c5f973374c911708a383b

record() {
    local command="$1"
    printf 'COMMAND=%s\n' "$command"
    (cd "$ROOT" && bash -o pipefail -c "$command")
    local rc=$?
    printf 'EXIT_CODE=%d\n\n' "$rc"
    return 0
}

record "test -f '$SRPM'"
record "sha256sum '$SRPM'"
record "rpm2cpio '$SRPM' | cpio -it | LC_ALL=C sort"
record "rpm2cpio '$SRPM' | cpio -i --to-stdout 'llvm-22.1.8.tar.gz' | sha256sum"
record "mkdir -p '$OUT/srpm'"
record "(cd '$OUT/srpm' && rpm2cpio '$SRPM' | cpio -idmv '0001-Apply-Tizen-toolchain-compatibility-fixes.patch' 'llvm.spec')"
record "sha256sum '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/srpm/llvm.spec'"
record "nl -ba '$OUT/srpm/llvm.spec' | sed -n '35,55p;150,168p'"
record "sed -n '1,80p' '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch'"

record "git -C '$LLVM' log --all --format='%H%x09%P%x09%s' --regexp-ignore-case --grep='Apply Tizen toolchain compatibility fixes'"
record "git -C '$LLVM' show -s --format=fuller '$PATCH_COMMIT'"
record "git -C '$LLVM' show --stat --summary '$PATCH_COMMIT'"
record "git -C '$LLVM' show --format= --name-status '$PATCH_COMMIT'"
record "git -C '$LLVM' show --format= '$PATCH_COMMIT' -- packaging/llvm.spec"
record "git -C '$LLVM' merge-base --is-ancestor '$PATCH_COMMIT' '$PLATFORM_COMMIT'"
record "git -C '$LLVM' merge-base --is-ancestor '$PATCH_COMMIT' tizen_base"
record "git -C '$LLVM' branch -a --contains '$PATCH_COMMIT'"
record "for c in \$(git -C '$LLVM' rev-list --reverse '$UPSTREAM_COMMIT..$PLATFORM_COMMIT'); do printf '%s\\t' \"\$c\"; git -C '$LLVM' diff-tree --no-commit-id --name-only -r \"\$c\" | awk 'BEGIN{s=0;p=0}{if (\$0 ~ /^packaging\\//) p++; else s++}END{printf \"source_files=%d packaging_files=%d\\n\",s,p}'; done"
record "for c in \$(git -C '$LLVM' rev-list --reverse '$UPSTREAM_COMMIT..tizen_base'); do printf '%s\\t' \"\$c\"; git -C '$LLVM' diff-tree --no-commit-id --name-only -r \"\$c\" | awk 'BEGIN{s=0;p=0}{if (\$0 ~ /^packaging\\//) p++; else s++}END{printf \"source_files=%d packaging_files=%d\\n\",s,p}'; done"

record "git -C '$LLVM' format-patch -1 --stdout --no-signature '$PATCH_COMMIT' -- . ':(exclude)packaging/**' > '$OUT/bdf_source_only_git_format_patch.patch'"
record "sha256sum '$OUT/bdf_source_only_git_format_patch.patch'"
record "diff -u '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/bdf_source_only_git_format_patch.patch' > '$OUT/patch0_vs_git_format_patch.diff'"
record "cat '$OUT/patch0_vs_git_format_patch.diff'"
record "sed -e '1{/^From [0-9a-f]\\{40\\} Mon Sep 17 00:00:00 2001\$/d;}' -e 's/^Subject: \\[PATCH\\] /Subject: /' '$OUT/bdf_source_only_git_format_patch.patch' > '$OUT/bdf_source_only_gbp_header_normalized.patch'"
record "sha256sum '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/bdf_source_only_gbp_header_normalized.patch'"
record "cmp -s '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/bdf_source_only_gbp_header_normalized.patch'"
record "diff -u '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/bdf_source_only_gbp_header_normalized.patch'"
record "wc -c '$OUT/srpm/0001-Apply-Tizen-toolchain-compatibility-fixes.patch' '$OUT/bdf_source_only_gbp_header_normalized.patch'"
record "git -C '$LLVM' status --porcelain=v1"
