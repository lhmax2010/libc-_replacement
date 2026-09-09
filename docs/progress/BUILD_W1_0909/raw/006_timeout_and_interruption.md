# 时间上限、中止与远端核对

## aarch64 与 x86_64 GCC / 非 libc++ 构建

aarch64 实际命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A aarch64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-aarch64 -C --fail-fast --threads 1 --define '_smp_mflags -j1' --define 'mlgo_build_jobs 1' --release 109.3 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

x86_64 首次调用原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A x86_64 -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-x86_64 -C --fail-fast --threads 1 --define '_smp_mflags -j1' --define 'mlgo_build_jobs 1' --release 109.3 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

该调用在进入包构建前退出 `1`；原因是两个 GBS export 同时操作同一
worktree，x86_64 遇到临时 `gbp_index.lock`。aarch64 export 完成后锁正常
消失。修正为保留相同参数、重新调用 x86_64；不是修改源码、spec 或判据。
重试命令与上面的 x86_64 命令逐字相同。

两次有效构建均观测到 GNU 14.2 编译器，且配置命令未含
`-stdlib=libc++`、`-lc++abi` 或 `LLVM_ENABLE_LIBCXX=ON`。

## 到达八小时时间上限后的处置

W1 在约 19:38 到达八小时时间上限时：

- aarch64 到达 4,951/7,546；中止落盘后的末项为 4,956/7,546；
- x86_64 重试到达 5,448/7,634；中止落盘后的末项为 5,450/7,634；
- 两者均未到写包与最终产物核查阶段，最终 GBS 退出码因终止进程组而
  `NOT_OBSERVED`。

先向交互会话发送中断，再向顶层进程组发送信号；由于构建根内包装进程
由 root 持有，顶层退出后仍有本用户持有的 rpmbuild/ninja 子进程。最终
解析并终止两个明确的构建进程组。19:40:54 复查，未留下 gbs、rpmbuild、
ninja 或编译器进程。中止清理比时间点多用约 2 分 54 秒；该时间仅用于
停止进程与核验，没有继续验证。

日志落盘结果：

| 日志 | 原始大小 | 原始 SHA256 |
| --- | ---: | --- |
| armv7l 完整构建 | 10,376,351 | `259de8c69e5338d3fbec45dfd38c90a93c5086083cbe2aa6569b1c0daeca6940` |
| aarch64 部分构建 | 1,050,823 | `6ac99d7fda4cbae9d0c4adf26ba7c19318567cac4dbe3a5d05f2e1d1f4b4611d` |
| x86_64 首次锁冲突 | 2,614 | `b3a1796603939a43286bae455693f0d0161ad6648ec89d6fb781f0267b436253` |
| x86_64 重试部分构建 | 1,260,556 | `91fefd41b4f96275025fcb2a30d806933f4913c4dea33a91d3ac698777842e15` |

完整输出以确定性 gzip 形式归档在 `build_logs/`，压缩文件哈希另见
`build_logs/SHA256SUMS`。

## 工作树与远端分支

进程复查命令原文：

```bash
ps -eo pid,ppid,pgid,stat,comm,args | rg 'gbs|rpmbuild|ninja|clang|gcc' | rg -v 'rg ' || true
```

输出为空，命令组退出码 `0`。

远端核对命令原文：

```bash
git -C tmp/BUILD_W1_0908/llvm-worktree ls-remote origin refs/heads/tizen_base refs/heads/sandbox/lhmax2025/libcxx-migration
```

输出与退出码：

```text
2d23367d74afbf2bb1e9e4013fce072b3a154109 refs/heads/sandbox/lhmax2025/libcxx-migration
2d23367d74afbf2bb1e9e4013fce072b3a154109 refs/heads/tizen_base
remote_rc=0
```

候选工作树 HEAD 仍为 `2d23367d74afbf2bb1e9e4013fce072b3a154109`；
spec 与三份新资产仍是未提交候选。构建还额外生成了未跟踪
`llvm.manifest`，未将其纳入候选或任何提交。没有 package commit，
没有 package push。
