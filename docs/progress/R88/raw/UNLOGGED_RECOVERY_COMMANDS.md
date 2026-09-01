# R88 未经包装器的恢复命令记录

下列命令因包装器参数误用或用于观察仍在运行的低优先级导出进程，未由
`run_logged.sh` 自动拆分为 command/stdout/stderr/exitcode 四件套。这里按
实际执行顺序补录，不把它们伪装成自动生成日志。

## U01：首次误用包装器

命令：

```bash
progress/R88/code/run_logged.sh progress/R88/raw/003_remote_refs.txt git -C codes/llvm ls-remote origin refs/heads/tizen_base refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

输出：

```text
progress/R88/code/run_logged.sh: line 15: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.command.txt: No such file or directory
progress/R88/code/run_logged.sh: line 16: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.command.txt: No such file or directory
progress/R88/code/run_logged.sh: line 18: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.stdout: No such file or directory
progress/R88/code/run_logged.sh: line 20: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.exitcode: No such file or directory
cat: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.stdout: No such file or directory
cat: /home/toolchain/development/libc++_replacement/progress/R88/raw/progress/R88/raw/003_remote_refs.txt_git.stderr: No such file or directory
```

退出码：`1`。远端命令未执行。

## U02：查看包装器接口

命令：

```bash
sed -n '1,220p' progress/R88/code/run_logged.sh && find progress/R88/raw -maxdepth 2 -type f -printf '%P\n' | sort | tail -30
```

完整输出：

```text
#!/bin/bash
set -u

if [ "$#" -lt 3 ]; then
  echo "usage: $0 NUMBER LABEL COMMAND..." >&2
  exit 2
fi

number=$1
label=$2
shift 2
raw_dir=$(cd "$(dirname "$0")/../raw" && pwd)
prefix="$raw_dir/${number}_${label}"

printf '%q ' "$@" >"${prefix}.command.txt"
printf '\n' >>"${prefix}.command.txt"
set +e
"$@" >"${prefix}.stdout" 2>"${prefix}.stderr"
rc=$?
printf '%s\n' "$rc" >"${prefix}.exitcode"
cat "${prefix}.stdout"
cat "${prefix}.stderr" >&2
exit "$rc"
001_resource_gate.command.txt
001_resource_gate.exitcode
001_resource_gate.stderr
001_resource_gate.stdout
002_local_state.command.txt
002_local_state.exitcode
002_local_state.stderr
002_local_state.stdout
```

退出码：`0`；未接触被核对仓。

## U03：首次观察仍在运行的 GBS export

命令：

```bash
ps -eo pid,ppid,ni,stat,etime,%cpu,%mem,cmd | rg 'gbs export|run_logged.sh 017' || true; ls -l progress/R88/raw/017_* 2>/dev/null || true; tail -80 progress/R88/raw/017_squash_gbs_export.stderr 2>/dev/null || true; tail -80 progress/R88/raw/017_squash_gbs_export.stdout 2>/dev/null || true; du -sh tmp/R88/export 2>/dev/null || true
```

完整输出：

```text
1327373    6856   0 Ss         00:37  0.0  0.0 /bin/bash progress/R88/code/run_logged.sh 017 squash_gbs_export bash -lc  set -u export_dir=/home/toolchain/development/libc++_replacement/tmp/R88/export if test -e "$export_dir"; then echo "REFUSE_EXISTING=$export_dir" >&2; exit 90; fi mkdir -p "$export_dir" cd /home/toolchain/development/libc++_replacement/codes/llvm || exit 125 nice -n 15 ionice -c 3 gbs export --spec libcxx-runtimes.spec --packaging-dir packaging --commit e79f1c3dde12c356142b77231e304c0da67adaf1 -o "$export_dir" .
1327376 1327373  15 RN         00:37  8.7  0.1 /usr/bin/python3 /usr/bin/gbs export --spec libcxx-runtimes.spec --packaging-dir packaging --commit e79f1c3dde12c356142b77231e304c0da67adaf1 -o /home/toolchain/development/libc++_replacement/tmp/R88/export .
1328350    6856   0 Ss         00:00  0.0  0.0 /bin/bash -c ps -eo pid,ppid,ni,stat,etime,%cpu,%mem,cmd | rg 'gbs export|run_logged.sh 017' || true; ls -l progress/R88/raw/017_* 2>/dev/null || true; tail -80 progress/R88/raw/017_squash_gbs_export.stderr 2>/dev/null || true; tail -80 progress/R88/raw/017_squash_gbs_export.stdout 2>/dev/null || true; du -sh tmp/R88/export 2>/dev/null || true
1328360 1328350   0 S          00:00  0.0  0.0 rg gbs export|run_logged.sh 017
-rw-rw-r-- 1 toolchain toolchain 438 Sep  1 16:39 progress/R88/raw/017_squash_gbs_export.command.txt
-rw-rw-r-- 1 toolchain toolchain   0 Sep  1 16:39 progress/R88/raw/017_squash_gbs_export.stderr
-rw-rw-r-- 1 toolchain toolchain 218 Sep  1 16:39 progress/R88/raw/017_squash_gbs_export.stdout
info: tracking branch: origin/upstream -> upstream
info: Generating patches from git (upstream/22.1.8..e79f1c3dde12c356142b77231e304c0da67adaf1)
info: llvm-22.1.8.tar.gz does not exist, creating from 'upstream/22.1.8'
4.0K tmp/R88/export
```

退出码：`0`。这里只观察，没有启动第二个导出。

## U04：等待同一导出结束

命令：

```bash
while ps -p 1327376 >/dev/null 2>&1; do ps -p 1327376 -o pid,ni,stat,etime,%cpu,%mem,cmd; sleep 10; done; echo PROCESS_DONE; cat progress/R88/raw/017_squash_gbs_export.exitcode 2>/dev/null || true; tail -120 progress/R88/raw/017_squash_gbs_export.stdout; tail -120 progress/R88/raw/017_squash_gbs_export.stderr >&2; du -sh tmp/R88/export
```

完整输出：

```text
    PID  NI STAT     ELAPSED %CPU %MEM CMD
