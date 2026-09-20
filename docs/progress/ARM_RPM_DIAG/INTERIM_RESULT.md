# armv7l 写包失败诊断：复现前核查

状态：**PARTIAL，等待确认单次复现的包；尚未启动本轮构建。**
本页不是根因已查明报告。六小时窗口仍按 `START.md`，不因等待自动延长。

## 已核实的失败历史

| 执行 | 原始记录 | 实际结果 | 失败阶段与限制 |
| --- | --- | --- | --- |
| 9 月 17 日 LLVM | `progress/BUILD_STATIC_0917B/resume_0917/cells/llvm-arm-rpm-serial/build.log:7542–7560`；同目录 raw/202 | 15:00:14 开始，15:47:27 退出 1；写出 18 个 RPM 后失败 | `%build`、`%install`、文件处理已执行；归档阶段报错。具体下一份包及文件未由原日志指明 |
| 9 月 18 日 LLVM | `progress/BUILD_WEEKEND_0918/cells/llvm-arm-whole-retry/build.log:7542–7560`；raw/432 | 18:21:57 开始，19:15:29 退出 1；同样写出 18 个 RPM | 错误位置、文本及最后一个成功包一致，不等于已证明失败文件相同 |
| 9 月 18 日 bpftrace / armv7l / libc++ | `progress/BUILD_WEEKEND_0918/cells/bpf-armv7l-libcxx/build.log` 末尾、`events.jsonl` 末尾；raw/355 | 18:15:54 受控终止；子进程 -15，外层记录器 241 | 主包编译约 88%，`resolve_imports.cpp.o`；尚未进入 `%install` 或写包。不能记成已复现的 RPM 归档错误 |
| 较早 rsync 复制 | `progress/BUILD_STATIC_0917B/resume_0917/copy_rsync.strace:155–156` | `openat(... "llvm-opt-report", O_RDONLY|O_NOFOLLOW) = 3`；随后 `read(3, ..., 40896) = -1 EPERM` | 发生在输入复制阶段，不是上述 RPM 归档错误；为何该次 read 被拒仍未查明 |

两次 LLVM 的实际末端输出均为：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
error: create archive failed: cpio: write failed - Function not implemented
```

两次 LLVM 还都在原日志 7086–7105 行出现 `Unable to open /home/abuild/rpmbuild/SOURCES/llvm.spec` 等检查器查询错误。检查器随后打印 SUCCESS，流程继续写包；该输入/检查缺口应登记，但目前不能把它当作最终归档错误的原因。本轮未修改检查器、spec 或输入路径。

本轮重读的精确摘录在 raw/013、017、023、024、026、033；不是重新生成的原日志。覆盖的是上述已定位原始执行，不把报告中的概括当作额外失败次数。

## 环境分类：当前能够和不能够证明什么

| 类别 | 观测 | 当前判断 |
| --- | --- | --- |
| 权限、所有权、挂载 | raw/012：`/home` 为 ext4、`rw,relatime`；raw/014、030：已检查的 RPM 输出目录及 LLDB debug 文件路径属主均为 toolchain，目录可遍历；当前 `/sys/fs/selinux` 不存在 | 未见所查路径被 abuild 所有权挡住的直接证据；失败瞬间状态尚未采集，不能排除其他路径或其他安全机制 |
| 磁盘、inode、I/O | 开工可用 219197784064 字节；inode 使用率 28%；同一 3932160 字节读写/fsync/校验探针 0.105293 秒，返回 0；medium 门禁返回 0 | 当前容量和探测通过，不倒推历史所有 I/O 均正常；内核日志限定时间窗/模式查询没有条目，不等于绝对不存在磁盘错误 |
| 编译、sysroot、依赖 | LLVM 日志已经进入写包并写出 18 个包；bpftrace 日志因窗口停止在编译阶段 | 尚无证据证明两者具有同一个交叉编译/依赖根因；bpftrace 完整写包结果为 NOT_OBSERVED |
| RPM、宏、文件清单、归档层 | 两次 LLVM 同位置 `cpio: write failed - Function not implemented`；实际 ARM 根为 rpm/rpm-build 4.14.1.1-1.5、xz/liblzma 5.8.1-1.9、glibc 2.40-1.10（raw/025） | **已定位故障阶段，未定位根因。** 需要单次带诊断的复现；不能仅凭文字断言宿主 write 系统调用返回 ENOSYS |

raw/009 对两份 LLVM BUILDROOT 的常规文件查询没有大于 2 GiB 的条目；未据此认定归档总量、地址空间或压缩器一定没有问题。
上一轮只跟踪选定的宿主失败系统调用，不能覆盖来宾或 RPM 库内部全部错误路径。本轮尚未新增失败瞬间跟踪。

## 复现待确认

任务同时提到 LLVM 与 bpftrace，而第 2 项要求“单独复现一次”。已提出问题：本次复现是 **LLVM armv7l libc++ 整轮写包**，还是 **bpftrace armv7l libc++ 路径**？
在确认前不擅自选择、不跑两轮代替一轮、不修复或绕过错误。失败瞬间的 `ls -la`、属主权限记录，以及最终根因和修法目前均为 **NOT_OBSERVED**。

## 本轮操作和限制

- 只做历史日志、环境和构建根 RPM 身份的只读核查；`root_query.py` 将复用命令中的全部绑定改为只读后查询包数据库，没有运行构建阶段。
- 未修改 codes、spec、系统配置或既有记录；未使用 sudo，未推送任何仓库。
- raw/021 尝试查找 `.so.9` 返回 2；随后 raw/022 通过实际目录确认该根使用 `.so.3`。这只是诊断路径错误，不是新的构建故障。
- raw/018 是一个组合 shell 环境检查，其最后返回 0 不代表每个子命令成功；`/sys/fs/selinux` 不存在的 stderr 已保留，不把它当成 `getenforce` 已执行。
- 为准备归档层定位，取得上游 rpm-4.14.1 的 `rpmio.c`、`cpio.c`，固定提交 `bfee1410af51c1cc9724791fb8d985260a62102b`，下载命令与返回码见 raw/031–032。**它们不是实际部署的 Tizen 4.14.1.1 源码，不据此认定本机根因。** 最初尝试不存在的 4.14.1.1 上游 tag 返回 404，之后用远端 tag 列表核实（raw/028）。
- 尚无经实证的修法，因此本轮没有实施任何修正。
