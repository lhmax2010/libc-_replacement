# 低资源续跑记录

## 结论与待确认事项

**PARTIAL。本轮没有新增通过格，没有包提交或包推送。**

发生了本次操作引入的续跑故障：直接进入旧构建根执行 rpmbuild 时，
编译器查找路径与原缓存不同，CMake 重置了缓存并丢失部分配置。
发现后已停止该次构建，并恢复原路径与完整配置参数。配置值复核通过，
但 Ninja 预演的待处理目标从 6346 增为 7146。不能再声称保持了原
801 个目标的全部复用，也不能把该次错误配置下的构建计为验证。

**需要人工确认：是否允许在现有 armv7l 构建树上，按已恢复的正确配置
继续执行 Ninja 必要的重编（可能涉及原来已完成的目标）？**
本轮要求明确是从 801/7147 断点继续，故未自行转为重新完整构建。
该停止不是 I/O 超时的停止，也不是 bcc-tools 源码失败。

## 开工与资源限制

开始时间约为 2026-09-15 15:08 +08，设定硬截止为次日 03:08 +08。
本轮在到达时限前因上述待裁决事项停止，没有跑满十二小时。

- `raw/001_resource_gate.*`：medium 检查退出 0。
- `raw/002_resume_inspection.*`、`raw/005_*`：原构建树存在，
  801/7147 断点对应 dry-run 剩余 6346，未清空构建根。
- 仅启动 armv7l LLVM；没有重建 aarch64/x86_64 LLVM。
- jobs=1，nice=19，ionice class=3。构建日志显示 `ninja -j 1`。
- 使用 `systemd-run --user --scope -p MemoryMax=16536451072`：
  MemTotal 为 32297756 KiB，一半为 16536451072 字节；内核实际
  `memory.max=16536449024`，按页取整，比请求值少 2048 字节。
  不是只给外层脚本设置限制：事件记录核对 GBS、QEMU、rpmbuild
  及构建子进程属于同一 scope，nice 均为 19。

## I/O 暂停控制

监控脚本约定每 500 个构建目标执行小规模写入、fsync、读回比对。
探针写入量为 8912896 字节。超过 30 秒冻结构建 scope，等待
600 秒复测；三次暂停后仍不通过才停止。没有放宽用户阈值。

本次有效编译未达到 500 目标，**实际构建中每 500 目标探测及三轮
暂停重试尚未触发，记 NOT_OBSERVED**。不得把脚本具备该逻辑说成
这些分支均已实测。独立 freezer 校准显示 frozen=1，恢复调用退出 0。

`systemctl --user --no-block freeze` 曾触发客户端断言（退出 -6），
已改用不带 --no-block 的调用，独立冻结/恢复校准通过。初次清理时
对含 root 进程的 scope 整体 kill 返回 Access denied；后续只对
该 scope 中本用户的构建进程发信号并恢复冻结，不提权、不触碰其他任务。
错误与修正见 raw/008–009、014–016。

## 续跑失败的直接证据

### 进入方式

GBS incremental 的现有实现会把源码工作树 bind 到构建目录；为保留
当前编译输出，选用 `gbs chroot` 后运行原导出 spec 的
`rpmbuild --noprep --noclean -ba`，跳过重新解包，并将并行宏设为 1。
该选择是本轮自行作出的实现判断，未预先充分核对 PATH 与 CMake 缓存
的关系，导致下述故障。

### 配置重置

原缓存（raw/002）：

```text
CMAKE_CXX_COMPILER:STRING=/bin/armv7l-tizen-linux-gnueabi-clang++
CMAKE_C_COMPILER:STRING=/bin/armv7l-tizen-linux-gnueabi-clang
LLVM_ENABLE_LIBCXX:BOOL=ON
```

本次重置日志（`build-w2c-llvm-arm-retry.build.log`，raw/013 摘录）：

```text
You have changed variables that require your cache to be deleted.
Configure will be re-run and you may have to reset some variables.
```

后续缓存变为 `/usr/bin/...-clang`、Release、LLVM_ENABLE_LIBCXX=OFF、
LLVM_ENABLE_PROJECTS 为空，Ninja 总数变为 3993。发现后立即 freeze，
停止本用户构建进程并 thaw。外层退出 -15，未出现正常内层完成标记。
该次构建的任何对象都不是通过证据；没有写出可交付 LLVM RPM。

### 恢复及其局限

恢复脚本取自本次 rpmbuild 生成的实际构建脚本，截取 Ninja 之前的
配置部分，显式恢复原 `/bin` 编译器路径，并固定 PATH。两遍配置
完成，复核以下值（raw/026、`build-w2c-config-check2.build.log`）：

```text
CMAKE_BUILD_TYPE:STRING=MinSizeRel
LLVM_ENABLE_LIBCXX:BOOL=ON
LLVM_TARGETS_TO_BUILD:STRING=ARM;BPF
LLVM_ENABLE_PROJECTS:STRING=clang;lldb;clang-tools-extra;lld;compiler-rt;openmp
```

