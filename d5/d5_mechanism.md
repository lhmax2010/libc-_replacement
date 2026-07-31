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
- 2026-07-30 OQ-5 红项裁决：`D5_MACRO_GLUE_WHITESPACE` 维持为真缺陷；
  空 allowlist 的“逐字节不变”判据不放宽。修复限定为宏胶水形态调整：
  条件展开自带分隔符、注入点无缝紧贴，机制本体不变。首跑证据路径追溯和
  本次复跑证据入口记录于
  `/home/toolchain/plan_evaluation/p1/d5_allowlist/evidence/oq5_red_rerun2/oq5_first_run_locator.md`。

勘误注记（2026-07-30）：§3 的 PoC 阴性对照只覆盖单包最小 fixture，
没有覆盖平台 buildconf 在空 allowlist 下对真实 spec 的逐字节影响，因此
存在单包盲区。OQ-5 平台差分首跑发现旧宏把分隔空格留在注入点外侧，空展开
仍会残留空格，违反“逐字节不变”。本次修正只改变胶水形态：命中时宏输出自带
前导分隔符，未命中时零字节；source 名判定、注入点位置和 POSIX token 循环
机制维持不变。正文关于机制可行性的结论不因该勘误放宽，必须以 OQ-5 复跑
重新证明。

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
子包必须作为同一个 libc++ **构建单元**。波次台账中的二进制包必须先映射
到 source RPM，再生成去重后的 buildconf allowlist。

D-G4 将“同源构建”与“镜像晋级”分层：同一 source 的 candidate 全输出都
使用 libc++ 构建，但每个输出必须进入逐 RPM 晋级台账，标为 `ADMIT`、
`ADMIT_STDLIB_NEUTRAL` 或 `HOLD_SIBLING`。只有前两类 candidate 可进入
镜像；普通 ADMIT 按 TIER1 分量闭包，neutral 必须精确命中经 authority
manifest 认证的 D-G4 登记册。HOLD_SIBLING 继续选择登记的存量
NEVRA+SHA256。构建同源不再被误解为同源 candidate 必须同时晋级。

### 2.3 RPM 4.14-safe 判定

候选宏使用 POSIX shell token 循环精确匹配，不使用
`%{expr}`、`%[...]` 或宿主侧特性：

```spec
%d5_libcxx_allowlist d5-positive
%d5_package_cxxflags %(result=; for item in %{d5_libcxx_allowlist}; do if test "x$item" = "x%{name}"; then result=' -stdlib=libc++'; fi; done; echo -n "$result")
%d5_package_ldflags %(result=; for item in %{d5_libcxx_allowlist}; do if test "x$item" = "x%{name}"; then result=' -lc++ -lc++abi'; fi; done; echo -n "$result")
```

该宏追加在现有 `Optflags` 的公共来源中。结果为：

- `%{name}` 在 allowlist：C++ 编译和 C++ 链接命令出现
  `-stdlib=libc++`，`%build_ldflags` 另令链接命令出现
  `-lc++ -lc++abi`；
- `%{name}` 不在 allowlist：宏输出严格为空，既有 flags 的字符序列不变。

胶水形态说明：D5 选项宏在命中时自带前导分隔符，未命中时输出零字节；
各注入点与 `%{d5_package_*flags}` 无缝紧贴。因此空 allowlist 不会留下额外
空格，`%build_ldflags` 的尾部空格也同法收干净；source 名判定、注入点和
POSIX token 循环机制不变。

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

## 8. allowlist 准入模板

每个 source package 加入 allowlist 前必须完成
`admission_check_template.tsv` 的四项检查：

1. **静态归档**：是否产出 `.a`。若存在，归档及其消费者必须按冻结
   HLD 的静态归档条款在同一 stdlib 批次重建，不能把 `.a` 当成动态图
   外的独立包；
2. **自定义链接**：是否使用 `-nostdlib`、`-nodefaultlibs` 或自定义
   linker script。命中后必须单独证明 runtime、unwinder 和 ABI provider
   闭包；
