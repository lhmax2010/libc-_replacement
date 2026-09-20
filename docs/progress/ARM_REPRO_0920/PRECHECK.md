# 复现前核对

## 原命令与唯一偏离

原 `progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json` SHA256 为 `38b3dd533205c58777e9fce51fe0b26ecefb174e6144eef3bed3bfea69e89b4f`。`original-argv.json` 保存原字节；本轮 `cell_commands/original-argv-repro.json` 与原数组逐元素比较，仅 strace 的 `-o` 后一项由旧 trace 路径变为 `progress/ARM_REPRO_0920/host-original-config.strace` 的完整绝对路径。机器断言见 `prepare.py` 和 `argv-only-difference.json`。

保留原 host strace、status=failed、binfmt 入口、无 -vv、QEMU_RESERVED_VA=0x100000000、可写 /tmp/task 绑定及全部 RPM 参数。不改 spec，不改构建根 rpm。外层日志/限时控制器为本轮副本，保持固定 PATH、Ninja -j1、nice19、ionice idle、MemoryMax=16536457216；失败后才触发目录/文件取证，不在 rpmbuild 内插入仪表。

旧 RPM/BUILDROOT/TMP 原目录不移走。执行前 cp -a 到 `tmp/ARM_REPRO_0920/previous-weekend-rpm-arm-llvm`，逐常规文件 SHA256、链接目标与模式/属主比较，保留 manifest。这个备份不是把 BUILD 树重置到旧时刻；本次沿用当前树，不声称恢复了历史所有时变状态。

## off_t 与载荷

实际二进制身份见 raw/022，ARM rpm ELF32 与 aarch64 rpm ELF64 见 raw/009。两套 rpmio 均导入 lseek64/stat64/fstat64（raw/011）。仅凭导入名字不判断全部类型；本次另直接反汇编实际 librpm：

- ARM `rpmcpioTell` 在 0x39cb8/0x39cbc 从结构偏移 +8/+12 分别读取 r0/r1，共 64 位。后续写头检查在 0x39d38–0x39d50 读取 offset 的两字、fileend 的两字并联合比较；写头末尾在 0x3a158/0x3a15c 使用 adds/adc 做 64 位加法。完整原文 raw/018。
- aarch64 `rpmcpioTell` 在 0x48414 从结构 +0x10 读取 x0（64 位），完整原文 raw/021。
- 固定源码 `progress/ARM_RPM_DIAG/upstream-cpio.c:34`–35 把这两个字段声明为 off_t，第92行函数返回 off_t；源码 SHA256 `783567b26c1dfd1b847d68c23cc62d213c3c4f80dee89f110d72d147bfc3ec16`。

**静态二进制核查支持：所查实际 cpio 实现的 offset/fileend 为64位，两架构一致。** 历史编译命令是否显式写 `_FILE_OFFSET_BITS=64` 仍 NOT_OBSERVED：现有二进制已 strip，相关 debug 目录和可定位的构建 config 记录未找到。不把“未取得宏定义”误写成“off_t 是32位”。

ARM lldb-debuginfo 五个普通文件分别为224280、94576720、7594696、232318436、9954168字节，总和 **344668300**，现有旧BUILDROOT大小与RPM清单逐项相符（raw/015、payload-precheck.json）。这些是复现前的大小，不冒充失败瞬间或写头时 st_size。载荷小于2GiB，不支持以该阈值解释故障。

aarch64完整12包中没有lldb-debuginfo子包（raw/014列全包名），因此对应包的载荷合计为 NOT_AVAILABLE；不能拿未打包目录随意拼成同口径子包。它的64位rpm/cpio实现已独立核对。

## 技术性命令记录

raw/011末尾源码路径检索无匹配，退出1；此前readelf输出仍完整保留。raw/014末尾宏检索无匹配，退出1；反汇编和12包列表已完成。aarch64只读反汇编曾等待，拟停止时进程已经自然退出，raw/020 kill返回1（No such process），没有停止构建进程；后续有界只读反汇编raw/021退出0。不存在/尚未生成的备份manifest探测非零保留，不作为构建失败。

本步骤只做预检；首轮结果尚待实际执行。第一步若成功立即停，未取得失败瞬间归档值时必须记 NOT_OBSERVED。
