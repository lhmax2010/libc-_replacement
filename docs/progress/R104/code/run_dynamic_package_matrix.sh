#!/bin/bash
set -euo pipefail

if [ "$#" -ne 4 ]; then
  echo "usage: $0 PACKAGE SPEC NUMBER_BASE RELEASE_BASE" >&2
  exit 2
fi

package=$1
spec=$2
number_base=$3
release_base=$4
workspace=/home/toolchain/development/libc++_replacement
repo="$workspace/codes/R104/$package"
logger="$workspace/progress/R104/code/run_logged.sh"
verifier="$workspace/progress/R104/code/verify_dynamic_cell.sh"
config="$workspace/gbs_llvm.conf"
target_branch=sandbox/lhmax2025/libcxx-migration
summary="$workspace/progress/R104/work/matrix_${package}.summary"

: >"$summary"
cell=0
package_ok=1
for arch in armv7l aarch64 x86_64; do
  for mode in libcxx gcc; do
    cell=$((cell + 1))
    number=$((number_base + (cell - 1) * 2))
    verify_number=$((number + 1))
    release="${release_base}.${cell}"
    root="$workspace/tmp/GBS-ROOT/R104-matrix-${arch}-libcxx"
    build_label="build_${package}_${arch}_${mode}"
    verify_label="verify_${package}_${arch}_${mode}"
    build_stdout="$workspace/progress/R104/raw/${number}_${build_label}.stdout"
    args=(nice -n 15 ionice -c 3 gbs -c "$config" build -A "$arch"
      -B "$root" -C --fail-fast --threads 1 --define '_smp_mflags -j2')
    if [ "$mode" = gcc ]; then
      args+=(--define '_toolchain_override gcc')
    fi
    args+=(--release "$release" --style git --include-all
      --packaging-dir packaging --spec "$spec")

    printf '%s %s START number=%s release=%s\n' "$arch" "$mode" "$number" "$release" | tee -a "$summary"
    if ! (cd "$repo" && bash "$logger" "$number" "$build_label" "${args[@]}" >/dev/null 2>&1); then
      rc=$(cat "$workspace/progress/R104/raw/${number}_${build_label}.exitcode")
      printf '%s %s BUILD_FAIL rc=%s\n' "$arch" "$mode" "$rc" | tee -a "$summary"
      package_ok=0
      continue
    fi
    if ! bash "$logger" "$verify_number" "$verify_label" "$verifier" \
      "$package" "$arch" "$mode" "$root" "$release" "$build_stdout" >/dev/null 2>&1; then
      rc=$(cat "$workspace/progress/R104/raw/${verify_number}_${verify_label}.exitcode")
      printf '%s %s VERIFY_FAIL rc=%s\n' "$arch" "$mode" "$rc" | tee -a "$summary"
      package_ok=0
      continue
    fi
    printf '%s %s PASS\n' "$arch" "$mode" | tee -a "$summary"
    cat /proc/loadavg | tee -a "$summary"
    awk '/MemAvailable/ {print}' /proc/meminfo | tee -a "$summary"
  done
done

if [ "$package_ok" -ne 1 ]; then
  echo "PACKAGE_NOT_PUSHED" | tee -a "$summary"
  exit 1
fi

audit_number=$((number_base + 12))
commit_number=$((number_base + 13))
push_number=$((number_base + 14))
verify_push_number=$((number_base + 15))
branch_compare_number=$((number_base + 16))

bash "$logger" "$audit_number" "audit_${package}_before_commit" bash -c \
  "set -euo pipefail; git -C '$repo' diff --check; test \"\$(git -C '$repo' diff --name-only)\" = 'packaging/$spec'; remote=\$(git -C '$repo' ls-remote --heads origin refs/heads/$target_branch | cut -f1); local=\$(git -C '$repo' rev-parse HEAD); echo remote=\$remote; echo local=\$local; test \"\$remote\" = \"\$local\"; git -C '$repo' status --short --branch" >/dev/null

git -C "$repo" config user.name hao.lin
git -C "$repo" config user.email hao.lin@samsung.com
bash "$logger" "$commit_number" "commit_${package}" git -C "$repo" commit \
  -am "packaging: select libc++ for Clang toolchain builds

Use libc++ only when the Base Toolchain selector resolves to Clang.
Keep GCC and non-toolchain builds on their existing standard library.

在 Base Toolchain 选择 Clang 时使用 libc++；GCC 与非工具链构建保持原行为。" >/dev/null

bash "$logger" "$push_number" "push_${package}_sandbox" git -C "$repo" push origin \
  "HEAD:refs/heads/$target_branch" >/dev/null

bash "$logger" "$verify_push_number" "verify_${package}_sandbox_push" bash -c \
  "set -euo pipefail; local=\$(git -C '$repo' rev-parse HEAD); remote=\$(git -C '$repo' ls-remote --heads origin refs/heads/$target_branch | cut -f1); echo local=\$local; echo remote=\$remote; test \"\$local\" = \"\$remote\"; git -C '$repo' status --short --branch" >/dev/null
bash "$logger" "$branch_compare_number" "verify_${package}_other_branches" \
  "$workspace/progress/R104/code/verify_remote_branches.sh" "$package" >/dev/null
echo "PACKAGE_PUSHED $(git -C "$repo" rev-parse HEAD)" | tee -a "$summary"
