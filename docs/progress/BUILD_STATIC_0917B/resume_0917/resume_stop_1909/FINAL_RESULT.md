# 续跑结果：PARTIAL，输入准备受阻

固定窗口：2026-09-17 08:48:19 至 22:48:19 +08，未因暂停重置。暂停后于 18:52 核查恢复，约 19:08 因复制校验错误及未决门禁停止；未用满时间上限，不以超时解释本次停止。

| 项目 | 结果 | 直接证据 |
| --- | --- | --- |
| LLVM aarch64 | 完整 RPM 通过 | 14:17:21 最终 rpmbuild 返回 0；12 个 RPM 摘要通过；AARCH64_RPM_RESULT.md、verify_aarch64/result.json |
| LLVM armv7l | 整轮写包失败；18 个已写出 RPM 单独核查通过 | raw/202=1，cpio write failed / Function not implemented；raw/261=0，verify_armv7l/；不能把两种结果合并成整轮 PASS |
| armv7l libc++/libc++abi 静态开发 RPM | NOT_STARTED | 准备脚本不是执行证据 |
| bpftrace armv7l，libc++ / GCC / 未定义路径 | NOT_STARTED | LLVM 部分产物是否允许用作输入尚待确认 |
| bpftrace aarch64，libc++ / GCC / 未定义路径 | NOT_STARTED | 私有输入根复制校验失败，未使用该根启动构建 |
| armv7l 物理板取消 / 功能 / 安装测试 | NOT_STARTED | 只做过只读预检；未传输探针或安装包 |

## 暂停后的开工核查

raw/274–280：磁盘 227 GiB，medium 门禁退出 0，同一 I/O 探针退出 0；没有先前构建/复制进程。旧复制日志在 15:58:36 后没有 finished/exitcode，**退出码 NOT_OBSERVED**，未补造。

重启后 MemTotal=32297772 KiB，50% 按页向下取整为 **16536457216 字节**。旧值启动被保护断言拒绝（raw/279=1，未运行复制），修正后 raw/281 在实际 cgroup 中核验该值、nice19、idle。Ninja 并行度规则仍为 1；本次恢复后尚未启动 Ninja。

完整 du 结果与此前一致：需复制 1613406208 字节。其目录元数据读取较慢；两次额外同一 I/O 测试分别为 6.991 秒、1.311 秒，均返回 0，未触发超 30 秒的十分钟暂停规则。

## 新的阻断：复制工具读取差异

旧根 `usr/bin/llvm-opt-report`（40896 字节，0755/root:root）在 rsync 中读取返回 EPERM，更新被校验丢弃。停止已确认的进程组 13095，停止命令返回 0；监控子进程退出 -15，外层记录器退出 241。

受控单文件诊断（raw/292、copy_read_diagnostic.json、copy_rsync.strace、copy_cp.strace）表明：

- rsync 退出 23；openat 成功，随后的 read 返回 EPERM。
- cp 退出 0；普通源读取与复制件 SHA256 相同：`55629940b16d1ca7c100f17a41f3e1677f7ec3726449d9d8fa0f462f9b941a35`。
- 具体原因未查明。该结果不足以认定源文件损坏，也不足以认定可忽略 rsync 错误。没有改源权限、提权或自行切换整批复制方式。

## 需要人工确认

1. 是否允许只将已单独核验的 armv7l `llvm-static-devel` 作为后续输入，同时明确保留 LLVM 整体写包失败；或必须先排除完整写包错误。
2. 对同一文件 rsync/普通读取的权限结果差异，应先由环境侧核查，还是允许采用经完整哈希核对的其他复制方式。当前不擅自判定其性质。
3. 此前提出的 armv7l 原生工具五对象等价抽查尚未获答复；仍未切换。aarch64 的原生工具抽查证明仅限既有五目标，不外推到 armv7l。

## 保留的断点与边界

- LLVM aarch64：tmp/STATIC_0917B/rpm-aarch64/RPMS/aarch64/，完整通过的集合。
- LLVM armv7l：tmp/STATIC_0917B/rpm-arm-llvm/RPMS/armv7l/，18 个单独通过核验的文件，整轮失败不能抹去；旧中止集合另存，未混用。
- aarch64 部分复制：tmp/STATIC_0917B/bpf-input-aarch64/，**不完整，不可直接作为构建输入**。
- 单文件复制诊断：tmp/STATIC_0917B/rsync_read_probe/，只保留任务自己的诊断副本。
- 平台源码、配置、Source1002 未修改；经批准的 LLVM 构建树曾用于增量构建，并非声称构建目录未变化。本次私有根复制没有修改其来源 usr/RPM 数据库；没有包仓推送或新增 PC sudo。
- 板上此前 root 只读检查后已恢复 owner；本次暂停后没有新增板上操作。没有测试包或任务探针需要卸载。

**结论：候选尚不能按本任务门禁替换发布资产。** 未完成的完整 RPM、安装、物理板验证都如实保留，不以已有候选构建、宏展开或准备脚本代替。
