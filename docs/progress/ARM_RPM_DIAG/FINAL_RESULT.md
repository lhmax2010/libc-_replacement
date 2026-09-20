# LLVM armv7l 写包失败诊断

## 结论

**PARTIAL：历史故障确认存在，但本次单轮复现没有失败；根因仍为 `NOT_OBSERVED`。没有修复、绕过或推送。**

本轮仅执行一次 LLVM armv7l libc++ 整轮写包：2026-09-18 19:58:55 至 20:53:41（约 54 分 46 秒），`rpmbuild` 返回 **0**，实际写出 **22 个 RPM**；逐个 `rpm -K --nosignature` 的摘要检查均返回 0。这不等于历史问题已经解决，也不自动改变此前产物的审批状态。新 RPM 仅保留为诊断产物，未替换后续任务的输入。

第 19 个包是 **`lldb-debuginfo-22.1.8-1.armv7l.rpm`**：本轮创建事件、实际写包顺序及头部均已直接核实。旧轮失败 trace 的末端也出现该输出路径。但旧 trace 没有保留成功调用及失败 RPM 头部，**旧轮内部究竟哪次归档操作失败，仍未观测**。

不能依据报错中的 `Function not implemented`，直接认定某次宿主 `write/mknod/link/setxattr/fallocate` 返回了 ENOSYS。本轮宿主所跟踪调用未出现这类失败；客体中确实有 `statx` 返回 ENOSYS，随后回退 `fstatat64` 成功。RPM 的错误格式化还存在读取当时 errno 的路径，详见下文。**尚未证明旧轮的 ENOSYS 是这一回退调用留下的 errno，也未证明是文件系统不支持某种写操作。**

## 1. 历次现象：不是同一种失败

| 记录 | 实际输出与退出码 | 阶段 |
| --- | --- | --- |
| 9 月 17 日 LLVM | 写出 18 个后 `error: create archive failed: cpio: write failed - Function not implemented`；退出 1 | 归档写包 |
| 9 月 18 日 LLVM | 同样 18 个、同样末端文本；退出 1 | 归档写包 |
| 9 月 18 日 bpftrace armv7l/libc++ | `resolve_imports.cpp.o` 附近、约 88%，受时间窗口终止；子进程 -15，记录器 241 | 编译阶段，未进入写包 |
| 此前 rsync | `read(3, ..., 40896) = -1 EPERM`，rsync 退出 23 | 复制 `llvm-opt-report`，不是 RPM 归档 |

