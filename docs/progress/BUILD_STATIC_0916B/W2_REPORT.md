# 完整 RPM：PARTIAL

## 已完成

用 f203923a 的 Git 内容重新生成完整 Source0，使用上一轮候选
libcxx-runtimes.spec，实际执行 **aarch64 rpmbuild -bb**，经过 %prep、%build、
%install、文件检查、写包与 %clean，退出 0。不是 --short-circuit，也不是手工
构造“只装现有 .a”的替代 spec。1833 个 Ninja 目标完成，日志见
runtime-aarch64-rpm.build.log。编译并行 1，内存/优先级继承记录器限制。

四个 RPM 保留在 `tmp/NIGHT_0917/finish_validation/rpm/RPMS/aarch64/`：

- libc++-22.1.8-1.aarch64.rpm
- libc++-devel-22.1.8-1.aarch64.rpm（文件清单包含 /usr/lib64/libc++.a）
- libc++abi-22.1.8-1.aarch64.rpm
- libc++abi-devel-22.1.8-1.aarch64.rpm（包含 /usr/lib64/libc++abi.a）

逐包 SHA256 与完整文件清单见 raw/050、rpm_outputs.tsv。运行时仍有既知的缺
GNU Build ID 警告；本任务未处理。Release 为候选 spec 原有的 1，未自行改号。

此次使用 _toolchain=clang；本轮未为这份新 RPM 再跑 GCC/undefined，不能把
上轮目标构建直接写成此次 RPM 三路径通过。新 RPM 的构建环境/标志与上轮独立
目标构建记录分别保留，未以两者字节相同为前提，也未替换原 bpftrace 候选输入。

## 未完成与阻断记录

后续 LLVM 完整 RPM 不能用上一轮 Clang standalone/libclang_static 目标目录
代替：候选 llvm.spec 的 %install 要安装 LLVM、Clang、LLDB、OpenMP 等完整树。
已检查的旧完整 LLVM 树为 GCC 配置，且 libLLVM/libclang 共享产物不在原位置。
没有把 standalone 目录或旧 GCC 产物拼成“libc++ 完整 LLVM RPM”。

实际检查三类构建根：

1. 旧 LLVM 根有 Python-devel、patchelf、binutils-devel 等，但无 Clang 和 libc++
   开发包；计划复制到任务临时根，再安装已有 Clang/libLLVM RPM 与新运行时 RPM。
2. bpftrace 的 libc++ 根有已验证 Clang，但缺 Python-devel、patchelf、binutils-devel、
   libxml2-devel、Ninja 的 RPM 记录。
3. 运行时构建根可完成本次运行时 RPM，但也不具备上述完整 LLVM 依赖集合。

复制第 1 种根的 usr 时实际退出 1，下列位置 Permission denied：

```
usr/sbin/glibc_post_upgrade
usr/sbin/unix_update
usr/share/upgrade
```

复制在此停止；未跳过这些文件、未把缺文件的目录当作干净可用构建根、未提权。
`tmp/NIGHT_0917/finish_validation/llvm-root/` 只是未完成副本，**不得续用而不核查**。
准备脚本后面的 RPM 安装没有执行，namespace UID 0 也没有进入；不能宣称已安装
静态开发输入。原根未修改。

因此本轮 **libclang.a 的完整开发 RPM、bpftrace 三路径 RPM、干净容器安装和
%post 覆盖验证均未完成**。没有生成这些结果的 SHA，不用上轮手工安装的哈希替代。
这不是证明无法构建；继续需要可读完整 LLVM 构建根，或重新准备完整依赖根。
在静态取消已实际失败且构建根准备受阻的情况下，本轮保存断点并交人工裁决，
没有继续扩大环境重建工作。没有以超时为理由：本轮未耗尽四小时时限。

## 架构口径与待确认

候选 bpftrace 的 ExclusiveArch 为 armv7l/aarch64，Source1002 无 x86_64。
未擅自扩架构、绕过 ExclusiveArch 或为 GCC/x86_64 伪造原始资产。
已询问是否三路径 RPM 仅做 aarch64；回答尚待人工。x86_64 的独立构建/功能
实验不能视作已有官方打包路径。

## 技术性修正

首轮 RPM 默认使用 /var/tmp 写临时文件，原构建根只读而失败；重跑仅把 _tmppath
指向 /tmp/night/finish_validation/rpm。没有修改 spec 逻辑。失败与重跑日志均保留。
资源门禁和源码输入哈希见 raw/023、rpm_source_manifest.json。
