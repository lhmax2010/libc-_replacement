#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
LLVM="$ROOT/codes/llvm"
PLATFORM_COMMIT=c12cbe887ff08f04628e1b1165f8d02ce69a9146
UPSTREAM_COMMIT=ca7933e47d3a3451d81e72ac174dcb5aa28b59d1

record() {
    local command="$1"
    printf 'COMMAND=%s\n' "$command"
    (cd "$ROOT" && bash -o pipefail -c "$command")
    local rc=$?
    printf 'EXIT_CODE=%d\n\n' "$rc"
    return 0
}

record "pwd"
record "git -C '$LLVM' status --porcelain=v1"
record "git -C '$LLVM' branch --show-current"
record "git -C '$LLVM' rev-parse HEAD"
record "git -C '$LLVM' remote -v"

for file in .gbs.conf gbs.conf debian/gbp.conf .gbp.conf; do
    record "if test -f '$LLVM/$file'; then echo FOUND_WORKTREE:codes/llvm/$file; nl -ba '$LLVM/$file'; else echo NOT_FOUND_WORKTREE:codes/llvm/$file; fi"
    record "if git -C '$LLVM' cat-file -e HEAD:'$file' 2>/dev/null; then echo FOUND_HEAD:codes/llvm/$file; git -C '$LLVM' show HEAD:'$file' | nl -ba; else echo NOT_FOUND_HEAD:codes/llvm/$file; fi"
done

record "find '$LLVM/packaging' -maxdepth 1 -type f ! -name '*.spec' -printf '%f\\n' | LC_ALL=C sort"
record "nl -ba '$LLVM/packaging/llvm.manifest'"
record "sha256sum '$LLVM'/packaging/* 2>/dev/null | LC_ALL=C sort"
record "nl -ba '$LLVM/packaging/llvm.spec' | sed -n '30,55p;150,165p'"
record "git -C '$LLVM' ls-tree -r --name-only '$PLATFORM_COMMIT' | awk '\$0==\".gbs.conf\" || \$0==\"gbs.conf\" || \$0==\"debian/gbp.conf\" || \$0==\".gbp.conf\" {print}'"
record "for f in .gbs.conf gbs.conf debian/gbp.conf .gbp.conf; do if git -C '$LLVM' cat-file -e '$PLATFORM_COMMIT':\"\$f\" 2>/dev/null; then echo FOUND:\$f; git -C '$LLVM' show '$PLATFORM_COMMIT':\"\$f\" | nl -ba; else echo NOT_FOUND_AT_PLATFORM_COMMIT:\$f; fi; done"
record "git -C '$LLVM' show '$PLATFORM_COMMIT':packaging/llvm.spec | nl -ba | sed -n '30,55p;150,165p'"

record "git -C '$LLVM' branch -a"
record "git -C '$LLVM' for-each-ref --format='%(refname) %(objectname) %(objecttype)' refs/heads refs/remotes | rg '(^|/)(upstream|upstream-[^ /]*|pristine-tar[^ /]*) ' || true"
record "git -C '$LLVM' tag --list '*22.1.8*'"
record "git -C '$LLVM' tag --list 'llvmorg*'"
record "git -C '$LLVM' show-ref --dereference refs/tags/upstream/22.1.8"
record "git -C '$LLVM' rev-parse 'refs/tags/upstream/22.1.8^{}'"
record "test \"\$(git -C '$LLVM' rev-parse 'refs/tags/upstream/22.1.8^{}')\" = '$UPSTREAM_COMMIT'"
record "git -C '$LLVM' log -1 --decorate=full --format=fuller refs/tags/upstream/22.1.8"

record "git -C '$LLVM' log --all --oneline -- .gbs.conf"
record "git -C '$LLVM' show 9f851548da1d:.gbs.conf | nl -ba"
record "git -C '$LLVM' log --oneline --reverse '$UPSTREAM_COMMIT..$PLATFORM_COMMIT'"
record "git -C '$LLVM' log --format='%H%x09%P%x09%s' --reverse '$UPSTREAM_COMMIT..$PLATFORM_COMMIT'"
record "git -C '$LLVM' log --oneline --reverse '$UPSTREAM_COMMIT..tizen_base'"
record "git -C '$LLVM' log --format='%H%x09%P%x09%s' --reverse '$UPSTREAM_COMMIT..tizen_base'"
record "git -C '$LLVM' rev-list --merges '$UPSTREAM_COMMIT..tizen_base'"
record "git -C '$LLVM' log --format='%H%n%B' '$UPSTREAM_COMMIT..tizen_base' | rg -i '^gbp-rpm:' || echo NOT_FOUND:gbp-rpm_commit_directives"
record "git -C '$LLVM' merge-base --is-ancestor '$PLATFORM_COMMIT' tizen_base"
record "git -C '$LLVM' status --porcelain=v1"