两次 LLVM 末端原文：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
error: create archive failed: cpio: write failed - Function not implemented
```

原始出处：

- `progress/BUILD_STATIC_0917B/resume_0917/cells/llvm-arm-rpm-serial/build.log:7542` 起 18 条 Wrote，7560 行错误；raw/202。
- `progress/BUILD_WEEKEND_0918/cells/llvm-arm-whole-retry/build.log:7542` 起同序列，7560 行错误；raw/432。
- 旧轮 trace `progress/BUILD_WEEKEND_0918/llvm-arm-rpm-failed-syscalls.strace:3171491` 明确出现 `lldb-debuginfo-22.1.8-1.armv7l.rpm`；本轮摘录见 raw/216、219。
- rsync：`progress/BUILD_STATIC_0917B/resume_0917/copy_rsync.strace:155`–156；先 open 成功再 read EPERM。此前 `cp -a` 成功并校验一致，**尚未查明为何两次读取结果不同**。
- bpftrace：`progress/BUILD_WEEKEND_0918/cells/bpf-armv7l-libcxx/build.log` 与 events.jsonl 末尾，raw/355。

两次旧 LLVM 日志 7086–7105 行还有 SOURCES 下 spec 无法打开的检查器错误，之后检查器打印 SUCCESS 并继续写出 18 个包；本轮也未修改该检查器。不能将它直接认作最后归档错误的原因。

## 2. 单次复现的条件及限制

原 spec SHA256：`cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`。继续使用此前已准备的 BUILD 树，执行 `--noprep --noclean -bb`，不是从空树全量编译。本轮构建日志记录 **96 个增量目标**；没有声称所有中间状态与旧失败瞬间逐字节相同。

保留原 `_binary_payload w5T1.xzdio`、`_build_create_debug 1`、目标架构、RPM 宏、spec、串行构建参数。完整 argv 在 `cell_commands/llvm-arm-repro.json`，命令原文及退出码在 raw/044、045；构建各阶段完整输出在 `cells/llvm-arm-repro/build.log`。

诊断性差异如实列出：

1. 新输出及 BUILDROOT 改到 `tmp/ARM_RPM_DIAG/rpm-arm-llvm/`，旧 `tmp/WEEKEND_0918/` 只读绑定，避免覆盖原证据。**这不是在完全相同输出目录上进行的单变量实验。**
2. 增加 `-vv`，宿主 `strace`，并显式用构建根内同一 QEMU 二进制增加 `-strace -D`。原入口是 binfmt；本轮入口显式指定。未证明这些诊断差异对时序/地址空间完全没有影响。
3. 宿主跟踪含用户指定五类调用及 *at 变体，同时保留 open/read/close 上下文；read/write 缓冲区记地址而非展开全部载荷，返回值仍保留。
4. 新建只读文件描述符观察 RPM 创建/关闭/删除，以防失败输出被自动移除。实际没有发生失败删除；头部前缀不是完整 RPM，**不作为可安装产物**。

未改变压缩方式、关闭 debug 子包、改 spec、修源码、放宽权限或重跑第二轮。本次成功**不能单独归因为新目录、96 个增量目标、日志级别或 QEMU 入口差异中的任一项**。

资源实际落实：`ninja -j1`，nice 19，ionice idle，cgroup `memory.max=16536457216` 字节（机器总内存的 50%），实际进程核验见 events.jsonl。开工可用空间约 204 GiB；读写/fsync/校验探针 0.105293 秒，后续 0.110699、0.124036 秒，均未触发暂停。medium 资源门禁退出 0。固定六小时截止为次日 01:46:15，本轮未延长。

## 3. 第 19 个包：文件及特殊属性

完整 16 条记录在 `package-19-file-list.tsv`；逐条新旧文件属性、SHA256 在 `package-19-metadata.json`。这是**本轮成功包的清单**，不是已恢复的旧失败 RPM 头部。

它含 11 个目录和 5 个普通文件：

| 普通文件（包内绝对路径） | 字节数 | 与旧失败轮保留同路径文件 SHA256 |
| --- | ---: | --- |
| `/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/bin/lldb-argdumper.debug` | 224280 | 相同 |
| `/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/bin/lldb-server.debug` | 94576720 | 相同 |
| `/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/bin/lldb.debug` | 7594696 | 相同 |
| `/usr/lib/debug/home/owner/share/tmp/sdk_tools/lldb/lib/liblldb.so.22.1.8.debug` | 232318436 | 相同 |
| `/usr/lib/debug/usr/bin/lldb-mcp.debug` | 9954168 | 相同 |

这 5 个文件实际均 uid/gid 1000/1000、0444、nlink=1；RPM 头部打包 mode=0644，二者分别记录，不混为同一权限。目录实际 0755。全部所查条目 xattr 列表为空；普通文件无稀疏分配特征。没有设备节点、FIFO、符号链接、普通文件硬链接组、setuid/setgid/sticky 位。最大包内路径 78 字节。

与前 18 个对照（`package-feature-comparison.tsv` 覆盖本轮全部 22 包，`first-18-rpm-files.tsv` 为旧轮已写出包）：

- 前 18 包已有符号链接；第 19 个反而没有。
- 第 2 包最大路径 137 字节，长于第 19 个的 78 字节。
- 第 16 包最大文件 750048160 字节，大于第 19 个最大文件 232318436 字节。
- 第 19 个不是首个包含目录或大型调试文件的包。
- 旧 BUILDROOT 全量元数据扫描也没有设备/FIFO/socket、稀疏文件或特殊权限位，见 `historical-metadata-summary.json`。这只是保留现场的扫描，不替代失败瞬间记录。

**未找到“前 18 个没有、第 19 个独有，且已证明触发失败”的特殊文件属性。** 两个位置的 liblldb debug 文件分别属于 lldb-debuginfo 与 lldb-devel-debuginfo，不能因旧 BUILDROOT 同时含两份而说第 19 包重复收入两份。

本轮未失败，因此“失败时立即 ls -la”的触发条件没有出现，记 `NOT_OBSERVED`；不能拿结束后的目录状态冒充失败瞬间。已配置的自动取证分支保留于 guard.py，实际未触发。

## 4. 文件系统、隔离环境和 aarch64 对照

完整证据和可比性说明见 `ENVIRONMENT.md`、raw/038、048、073–077、121–125、136、139。

| 项目 | 实际核查 |
| --- | --- |
| 文件系统 | armv7l 新旧根/输出与 aarch64 根/输出均在 `/dev/sda1` ext4，宿主 `rw,relatime`，**不是不同文件系统** |
| 隔离 | bubblewrap 用户/挂载命名空间；根只读、指定 BUILD/输出可写；实际 guest rpmbuild 进程 Seccomp=0、filters=0、NoNewPrivs=1 |
| xattr | setfattr/getfattr 不可用；客体 Python 等价 set/get/list 用户属性实测通过，临时探针文件已删除；不泛化到 security/trusted 命名空间 |
| 权限 | 所查输出路径/第 19 包文件属当前用户，可遍历/读写；没有该处被 abuild 所有权阻断的直接证据 |
| 磁盘 | 开工空间与 inode 门禁通过，探针通过；限定内核日志查询无条目不等于证明历史无磁盘错误 |
| ARM RPM | 32 位 rpm/rpm-build 4.14.1.1-1.5，xz 5T1，debug 子包启用 |
| aarch64 RPM | 64 位 rpm/rpm-build 4.14.1.1-6.6，实际 RPM 头部为 zstd 19；成功 12 包不含独立 debuginfo/debugsource |

两套 RPM 的 VCS 都指向 `platform/upstream/rpm#355231365d01b3fb7979a82a429f6b95c5e95993`。同源不等于同位宽、同压缩/子包配置。因此 aarch64 成功不能单独排除 ARM 归档栈问题，也不能认定文件系统差异是原因。