3. **混合子包**：是否由同一 source RPM 产生混合 C/C++ 子包。allowlist
   键仍是 source package，同源输出必须使用同一 stdlib 构建；
4. **晋级台账完备性与身份性**：candidate RPM manifest 的每个输出（包含
   `noarch`/共享输出）必须恰有一条台账记录，台账也不得出现 candidate
   manifest 中不存在的孤行。连接键固定为
   `(batch,target_arch,package,rpm_arch,nevra)`，manifest、ledger 与镜像
   实选 SHA 必须逐输出对账。普通 `ADMIT` 集合必须按经摘要认证的独立
   census 成员表形成 TIER1 分量闭包；台账自报分量只作交叉核对。
   `ADMIT_STDLIB_NEUTRAL` 仅在五元身份和 RPM SHA 精确命中经
   `gates/census_input_manifest.tsv` 认证的
   `gates/stdlib_neutral_registry.tsv` 时免闭包，仍须做身份三方对账；
   neutral 资格不再由台账列自行声明。HOLD 的镜像实选 SHA 必须等于
   存量权威。devel 与 `.a` 也不得漏行，libc++ `.a` 禁止对
   libstdc++ 消费者可见。执行件为
   `gates/tools/promotion_ledger_check.py`；缺列、空输入或缺少 census
   认证输入以 exit 3 fail-closed，摘要不符以业务红码
   `CENSUS_INPUT_UNVERIFIED` 阻塞。

波 1 五个 source package 的当前只读预扫见
`wave1_source_admission.tsv`：已覆盖 38 个相关冻结 RPM，未发现 `.a`；
五个源码树均未发现上述自定义链接开关；`askuser-notification`、
`libcynara-commons`、`security-manager` 是混合 C/C++ 源码树，正式构建
仍须用实际 target→RPM 映射复核 C-only 输出。

该五行表保持 S4 未决时的 19 包分支基线，不预填条件 source。若 S4 不满足
并选择 wave1 23 或 26 晋级分支，D5 allowlist 和准入表必须先增加
`boost-1.83.0-5.1.src.rpm` 与 `capi-appfw-capmgr-0.0.4-1.src.rpm`
两行，并以 `ledger/boost_source_unit_census.tsv` 复核架构目录
31/28/31 加共享 noarch 两输出后的三架构全量 **33/30/33**：

- 23 分支 ADMIT `boost-program-options`，同源 `boost-license` 记为
  `ADMIT_STDLIB_NEUTRAL`，其他 Boost candidate HOLD_SIBLING；
- 26 分支另 ADMIT
  `boost-filesystem/boost-log/boost-thread`，其他 Boost candidate
  HOLD_SIBLING。

若 S4 PASS，后续独立 T1-0008 批次同样必须先为 `boost` source 完成四项
准入并以 `ADMIT_STDLIB_NEUTRAL` 纳入 `boost-license`，但不改写 wave1
五源基线。若 26 分支生效，
还必须按 D-G4 跨批 authority 交接规则同步回写 Base-first 晋级台账。

冻结 RPM 输入上的三组实例见
`ledger/promotion_ledger_{basefirst,t1_0008_s4pass,wave1_cond}.tsv`
及其 manifest/authority；三组均须由第四门实跑为 GREEN。验证器断言必须与
数据同源生成，禁止硬编码预期。

## 9. OQ-5 正式采用门

正式并入 buildconf 前，开工日 D5 步骤必须先完成：

1. 对全平台 spec 运行空 allowlist 的 `rpmspec -P`，与 pre-D5
   buildconf 逐文件 diff 必须为空；
2. 选择至少一个含条件宏、生成子包和自定义 `%build` 的真实复杂包作
   负面对照，空 allowlist 下预处理 spec、编译/链接命令和 RPM payload
   必须与 pre-D5 基线一致；
3. 任一差异即停止 D5 并入，不能用当前两个最小 fixture 的 PASS 外推。

该项是 `startup_conditions.md` 执行期进入序列第 1 步的前置子项。
