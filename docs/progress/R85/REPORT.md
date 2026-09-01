# R85：QuickBuild 工具链快照发布可用性验证

## 结论

**发布可行性结论：当前快照不应原样发布（`NOT_RELEASE_READY`）。**

这不是补丁缺失导致的。相反，`aarch64`、`armv7l`、`x86_64` 三个可用架构的包均已确认包含四个定稿补丁：十五项头文件检查全部通过，库中存在层 B 的类型信息符号，层 A 与层 C 库内批的可见函数/路径也有符号表及反汇编证据；RPM 的 `VCS` 元数据精确指向预期 HEAD `08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`。

阻止原样发布的是包内公开安装的 `SOURCE_PROVENANCE` 明确失实：它仍记录旧 HEAD `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`、`tizen_base` 和 6 个补丁，而实际 SRPM 含 10 个补丁、最后四个对应本次 series。该文件由每个架构的 `libc++-devel` 安装，不能以“包外记录”处理。建议修正打包元数据后重建三个架构的 `libc++-devel` 所属源包产物，再进入发布流程。

另有一项独立风险：三个架构的 `libc++.so.1.0` 与 `libc++abi.so.1.0` 均无 GNU Build ID，构建日志也逐项给出 `Missing build-id` 警告。当前材料不能确认 Tizen 发布门禁是否允许；必须由发布负责人核对策略。即使发布策略允许无 Build ID，也不能消除上述 provenance 阻断项。

## 1. 范围与纪律

- 快照：`tizen-base-toolchain_20260828.101647`。
- 仅下载 9 个核心二进制 RPM、3 份相应构建日志和 1 个源码 RPM；未镜像整仓。
- 未修改平台源码、补丁或 `project_config`；未执行发布；未向 Gerrit/外部源码仓推送。
- 资源门禁 `tools/resource_gate.sh --level light` 返回 0，见 `raw/001_*`。
- 下载包只作为本地临时验证输入，不提交仓库；其身份记录在原始输出中。

## 2. 快照盘点

完整逐包表见 `RPM_INVENTORY.tsv`。与 libc++/libc++abi 相关的产物为：

- 二进制架构：`aarch64`、`armv7l`、`x86_64`。
- 每个架构普通仓各 4 个子包：`libc++`、`libc++-devel`、`libc++abi`、`libc++abi-devel`。
- 每个架构调试仓各 3 个子包：`libc++-debuginfo`、`libc++abi-debuginfo`、`libcxx-runtimes-debugsource`。
- 源码仓 1 个：`libcxx-runtimes-22.1.8-1.1.src.rpm`。
- 所有相关 RPM 版本/Release 均为 `22.1.8-1.1`。
- 普通仓同时存在 `i586`、`i686`、`riscv64`、`noarch` 目录，但这些目录没有 libc++/libc++abi 相关 RPM。因此本快照**不覆盖**这些架构；任务点名的 `armv7l`、`aarch64`、`x86_64` 均覆盖。
- 在三个实际覆盖架构的上述 7 个预期二进制子包中未发现缺包。

原始索引证据：普通包 `raw/008_*`，源码包 `raw/009_*`，调试包 `raw/052_*`。

## 3. 层 C 头文件批

逐架构、逐文件结果见 `HEADER_CHECKS.tsv`；实际上下文（每架构五段，非仅“有/无”）见 `raw/021_compact_header_snippets_and_hashes.stdout`。三架构的五个文件 SHA256 分别完全一致，十五项均为 `PASS`。

| 文件 | 命中行 | 实际关键代码 |
| --- | ---: | --- |
| `__ostream/basic_ostream.h` | 218 | `} catch (const __cxxabiv1::__forced_unwind&) { throw; }` |
| `future` | 897 | 捕获后调用 `__handle_forced_unwind(...__restore_deferred)`，随后 `throw;` |
| `istream` | 378 | 捕获后置 `badbit`、调用 `__setstate_nothrow`，随后 `throw;` |
| `string` | 3467 | 捕获 `__forced_unwind` 后立即 `throw;` |
| `cxxabi.h` | 43 | `class _LIBCXXABI_TYPE_VIS __forced_unwind`，含虚析构与纯虚 dummy |

这些头文件确由 `libc++-devel` 所有；`rpm -qlp` 证据见 `raw/031_*`。因此用这批 `libc++-devel` 建立构建根时，消费者编译得到的是包含层 C 头文件防护的模板/内联代码。

## 4. 层 A、层 B 与层 C 库内批

方法与结果：

1. 用 `readelf -Ws`、`nm -D` 对解包后的共享库检查动态符号。
2. 用 `llvm-objdump` 对可定位的函数体按符号地址反汇编；ARM 库用 Thumb triple。
3. 从源码 RPM 只提取 spec、provenance 和补丁 0007—0010，计算 SHA256，并与 R83 冻结导出逐字节比较。
4. 从三份 QuickBuild 日志检查 `%prep` 实际应用 0007—0010，且最后写出所有 RPM。

关键直接证据：

