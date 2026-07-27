# D5 buildconf 全局可用与 per-source-package allowlist

状态：`POC_PASS_ADOPTION_BLOCKED_PENDING_CONFIG_DECISION`  
日期：2026-07-27

变更记录：

- 2026-07-27 设计冻结：先确定 buildconf 宏载体、source-package
  allowlist 语义和三组对照，不实施。
- 2026-07-27 受控实施：仅修改 `p1/d5_allowlist/config/` 下的隔离副本，
  完成三组对照、armv7l G1–G5 和三架构仓接线验证。
- 2026-07-27 资源控制续跑：首次 rootstrap 下载在第 66/105 项处随会话
  中断；后续统一限制为 CPU 0–1、`nice 10`、单线程和每进程 6 GiB
  虚拟内存上限。

## 1. 数据支撑的输入事实

- 当前原生 clang 配置为
  `/home/toolchain/plan_evaluation/gbs_llvm.conf`，SHA256
  `a32c1aba714714bfba8aba1b4e78bf015f297f4162bcff9652250c6d5b81f2b7`。
- S6 钉住的 project config 为
  `p1/wave1_cynara/blocker_adjudication/raw/01_effective_tizen_unified_standard.conf`，
  SHA256
  `9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5`。
- S6 命令 SHA256 为
  `0108f58741daab38f44be60cef6a30265965721efa3615cf3d4a5e76fba41caf`；
  命令含 `-D`，因此 project config 内容是构建语义的一部分。
- armv7l、aarch64、x86_64 的本地候选仓均含
  `libc++-devel-21.1.1-2` 及 runtime/ABI 配套 RPM，并各自带
  `repodata/repomd.xml`。
- aarch64/x86_64 的候选 `libc++.so.1` 声明
  `libatomic.so.1()(64bit)` 依赖；目标仓解析性必须由目标架构 solver
  实测，不能用宿主 RPM 数据库代替。

## 2. 已落定机制

### 2.1 物理载体

采用 project buildconf 的 `Macros:` 段作为 allowlist 和判定逻辑载体；
本地 libc++ repo 通过 GBS profile 的 repo 列表接入。buildconf 对所有
构建根声明 `libc++-devel = 21.1.1-2` 可用，其依赖链拉入
`libc++`、`libc++abi-devel` 和 `libc++abi`。

本任务先在 `p1/d5_allowlist/config/` 下复制一份候选 buildconf 和 GBS
配置进行 PoC；不修改原始 `gbs_llvm.conf`、S6 project config 或平台仓。

### 2.2 判定单位

RPM 宏在解析 spec 时以 `%{name}` 判定。因此 allowlist 的键是
**source RPM/spec Name**，不是二进制子包名。一个 source RPM 产生的全部
子包必须作为同一个迁移单元。波次台账中的二进制包必须先映射到 source
RPM，再生成去重后的 buildconf allowlist。

### 2.3 RPM 4.14-safe 判定

候选宏使用 POSIX shell token 循环精确匹配，不使用
`%{expr}`、`%[...]` 或宿主侧特性：

```spec
%d5_libcxx_allowlist d5-positive
%d5_package_cxxflags %(result=; for item in %{d5_libcxx_allowlist}; do if test "x$item" = "x%{name}"; then result='-stdlib=libc++'; fi; done; echo -n "$result")
%d5_package_ldflags %(result=; for item in %{d5_libcxx_allowlist}; do if test "x$item" = "x%{name}"; then result='-lc++ -lc++abi'; fi; done; echo -n "$result")
```

该宏追加在现有 `Optflags` 的公共来源中。结果为：

- `%{name}` 在 allowlist：C++ 编译和 C++ 链接命令出现
  `-stdlib=libc++`，`%build_ldflags` 另令链接命令出现
  `-lc++ -lc++abi`；
- `%{name}` 不在 allowlist：宏输出严格为空，既有 flags 的字符序列不变。

`-stdlib=libc++` 负责 clang 头文件选择和 driver 的标准库链接语义；
显式 `-lc++/-lc++abi` 只追加到 `%build_ldflags`，保留在链接命令中作为
D5 注入取证面，不进入普通编译命令。候选 buildconf 已有
`-Wno-unused-command-line-argument`，故公共 `%optflags` 被 C 编译步骤复用
时不会把 `-stdlib` 诊断升级为失败。