编译器值为原 `/bin` 路径；缓存字段类型现为 UNINITIALIZED，非原 STRING。
首次自检因将类型固定为 STRING 而失败，改为核对字段名和精确值后通过。

恢复后 dry-run 最后一项为 `[7146/7146]`。针对原来已完成的
IVUsers.cpp.o 作独立 `ninja -d explain -n`，实际输出指出
`build/include/llvm/Config/llvm-config.h` 比若干对象更新，生成工具
和生成头文件的依赖链也变 dirty。完整解释见 `build-w2c-explain.build.log`。
**dry-run 会保守列出生成依赖，不能据 7146 宣称实际必定全部重编**；
但没有足够证据保证复用原 801 项，故未继续编译，也没有伪造时间戳
或修改 Ninja 依赖记录来强迫复用。

## 其他技术性失败与修正

1. 裸 chroot 中 /proc 未挂载，初次在内部读 cgroup 失败；改在宿主机
   核对实际子进程 cgroup，没有据外层成功假定内层成功。
2. 已确认 gbs chroot 不可靠传递内部命令退出码，因此脚本正常完成时
   显式输出 W2C_INNER_EXIT；缺标记不算成功。
3. 配置恢复首试因原 RPM 脚本引用未定义 PKG_CONFIG_PATH，新增的
   set -u 报错。恢复为与原脚本一致的 set -e 后重跑；未修改 spec。
4. 短命令可能在 scope 观测前退出，监控器随后增加了读取真实内层
   标记的分支；该分支不声称已观测到内存控制组。
5. 监控器后续版增加“观测到 CMake cache reset 即停止”门禁；此版
   未用于重新编译，不能冒充前一次执行所用版本。

## 门禁矩阵与后续依赖

| 包 | 架构 | libc++ | GCC | 来源/限制 |
| --- | --- | --- | --- | --- |
| bcc-tools | armv7l | NOT_OBSERVED | NOT_OBSERVED | LLVM 输入未完成；各 5 次核查均未执行 |
| bcc-tools | aarch64 | PASS | PASS | 沿用 ../BUILD_W2_0912/checks/，本轮未重跑 |
| bcc-tools | x86_64 | PASS | PASS | 同上 |
| bpftrace | armv7l | NOT_OBSERVED | NOT_OBSERVED | bcc-tools 前置未闭合，未实施 |
| bpftrace | aarch64 | NOT_OBSERVED | NOT_OBSERVED | 同上 |

bpftrace 的 x86_64 不在 ExclusiveArch 中，按本轮人工确认不列入矩阵。
未连接开发板。bcc-tools 未提交的 15 行候选保持原样；bpftrace 不动。

11 包状态仍是 9 包已完成并推送、bcc-tools 部分验证、bpftrace 未实施。
tensorflow2 的 armv7l/GCC 保留“与基线同点同因失败”的限定，不能改写
为六格均完整构建成功。具体 SHA 及原验证口径见 PACKAGE_STATUS.tsv。

## 自行判断与未知

- 自行选择直接 rpmbuild 续跑是本轮故障来源，不能归因于用户原断点。
- 原构建树未删除，但部分对象和生成配置已经被这次错误启动改写；
  不声称原断点完整无损。
- 是否允许按恢复后依赖图继续，需人工决定；耗时尚无法准确估计。
- 未新增包验证，未新增包兼容性结论。原 11 包 spec 交叉核查仅沿用
  ../BUILD_W2B_0915/COMPATIBILITY_REVIEW.md，本轮不冒称重复完成。

## 收尾核对与脚本身份

raw/030：本轮 systemd scope 已无加载实例，相关构建进程已退出。
LLVM 输入工作树仍为 f203923a1508c9344f5fc6b17bd8822f011655c4 且干净。
bcc-tools 仍仅有原 spec 候选；bpftrace 工作树干净。
raw/032：两包远端分别仍为 dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2
与 30e51cd665360f85b665308dc13ba27df0c5f739。本轮未执行包 push。

造成配置重置的实际执行脚本已另存：
`govern_build_v1.py` 与 `llvm_resume_executed_retry.sh`。后者为恢复记录，
从仅增加 PATH 一行后的脚本还原；两者 SHA256 均与执行前 raw/010
记录精确一致（raw/030、034），并非将当前修订脚本冒充执行版本。
当前 `govern_build.py`、`llvm_resume.sh` 是后续修订版本，尚未用于续编。
更早的 freezer 客户端崩溃试次仅保存了当时哈希与执行日志，未完整另存
其监控器源文件；这是本轮脚本快照纪律的缺口，不能声称该试次可逐字重现。

提交前首次 whitespace 检查因命令原文包装器保留的行尾空格及原始输出
空白而退出 2（raw/037），未产生提交。不清洗原始证据；改为对报告、
状态表和自检脚本做 whitespace 检查，原始日志保持逐字保存。