## 5. ENOSYS 的实际证据与源码边界

宿主 trace：`host.strace`，共 25159499 行；客体 trace：`tmp/ARM_RPM_DIAG/guest-rpmbuild.strace`，共 1926506 行。`trace-audit.json` 与 `host/guest-error-calls.tsv` 是粗检索引，不是完整系统调用解析器。

宿主**所跟踪调用**中未检出 ENOSYS，以及选定写入类调用返回 -1 的记录。不能由此声称未跟踪调用或历史执行也没有问题。

客体实际原文（1926430–1926431 行）：

```text
386378 statx(4,"",AT_EMPTY_PATH|AT_NO_AUTOMOUNT|AT_STATX_SYNC_AS_STAT,STATX_BASIC_STATS,0x407ff020) = -1 errno=38 (Function not implemented)
386378 fstatat64(4,"",0x407ff020,0x1000) = 0
```

对应前一行打开 `/usr/lib/rpm/fileattrs/pkgconfig.attr`，之后继续读文件。类似 ENOSYS 在启动阶段已出现，并非到第 19 个包才开始；另有 `set_robust_list` 不支持记录。**这些调用没有导致本轮 rpmbuild 失败。致命 ENOSYS 调用及参数：`NOT_OBSERVED`。**

客体 trace 存在多进程写同一行的交错。粗检的 22 条“含 write 和失败文本”候选全部含多个 PID/调用片段，不能把另一调用的 errno 配给 write。逐条保存在 `guest-interleaving-review.json`；不将 22 报为失败 write 次数，也不将 ENOSYS 匹配行数冒充准确调用次数。

已只读取得实际 RPM VCS 的源码，无工作树修改。以下是固定提交实际代码，而非仅拿近似上游版本推论：

`lib/cpio.c:224`：