### 2.4 未采用方案

- 不使用 spec 内条件：会修改平台源码，且无法保证未列包的 spec 不变。
- 不使用 `gbs --define`：会改变 S6 命令字节，并使 `OFF_PROVEN` 的
  “define 集为空”证据失效。
- 不使用 `prjconf Substitute` 决定标准库：Substitute 只能改变依赖求解，
  不能令 clang 的头文件选择和链接选项按包切换。
- 不用 CXX wrapper：CMake 对带参数的 `CXX` 值处理并不统一，不能作为
  平台通用机制。

## 3. 三组实测预登记

### 3.1 阳性

source package `d5-positive` 在 allowlist。必须同时满足：

1. 原始 C++ 编译命令含 `-stdlib=libc++`；链接命令另含
   `-lc++ -lc++abi`；
2. 产物 `DT_NEEDED` 含 `libc++.so.1`，不含 `libstdc++.so.6`；
3. G1：无 libstdc++ NEEDED、无 GLIBCXX/CXXABI 版本引用及未批准
   `_ZNSt*` 导出；
4. G2：不适用于迁移侧，记录 `N/A_ACTIVE_SIDE`；
5. G3：NEEDED 闭包和强 UND 可解析，provider 属候选 libc++ 链；
6. G4：相对预登记导出清单无增删；
7. G5：`LD_BIND_NOW=1 ldd -r` 强未解析为零，并记录关键绑定。

### 3.2 阴性

同一 source package `d5-positive` 从 allowlist 摘除，用候选 buildconf
重建；另以未加 D5 宏的原生 buildconf 构建基线。过滤构建系统进度行后，
实际 compiler/link command 行必须逐字节相同；RPM payload 解包后逐文件
类型、模式和 SHA256 必须一致。若 RPM header 因构建时间不同而变化，单独
记录，不用 raw RPM SHA 代替内容判定。

### 3.3 混态

同一 buildroot 先构建 allowlisted `d5-positive`，再构建未列入的
`d5-control`。第二包的 compiler/link command 必须与其独立原生基线逐字节
一致，产物必须依赖 `libstdc++.so.6` 且不依赖 `libc++.so.1`。

## 4. S6 兼容性预判

设计可保持 S6 `command.txt` 的命令文本不变：allowlist、repo 和 flags
都从 buildconf/profile 生效，不增加 `--define`。但是当前 S6 命令以 `-D`
钉住 SHA 为 `9eac…` 的旧 project config；正式采用 D5 必须更新该
buildconf 内容（并让 `gbs_llvm.conf` 可见本地/平台 libc++ repo）。

因此：

- `OFF_PROVEN` 的 profile 宏结论可通过证明 D5 块不定义
  `uid_app_sandboxing/dev_wos` 来继承；
- S6 的 **command bytes 可以不变**，但 buildconf SHA 与 repo 配置证据
  会变化，S6 必须重新取证和冻结；
- 本 PoC 不会原地修改上述两个文件。若实测证明机制成立，正式接入仍需
  平台裁决后修改 buildconf/repo 配置。

## 5. 实测结果

### 5.1 目标 RPM 宏门

目标 armv7l buildroot 内 RPM 为 4.14.1。三组展开结果分别为：

- allowlist=`d5-positive`、name=`d5-positive`：
  `CXX=[-stdlib=libc++] LD=[-lc++ -lc++abi]`；
- allowlist=`d5-positive`、name=`d5-control`：两者均空；
- allowlist=`%{nil}`、name=`d5-positive`：两者均空。

两份 fixture spec 的目标侧 `rpmspec -P` 均返回 0。证据：
`evidence/macro_precheck/target_rpm_eval.log` 和
`target_rpmspec_smoke.log`。

### 5.2 三组 armv7l 对照

| 对照 | 结果 | 数据依据 |
|---|---|---|
| 阳性 | PASS | 编译命令出现 `-stdlib=libc++`；链接命令出现 `-lc++ -lc++abi`；产物 NEEDED 为 libc++/libc++abi，无 libstdc++ |
| 阴性 | PASS | 同一包在原生与空 allowlist 下，compiler/link command 文件逐字节相同；解包后的路径、类型、模式和 SHA256 清单相同 |
| 混态 | PASS | 同 buildroot 先构建 allowlisted 包、再构建 control；control 命令与其独立原生基线逐字节相同，产物仍依赖 libstdc++，无 libc++ |

