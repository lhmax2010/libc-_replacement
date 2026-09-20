# 复现环境及与 aarch64 的可比性

## 本次复现

19:58:55 启动一次 LLVM armv7l 完整 `rpmbuild --noprep --noclean -bb`，保留原准备树，使用原 spec，不声明从空目录全量编译。截止仍为次日 01:46:15。

- 完整命令：`raw/044_prepare.stdout.txt`、`cell_commands/llvm-arm-repro.json`。
- 输入 spec、串行 Ninja 包装器、QEMU 二进制的 SHA256：`repro-inputs.json`。
- 只改变诊断输出位置、增加 `-vv` 和两层 trace；不改压缩格式、debug 包策略、文件清单或依赖。
- `tmp/WEEKEND_0918` 在本轮只读绑定，新 RPM/BUILDROOT/TMP 在 `tmp/ARM_RPM_DIAG/`，不覆盖此前 18 个 RPM。
- QEMU 使用同一个构建根的 `/usr/bin/qemu-arm`；原 binfmt 指向 `/usr/bin/qemu-arm-static`，该根内该路径是 `qemu-arm` 的符号链接。显式增加 `-strace -D` 仅记录来宾 rpmbuild 的系统调用。版本 9.2.3，见 raw/036、040、042、056。
- 宿主 `strace -f` 包含要求的 write/mknod/link/setxattr/fallocate，另含其 *at 变体及 open/read/close。读写缓冲区以地址记录而非展开文件正文，不影响返回值记录。见 raw/044。
- 资源：`arm-rpm-diag.scope` 的实际 `memory.max=16536457216`；Ninja PID 387793 实际命令含 `-j1 -j 1`，nice 19、ionice idle、同一 cgroup，见 cells/llvm-arm-repro/events.jsonl。
- 全量构建输出：`cells/llvm-arm-repro/build.log`；宿主 trace：`host.strace`；来宾 trace：`tmp/ARM_RPM_DIAG/guest-rpmbuild.strace`。
- 只读头部观察器留存新输出 RPM 的前 1 MiB，便于 RPM 自动删除失败输出后仍能查询文件清单。这些文件标记为 header-prefix，**不是可安装 RPM**；不更改被观察文件，见 `watch_headers.py`、raw/119。

## 文件系统、容器与扩展属性

`df -T` 和 findmnt 已确认 armv7l 构建根、armv7l 输出、aarch64 构建根与 aarch64 输出均位于 `/dev/sda1`、ext4、宿主 `rw,relatime`。因此“两个构建根在不同文件系统”不成立（raw/038、077）。

使用 bubblewrap 用户/挂载命名空间；不是在宿主根直接 rpmbuild。只读查询环境里的 `Seccomp: 0`、`Seccomp_filters: 0`（raw/048）。诊断输出绑定为 ext4 `rw,nosuid,nodev,relatime`；环境探测为了只读安全将 BUILD 绑定成 ro，这一点与实际构建的 BUILD rw 不同，不能把探测 mount 输出冒充构建全部挂载状态。

环境无 `setfattr/getfattr` 命令，因此用来宾 Python `os.setxattr/os.getxattr/os.listxattr` 等价探测：在 `/tmp/diag` 新建独占临时文件，写入 `user.arm_rpm_diag=probe`，读回完全一致，返回 0，最后只删除该临时文件。证明 **user 命名空间 xattr 在所测路径可用**；不泛化为任意 security/trusted 命名空间都允许。完整调用和输出见 `environment-2.json`、raw/048。

## aarch64 不是同条件对照

| 项目 | armv7l 失败轮/本轮 | aarch64 已成功轮 |
| --- | --- | --- |
| RPM 程序 | ARM 32 位 | ARM aarch64 64 位 |
| 文件系统 | 同一 /dev/sda1 ext4 | 同一 /dev/sda1 ext4 |
| debug 包 | 明确 `_build_create_debug 1`，产生 debuginfo/debugsource 子包 | 实际 12 个 RPM，无独立 debuginfo/debugsource |
| 载荷压缩（读取已有 lldb RPM 头） | xz，5T1 | zstd，19 |
| 编译器入口 | 原 armv7l 构建树/工具集 | 经此前抽查批准的 /emul 原生交叉入口 |

证据：raw/073–077、121–125。上述差异是已观测的可比性限制，不直接等同于根因，更不通过改压缩器或关 debug 包来绕过本轮复现。

## RPM 源码身份

实际安装 RPM 的 VCS 字段均为 `platform/upstream/rpm#355231365d01b3fb7979a82a429f6b95c5e95993`（raw/066）。已只读 fetch 该提交至 `tmp/ARM_RPM_DIAG/rpm-source.git`（raw/102），没有工作树、没有改平台源码。

固定提交的实际源码分别保存在 raw/107（rpmio/rpmio.c）、110（build/pack.c）、111（lib/cpio.c）、112（lib/rpmfi.c）。源码显示 `RPMERR_WRITE_FAILED` 可由库内部检查返回，错误格式化函数会读取 errno；因此 ENOSYS 文本不能单独证明某个宿主写入系统调用失败。本轮最终结论仍须结合实际 trace。

此前 HTTP 取得精确源码的尝试有超时、403、旧 SRPM 404；它们是取证方法的失败，不是本次 rpmbuild 故障。随后 SSH 只读 fetch 成功，未使用 sudo 或密码文件。
