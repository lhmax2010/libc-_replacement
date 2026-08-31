# R83 人工 GBS 全量编译运行手册

本手册不在 R83 本机执行。它固定使用 Gerrit sandbox HEAD
`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`，先构建同一提交的 LLVM/Clang，
再构建 `libcxx-runtimes`，最后把两者作为优先本地仓依赖执行全量构建。

## 0. 人工只需确认的路径与权限

下面给出可直接执行的默认路径。若服务器没有 `/srv` 写权限，只需把
`R83_ROOT` 改为执行账号拥有的绝对路径；`GERRIT_USER` 仅在账号不是
`lhmax2025` 时修改。除此之外不要改分支名和提交 SHA。

```bash
set -eu
export R83_ROOT=/srv/r83-libcxx-full-build
export R83_INPUT=/srv/r83-input
export R83_MATERIALS=/srv/r83-input/R83
export GERRIT_USER=lhmax2025
export LLVM_REF=refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
export LLVM_SHA=08a64b6eb95ed74339f1d862a6c22a2f93fa78f9
export LLVM_SRC="$R83_ROOT/src/llvm"
export BASE_URL=https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/
export UNIFIED_URL=https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/reference/repos/standard/packages/
install -d "$R83_ROOT"/{src,config,repo,logs,results,gbs-root}
test -r "$R83_INPUT/Tizen-Base-Toolchain/project_config"
test -r "$R83_INPUT/Tizen-Unified-Toolchain/project_config"
test -r "$R83_MATERIALS/artifacts/PROJECT_CONFIG_CHANGES.diff"
```

前置条件：执行账号能通过 SSH 读取 Gerrit；能用 `sudo` 创建 GBS chroot；
能访问两个 HTTPS snapshot 仓；已安装 GBS 2.0.8、Git、RPM/rpmbuild、
createrepo、patch、cpio/rpm2cpio、coreutils、findutils、ripgrep。宿主无需预装 clang，GBS 会在
buildroot 中安装目标 clang。

## 1. 固定 sandbox 源码身份

```bash
git clone "ssh://${GERRIT_USER}@review.tizen.org:29418/platform/upstream/llvm" "$LLVM_SRC"
git -C "$LLVM_SRC" fetch origin "$LLVM_REF"
git -C "$LLVM_SRC" switch --detach FETCH_HEAD
test "$(git -C "$LLVM_SRC" rev-parse HEAD)" = "$LLVM_SHA"
test -z "$(git -C "$LLVM_SRC" status --porcelain=v1)"
git -C "$LLVM_SRC" log --oneline --decorate -10 >"$R83_ROOT/results/sandbox-log.txt"
git -C "$LLVM_SRC" show-ref --verify refs/tags/upstream/22.1.8 >"$R83_ROOT/results/upstream-tag.txt"
```

这一步而不是 `project_config` 决定源码分支。两份 `project_config` 中没有 Git
URL、分支或 sandbox 选择项。

## 2. 生成两套只供本次使用的 buildconf

原文件不修改。`bootstrap.conf` 用于构建 LLVM 和 libc++ provider；
`libcxx.conf` 在其副本上注入 libc++ 开发包和消费者编译/链接选项。

```bash
cp -a "$R83_INPUT/Tizen-Base-Toolchain/project_config" "$R83_ROOT/config/Base.project_config.orig"
cp -a "$R83_INPUT/Tizen-Unified-Toolchain/project_config" "$R83_ROOT/config/Unified.project_config.orig"
test "$(sha256sum "$R83_ROOT/config/Base.project_config.orig" | awk '{print $1}')" = aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209
test "$(sha256sum "$R83_ROOT/config/Unified.project_config.orig" | awk '{print $1}')" = a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086
cp -a "$R83_ROOT/config/Base.project_config.orig" "$R83_ROOT/config/Base.project_config"
cp -a "$R83_ROOT/config/Unified.project_config.orig" "$R83_ROOT/config/Unified.project_config"
patch --fuzz=0 --dry-run -d "$R83_ROOT/config" -p0 <"$R83_MATERIALS/artifacts/PROJECT_CONFIG_CHANGES.diff"
patch --fuzz=0 -d "$R83_ROOT/config" -p0 <"$R83_MATERIALS/artifacts/PROJECT_CONFIG_CHANGES.diff"
cat "$R83_ROOT/config/Base.project_config.orig" "$R83_ROOT/config/Unified.project_config.orig" >"$R83_ROOT/config/bootstrap.conf"
cat "$R83_ROOT/config/Base.project_config" "$R83_ROOT/config/Unified.project_config" >"$R83_ROOT/config/libcxx.conf"
sha256sum "$R83_ROOT/config/"*.conf "$R83_ROOT/config/"*.project_config* >"$R83_ROOT/results/config-sha256.txt"
```