1327376  15 SN         00:42  8.8  0.1 /usr/bin/python3 /usr/bin/gbs export --spec libcxx-runtimes.spec --packaging-dir packaging --commit e79f1c3dde12c356142b77231e304c0da67adaf1 -o /home/toolchain/development/libc++_replacement/tmp/R88/export .
    PID  NI STAT     ELAPSED %CPU %MEM CMD
1327376  15 SN         00:52 11.8  0.1 /usr/bin/python3 /usr/bin/gbs export --spec libcxx-runtimes.spec --packaging-dir packaging --commit e79f1c3dde12c356142b77231e304c0da67adaf1 -o /home/toolchain/development/libc++_replacement/tmp/R88/export .
PROCESS_DONE
0
info: tracking branch: origin/upstream -> upstream
info: Generating patches from git (upstream/22.1.8..e79f1c3dde12c356142b77231e304c0da67adaf1)
info: llvm-22.1.8.tar.gz does not exist, creating from 'upstream/22.1.8'
info: Generating patches from git (upstream/22.1.8..e79f1c3dde12c356142b77231e304c0da67adaf1)
info: package files have been exported to:
     /home/toolchain/development/libc++_replacement/tmp/R88/export/libcxx-runtimes-22.1.8-1
330M tmp/R88/export
```

退出码：`0`。

## U05：定位首次交付审计的非零原因

命令：

```bash
sed -n '1,240p' progress/R88/raw/029_delivery_content_audit.stderr; sed -n '1,260p' progress/R88/raw/029_delivery_content_audit.stdout | tail -40; git diff --check -- docs/progress/R88; echo DIFFCHECK_RC=$?
```

完整输出：

```text
PASS 1 0001-Add-support-for-Tizen-toolchain-vendor.patch a71a7503e851f4ebe1c192326d832c28f79a81ff1c9db43696d0487c118d587c cf2d5d98ab6c48ef843b1c5d9372cc5640ad2b6c
PASS 2 0002-Link-libclang_rt.asan.so-against-libatomic-when-nece.patch cefb73ec25a64f8c70cc0e585dfc688a23eb44b740093755ffaf8ee86f7e716e d47008f74357f06fa89e89aa04ac02242138568e
PASS 3 0003-Fix-armv7l-build-when-compiler-is-clang.patch 9ce436eb2f73e01b112cfeb10d6108e33c719b1db54a8fc404abdc10c367102b 3f0c4ed2b51658bc1585361385439d453458851d
PASS 4 0004-clang-add-support-for-LLVM-gold-plugin-in-case-of-ac.patch 45d68e84754f93ebe10ba80725e6cbb28826890e344f0ae910e061690105e944 4584830adae087a4fb6348b80da4c755699bae35
PASS 5 0005-Merge-MLGO-AOT-compilation-support-and-optimization-.patch da2cacae22ef8ececf808596b14ff53b6741db23e3f34d7fc5e07a75fa3543c2 bac21649416c0ea19ee8fa7ef00d4a7d8fa3dafa
PASS 6 0006-libcxxabi-declare-__gnu_unwind_frame-in-cxa_personal.patch f52129a2f008603a3cf74fc1bf448d984b0f1827a48a9597f346223d87b98062 ab69099b6f2dd679eeaf8b1dfe82bb13fdabc479
PASS 7 0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch b2d657339c18c5c1416bc9b3890afd39394b9ce46af061c1db809315ad4746d3 b00e455e21b89a23a7b13ce9b68103fbc9b28fb4
PASS source_tarball llvm-22.1.8.tar.gz 1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2
SUMMARY PASS
REPORT_PROVENANCE_BLOCK=PASS
DIFFCHECK_RC=0
```

字段之间的原始分隔符为制表符；Markdown 展示中以空格呈现。

命令退出码为 `0`。由此确认 `029` 的原始失败不是 whitespace 或 provenance
错误，而是后续精确短语断言与报告实际措辞不一致；修正版见 `030`。

## U06：补齐同步命令自身的四件套

`032` 在运行期间复制 raw 目录，因此复制瞬间它自己的 stdout/exitcode 尚未
关闭。完成后执行以下机械同步，使交付目录包含 `032` 的最终四件套和本
记录：

```bash
cp -a progress/R88/raw/032_sync_evidence_before_commit.command.txt progress/R88/raw/032_sync_evidence_before_commit.exitcode progress/R88/raw/032_sync_evidence_before_commit.stderr progress/R88/raw/032_sync_evidence_before_commit.stdout progress/R88/raw/UNLOGGED_RECOVERY_COMMANDS.md docs/progress/R88/raw/
```

输出：无。退出码：`0`。