阴性和混态均以规范化 RPM payload 判等。raw RPM 的压缩流/构建元数据不
作为“内容一致”的替代判据。机械结果见 `contrast_verdict.tsv`，
命令与 payload 对账原文见 `evidence/commands/` 和
`evidence/payload/`。

### 5.3 armv7l G1–G5

五门全部 PASS：

- G1：迁移侧 NEEDED 含 libc++/libc++abi、不含 libstdc++，无
  GLIBCXX/CXXABI 引用，无未批准 `_ZNSt*` 导出；
- G2：同一 buildroot 的非 allowlist control 仍依赖 libstdc++，无
  libc++ 与 `std::__1`；
- G3：`LD_BIND_NOW=1 ldd -r` 返回 0，强未解析为 0；
- G4：实际导出集合精确等于预登记的单一 `d5_positive_probe`；
- G5：runner 返回 0；DSO 的两个 `std::*` 构造绑定到 libc++.so.1，
  `__cxa_throw`/`__cxa_allocate_exception` 绑定到 libc++abi.so.1。

机械表见 `gates_armv7l.tsv` 和 `gate_metrics.tsv`，逐字绑定原文见
`evidence/gates/positive/runner.ld_debug.log`。

### 5.4 三架构仓、头文件和 libatomic

armv7l、aarch64、x86_64 三者均：

- solver 安装 `libc++-devel-21.1.1-2`；
- `/usr/include/c++/v1/string` 归候选 devel 包所有，clang 21 以
  `-stdlib=libc++` 的实际搜索路径包含该目录；
- buildroot 中 `libatomic-14.2.0-1.11` 可解析；
- 阳性 DSO 的 `ldd -r` 返回 0，闭包不含 libstdc++。

aarch64/x86_64 闭包还实际出现 `libatomic.so.1`。汇总见
`multiarch_repo_gate.tsv`，原文见 `evidence/repo/` 与
`evidence/gates/positive_{aarch64,x86_64}/`。

## 6. 中断诊断与资源约束

首次构建日志没有编译、solver、内存分配或 OOM 错误，最后一行是
rootstrap 第 66/105 个 RPM 的正常下载，退出码为 1；这与外层会话中断
一致。宿主 kernel ring buffer 对当前用户不可读，因此不能用 dmesg
排除内核级 OOM，只能下结论：
`NO_OOM_EVIDENCE; SESSION_INTERRUPTION_MOST_CONSISTENT`。

续跑使用 `resource_limited_run.sh`：

- `taskset -c 0,1`，最多使用两个 CPU；
- `nice -n 10`；
- `ulimit -v 6291456`，每进程最多 6 GiB 虚拟内存；
- `MAKEFLAGS=-j1`、`NINJAFLAGS=-j1`、CMake 并行级别 1；
- 所有 GBS 命令 `--threads 1`。

中断原文为 `evidence/builds/baseline_interrupted.log`，限制配置为
`evidence/resource_limits.log`，dmesg 权限失败原文为
`evidence/dmesg_raw.log`。

## 7. S6/OFF_PROVEN 最终影响与停止点

本任务证明了机制可行，但**没有改动**原始 `gbs_llvm.conf`、S6
buildconf 或 S6 `command.txt`。正式采用需要两处外部状态变化：

1. `gbs_llvm.conf`/平台 profile 增加候选 libc++ 仓（本 PoC 用
   `127.0.0.1` HTTP 仅为本地取证，不是发布方案）；
2. S6 所指 project config 加入 `Required: libc++-devel`、allowlist
   宏和按包 flags。

所以：

- D5 机制结论：`PASS`；
- S6 命令文本可保持不变，但其 `-D` 指向的文件内容 SHA 会改变，
  现有 S6 冻结必须重开并重新计算 SHA；
- D5 diff 不定义 `uid_app_sandboxing`、`dev_wos` 或任何
  `--define`，OFF_PROVEN 的逻辑判定不变；但“构建命令—buildconf”
  字节证据链应随新 SHA 做一次机械重取证；
- 正式修改 buildconf/profile 超出本 PoC 隔离实现范围，状态为
  `ADOPTION_BLOCKED_PENDING_CONFIG_DECISION`，在此停下等待裁决。