## 3. 公共日志函数

```bash
run_logged() {
  label=$1
  shift
  printf '%q ' "$@" >"$R83_ROOT/logs/${label}.command.txt"
  printf '\n' >>"$R83_ROOT/logs/${label}.command.txt"
  set +e
  "$@" >"$R83_ROOT/logs/${label}.stdout" 2>"$R83_ROOT/logs/${label}.stderr"
  rc=$?
  set -e
  printf '%s\n' "$rc" >"$R83_ROOT/logs/${label}.exitcode"
  return "$rc"
}
```

任何 `run_logged` 非零都立即停；不得改补丁后续跑。若只是仓连接瞬断，保存
原日志后最多重试一次。

## 4. 每架构先构建 sandbox LLVM，再构建新 libc++/libc++abi

任务未指定最终架构矩阵。此手册按 `libcxx-runtimes.spec` 的全部
`ExclusiveArch` 执行：`x86_64 armv7l aarch64`。若人工只批准其中一部分，
必须在结果报告中明确缩小后的范围。

```bash
for arch in x86_64 armv7l aarch64; do
  install -d "$R83_ROOT/repo/$arch"

  cat >"$R83_ROOT/config/bootstrap-${arch}.gbs.conf" <<EOF
[general]
profile = profile.r83
buildroot = $R83_ROOT/gbs-root/bootstrap-$arch

[profile.r83]
repos = repo.base, repo.unified
buildconf = $R83_ROOT/config/bootstrap.conf

[repo.base]
url=$BASE_URL

[repo.unified]
url=$UNIFIED_URL
EOF

  run_logged "llvm-${arch}" nice -n 15 ionice -c 3 gbs -c "$R83_ROOT/config/bootstrap-${arch}.gbs.conf" build \
    -A "$arch" -B "$R83_ROOT/gbs-root/llvm-$arch" -C --fail-fast --threads 1 \
    --define '_smp_mflags -j2' --release 83.1 --style git \
    --packaging-dir packaging --spec llvm.spec "$LLVM_SRC"

  find "$R83_ROOT/gbs-root/llvm-$arch/local/repos" -type f -path "*/$arch/RPMS/*.rpm" \
    -exec cp -a '{}' "$R83_ROOT/repo/$arch/" ';'
  createrepo --update --changelog-limit=0 "$R83_ROOT/repo/$arch"

  cat >"$R83_ROOT/config/provider-${arch}.gbs.conf" <<EOF
[general]
profile = profile.r83
buildroot = $R83_ROOT/gbs-root/provider-$arch

[profile.r83]
repos = repo.r83, repo.base, repo.unified
buildconf = $R83_ROOT/config/bootstrap.conf

[repo.r83]
url=file://$R83_ROOT/repo/$arch

[repo.base]
url=$BASE_URL

[repo.unified]
url=$UNIFIED_URL
EOF

  run_logged "libcxx-runtimes-${arch}" nice -n 15 ionice -c 3 gbs -c "$R83_ROOT/config/provider-${arch}.gbs.conf" build \
    -A "$arch" -B "$R83_ROOT/gbs-root/provider-$arch" -C --fail-fast --threads 1 \
    --define '_smp_mflags -j2' --use-higher-deps --release 83.1 --style git \
    --packaging-dir packaging --spec libcxx-runtimes.spec "$LLVM_SRC"

  find "$R83_ROOT/gbs-root/provider-$arch/local/repos" -type f -path "*/$arch/RPMS/*.rpm" \
    -exec cp -a '{}' "$R83_ROOT/repo/$arch/" ';'
  createrepo --update --changelog-limit=0 "$R83_ROOT/repo/$arch"

  test "$(find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++-22.1.8-83.1.*.rpm' | wc -l)" -eq 1
  test "$(find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++-devel-22.1.8-83.1.*.rpm' | wc -l)" -eq 1
  test "$(find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++abi-22.1.8-83.1.*.rpm' | wc -l)" -eq 1
  test "$(find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++abi-devel-22.1.8-83.1.*.rpm' | wc -l)" -eq 1

  extract_dir="$R83_ROOT/results/provider-extracted-$arch"
  install -d "$extract_dir"
  find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++*-22.1.8-83.1.*.rpm' \
    ! -name '*debuginfo*' ! -name '*debugsource*' -print0 | while IFS= read -r -d '' rpm_file; do
      (cd "$extract_dir" && rpm2cpio "$rpm_file" | cpio -idmu)
    done
  rg -q '__forced_unwind' "$extract_dir/usr/include/c++/v1/__ostream/basic_ostream.h"
  rg -q '__forced_unwind' "$extract_dir/usr/include/c++/v1/future"
  rg -q '__forced_unwind' "$extract_dir/usr/include/c++/v1/istream"
  rg -q '__forced_unwind' "$extract_dir/usr/include/c++/v1/string"
  rg -q 'class .*__forced_unwind' "$extract_dir/usr/include/c++/v1/cxxabi.h"
  find "$R83_ROOT/repo/$arch" -maxdepth 1 -type f -name 'libc++*-22.1.8-83.1.*.rpm' \
    -print0 | sort -z | xargs -0 sha256sum >"$R83_ROOT/results/provider-$arch.sha256"
done
```