- 三架构 `libc++abi` 均导出 `__cxxabiv1::__forced_unwind` 的 typeinfo、typeinfo name、vtable、D0/D1/D2，符号版本为 `LLVM_22`；层 B 通过。
- 三架构 `libc++` 均引用该 typeinfo，并含 `__assoc_sub_state::__sub_wait`、`ios_base::__set_badbit_and_consider_rethrow`、`__set_failbit_and_consider_rethrow`；所选函数体反汇编与防护流程相符。
- `aarch64`、`x86_64` 的 `__cxa_rethrow` 反汇编直接显示本地异常走 `_Unwind_RaiseException`、外来异常走 `_Unwind_Resume_or_Rethrow`。`armv7l` 的 Thumb 函数体及两个 unwind 导入均已确认；其 PLT 调用名无法仅凭已剥离 ELF 的函数体逐条标注，故不把 PLT 地址映射作超出证据的推断。
- SRPM 四补丁 SHA256 与 R83 冻结记录完全一致，逐字节比较均返回 0；构建日志确认三架构均实际应用四补丁。

汇总见 `BINARY_CHECKS.tsv`，完整符号和反汇编见 `raw/026_*`、`raw/044_*`、`raw/045_*`、`raw/046_*`、`raw/053_*`。

局限：优化、模板实例化与 strip 使层 C 库内批的每一个源码 catch 点不能都在最终 ELF 中取得独立、可命名的二进制标记。对可见函数给出了直接符号/反汇编证据；对其余点只报告“SRPM 补丁字节一致且构建日志证实已应用”，不声称逐点反汇编完成。

## 5. 版本身份与 provenance

版本身份有三条相互独立的正证据：

- 下载的 9 个二进制 RPM 的 `VCS` 均为 `platform/upstream/llvm#08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`，`DISTURL` 也一致，见 `raw/043_*`。
- SRPM spec 的 `VCS` 是同一 HEAD，声明并在 `%prep` 应用 Patch0—Patch9，见 `raw/042_*`。
- SRPM 的 0007—0010 与 R83 冻结四补丁 SHA256 和内容一致，见 `raw/037_*`。

但包内 `SOURCE_PROVENANCE` 问题仍存在，且三个架构文件内容及 SHA256 完全相同：

```text
local_tizen_base_vcs=platform/upstream/llvm#3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b
local_tizen_base_branch=tizen_base
export_patch_count=6
source_export=GBS standard non-native export from upstream/22.1.8 plus six auto-generated downstream patches; --no-patch-export forbidden
```

实际是 sandbox HEAD `08a64b6...` 和 10 个导出补丁。完整原文见 `raw/032_packaged_source_provenance.stdout`。因此“源码版本是否对应预期 HEAD”的答案是：**实际构建输入和 RPM VCS 对应；安装进包的 provenance 声明不对应。**

## 6. 发布前处置

1. 修正 `SOURCE_PROVENANCE`，使 HEAD、分支、补丁数和 0007—0010 身份与实际 SRPM 一致。
2. 重新构建 `libcxx-runtimes` 的 `aarch64`、`armv7l`、`x86_64` 产物；修正文件位于 `libc++-devel` 包内，不能只改仓库旁路说明来修复已生成 RPM。
3. 重建后至少复核：RPM `VCS`、包内 provenance、十五项头文件、层 B 类型信息符号、四补丁应用日志。
4. 由发布负责人确认 libc++/libc++abi 缺 Build ID 是否符合平台政策；如不符合，还需修正链接/打包配置并随同重建。日志上下文见 `raw/058_*`、`raw/059_*`。

## 7. 已验证与未覆盖

已验证：快照目录盘点；三架构核心 RPM 下载与 digest；三架构十五项头文件；共享库动态符号与部分关键函数反汇编；SRPM 四补丁身份；三架构构建日志；RPM VCS；包内 provenance；GNU Build ID。

未覆盖/未执行：没有运行目标架构二进制；没有下载调试 RPM（只盘点）；没有逐一反汇编层 C 的每个源码 catch 点；没有确认平台对缺 Build ID 的正式发布政策；没有执行发布、构建、源码修改、补丁修改或 `project_config` 修改。

## 8. 自行判断与尚存疑问

- 自行判断：将“可用于发布”解释为产物内容与随包发布的来源声明都必须真实一致。基于这一发布完整性标准，错误的已安装 provenance 是阻断项，而不是可忽略备注。
- 自行判断：源码 RPM 约 254 MB，但它是确认精确四补丁身份与构建输入所需的唯一额外大文件，仍符合“只下载验证所需包”，未下载整仓或调试包。
- 尚存疑问：QuickBuild/Tizen 的正式发布门禁是否允许运行库无 GNU Build ID，当前环境没有权威策略材料，记为 `NOT_AVAILABLE`；已通过实际 ELF 和构建日志确认现象。
- 无其他未说明推断。

## 9. 原始材料索引

- 全部命令原文、标准输出、标准错误与退出码：`raw/`。
- 日志包装器：`code/run_logged.sh`。
- RPM 盘点：`RPM_INVENTORY.tsv`。
- 头文件逐架构结果：`HEADER_CHECKS.tsv`；实际片段：`raw/021_compact_header_snippets_and_hashes.stdout`。
- 二进制检查：`BINARY_CHECKS.tsv`。
- 早期错误尝试亦保留：`raw/018_*`（相对路径）、`raw/022_*`（错误猜测提交）、`raw/039_*`（日志器参数）、`raw/047_*`（日志器参数）、`raw/048_*`（误判 debug 目录层级）、`raw/057_*`（错误路径）；后续带 `retry`/`corrected` 的记录为有效结果。