```c
if (cpio->fileend != cpio->offset) {
    return RPMERR_WRITE_FAILED;
}
```

这条路径不发起写系统调用，也不设 errno。`lib/cpio.c:172` 的 trailer 检查有同类返回路径。`lib/rpmfi.c:2343,2354,2388`：

```c
int myerrno = errno;
case RPMERR_WRITE_FAILED: s = "write"; break;
if ((rc <= RPMERR_CHECK_ERRNO) && myerrno) {
    rstrscat(&msg, _(" failed - "), strerror(myerrno), NULL);
}
```

`build/pack.c:86`–92 将其组合为 `create archive failed`。以上证明**该错误文字本身不足以定位系统调用**；不证明旧轮一定命中了 fileend 检查，更不证明其 errno 一定来自 statx。原文全文见 raw/107、110–112；带行号摘录见 raw/187、189。

## 6. 分类判断、缺口与后续裁决

| 类别 | 目前能够确认 | 根因结论 |
| --- | --- | --- |
| 权限/安全机制 | 本轮实际进程无 Seccomp，所查目录属当前用户，xattr 探测通过；未使用 sudo | 未证明为旧归档失败根因；失败瞬间状态 NOT_OBSERVED |
| 磁盘/inode/I/O | 本轮空间和探针通过；同一 ext4；22 包摘要通过 | 旧轮磁盘故障根因 NOT_OBSERVED；rsync EPERM 另行保留 |
| 构建配置/sysroot/依赖 | 两次旧 LLVM 到归档阶段；本次同 spec 成功；96 个增量目标及新输出路径已申报 | 不支持将 bpftrace 编译中止合并为同一根因；未证明具体配置缺陷 |
| RPM 宏/文件列表/归档库 | 失败阶段已定位；第 19 包路径确认；5 个文件内容与旧轮一致；内部错误可附加 errno | **归档内部的具体失败分支 NOT_OBSERVED，尚无已验证修法** |

尚缺：能再次失败的现场中，`rpmPackageFilesArchive` 的实际返回码、`rpmcpioHeaderWrite/TrailerWrite` 的 fileend/offset、压缩层返回值及当时 errno。旧日志/失败调用筛选 trace 不具备这些信息；本轮成功无法补出失败状态。

若继续诊断，需要人工确认下一轮是否在原输出路径和原 binfmt 入口恢复失败条件，并做库级返回值/调用栈取证；应先保护现有现场，不能把换目录、换压缩器、关调试包当成已证实修复。**本轮没有实施这些操作，也没有擅自进行第二次整轮复现。**

## 7. 自行判断与交付索引

自行判断：为保护旧证据采用新输出目录；为覆盖 QEMU 客体增加客体 trace；为避免错误归因读取部署 RPM 的固定源码；采用只读头部观察器；以可执行元数据/实际调用而非包顺序猜测归因。上述方法的差异及局限均已列出。没有选定修法，没有改判定，没有继续 bpftrace 或板上操作。

- 完整命令、输出、退出码：`raw/`；复现 argv：`cell_commands/llvm-arm-repro.json`。
- 历史摘录：raw/013、017、023、024、026、033、207、208、216、219；`INTERIM_RESULT.md` 是早期确认前记录，**本报告为最终状态**。
- 22 RPM、SHA256、摘要校验：`output-manifest.tsv`、`output-01.json` 至 `output-22.json`，raw/215。
- 包特征对照/第 19 包：`package-feature-comparison.tsv`、`package-19-file-list.tsv`、`package-19-metadata.json`。
- 追踪及检索限制：`trace-audit.json`、`guest-interleaving-review.json`；raw/213、218–221。
- 脚本快照：`script-snapshots/` 为启动阶段快照；最终追加脚本/版本在 `final-script-snapshots/`，对应 SHA256 清单。
- 没有改 codes/spec/系统配置，没有提权、没有推送。构建生成文件及诊断文件仅落既有可写 BUILD 和本任务输出位置。未删除旧产物。技术性路径查询失败与后续纠正保留于 raw，不冒充构建失败。

**停止于此，交人工审阅；不将“本轮成功”写成“根因查明”。**