`--release 83.1` 只改变验证 RPM 的 Release，不改源码或补丁；其目的在于避免
与仓中可能存在的 `22.1.8-1` 同 NEVRA 包混淆。`repo.r83` 排第一并配合
`--use-higher-deps`，使 buildroot 选择 sandbox 产物。

## 5. 在安装新 provider 的 buildroot 环境中执行全量构建

这里的“先安装”是把 RPM 发布到优先本地仓，并由 GBS 在每个隔离 buildroot
中安装；不要把目标 RPM 安装到宿主系统。

```bash
for arch in x86_64 armv7l aarch64; do
  cat >"$R83_ROOT/config/full-${arch}.gbs.conf" <<EOF
[general]
profile = profile.r83
buildroot = $R83_ROOT/gbs-root/full-$arch

[profile.r83]
repos = repo.r83, repo.base, repo.unified
buildconf = $R83_ROOT/config/libcxx.conf

[repo.r83]
url=file://$R83_ROOT/repo/$arch

[repo.base]
url=$BASE_URL

[repo.unified]
url=$UNIFIED_URL
EOF

  run_logged "full-${arch}" nice -n 15 ionice -c 3 gbs -c "$R83_ROOT/config/full-${arch}.gbs.conf" build \
    -A "$arch" -B "$R83_ROOT/gbs-root/full-$arch" -C --full-build --fail-fast \
    --threads 2 --define '_smp_mflags -j2' --use-higher-deps --release 83.1 \
    --style git --packaging-dir packaging --spec llvm.spec "$LLVM_SRC"
done
```

`--full-build` 的远端源码发现依赖执行服务器现有的 Tizen GBS/snapshot 服务。
本机材料只含二进制仓 URL和 project_config，没有该服务器的认证代理或内部
源码镜像配置；若服务器的标准 profile 另有 source service，保留该配置并把
`repo.r83` 置于最前。若上述命令在“下载全量源码”阶段失败，记录为环境前置
条件不满足，不要改源码。

## 6. 结果归档

```bash
find "$R83_ROOT/repo" -type f -name '*.rpm' -print0 | sort -z | xargs -0 sha256sum >"$R83_ROOT/results/provider-rpm-sha256.txt"
find "$R83_ROOT/gbs-root" -type f \( -name 'index.html' -o -name 'log.txt' -o -name '.build.log' \) \
  -printf '%p\t%s\n' | sort >"$R83_ROOT/results/gbs-result-files.tsv"
find "$R83_ROOT/gbs-root" -type f -path '*/logs/fail/*' -printf '%p\t%s\n' | sort >"$R83_ROOT/results/fail-logs.tsv"
find "$R83_ROOT/gbs-root" -type f -path '*/logs/success/*' -printf '%p\t%s\n' | sort >"$R83_ROOT/results/success-logs.tsv"
rg -n -i 'error:|fatal error:|undefined reference|cannot find -l|failed|warning:' \
  "$R83_ROOT/logs" "$R83_ROOT/gbs-root" >"$R83_ROOT/results/diagnostics.txt" || true
tar -C "$R83_ROOT" -caf "$R83_ROOT/r83-full-build-evidence.tar.zst" \
  logs results config repo
sha256sum "$R83_ROOT/r83-full-build-evidence.tar.zst" >"$R83_ROOT/results/r83-full-build-evidence.tar.zst.sha256"
```

不要只交汇总：保留每条 command/stdout/stderr/exitcode、所有失败包完整
`log.txt`、GBS `index.html`、RPM 清单和 SHA256。
