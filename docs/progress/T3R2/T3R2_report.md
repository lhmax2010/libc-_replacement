# T3R 续跑报告：armv7l/aarch64 调用上下文修正后重建

## 结论

本次续跑总体状态为 `RED_STOP`：调用上下文缺陷已经闭合，两架构均由 GBS 明确解析为唯一目标 `libcxx-runtimes`，未再出现全仓扫描或 circle；aarch64 构建成功并归档 7 个二进制 RPM 与 1 个 SRPM，armv7l 在实际编译阶段因 `__gnu_unwind_frame` 未声明失败，按纪律登记 `UNCLASSIFIED`、退役新根且不重试。

这不是上一轮的调用错误复现。armv7l 已进入 `libcxxabi/src/cxa_personality.cpp` 编译，完整错误为：

```text
/home/abuild/rpmbuild/BUILD/llvm-22.1.8/libcxxabi/src/cxa_personality.cpp:1135:11: error: use of undeclared identifier '__gnu_unwind_frame'
```

未修改 spec、源码或只读目录；x86_64 既有制品逐 RPM SHA256 与 T3R 归档值逐项一致。

## 1. 调用上下文取证与修正

三次原调用的完整命令、执行目录、输入位置与 GBS 实际扫描起点见 `invocation_commands.tsv` 和 `invocation_delta.tsv`。上一轮 armv7l/aarch64 的 full log 没有打印 `pwd`，其启动目录依据本次人工裁决与既有 `T3R_report.md` 记录落盘，并在表内显式标注，没有把缺失日志伪装成新实测。

- x86_64 成功调用：`pwd`、位置参数、预导出输入三者均为仓外 `/var/tmp/toolchain-gbs/T3R-obs-input-89bc0b8/libcxx-runtimes`；depanneur 实际从该目录扫描，输出 `there is no circle in 1 packages`。
- armv7l 旧失败调用：`pwd` 与输入均在仓内，depanneur 的 `--path` 和实际扫描起点均被归一到仓根，报告 389 包 circle。
- aarch64 旧失败调用：输入位置参数已经在仓外，但 `pwd` 仍在仓根；depanneur 命令虽显示仓外 `--path`，`start building packages from:` 仍是仓根，报告 392 包 circle。

因此，导致扫描起点落入仓根的差异项是启动工作目录；armv7l 另有仓内输入位置与多余 `--spec` 差异。修正严格照抄 x86_64 成功上下文：进入仓外单包目录执行、使用相同仓外位置参数、相同 `-c`/`-C`/`--debug`/`--style obs`，只替换架构和全新 `-B ...-r2` 根。逐项调整与依据见 `invocation_adjustments.tsv`。

两次开工均先断言：仓外目录不是 Git 工作树、恰有一个 spec、包名为 `libcxx-runtimes`、五项输入与两项配置 SHA256 全部匹配。实际 GBS 日志随后在 `/usr/bin/build` 之前同时给出 `Checking dependencies for libcxx-runtimes`、`there is no circle in 1 packages` 与 `[1/1] building libcxx-runtimes`，原文见各架构 `target_assertion.raw.log`。

## 2. 逐架构结果

| 架构 | 状态 | 工具链 | 产物 | 红项 |
|---|---|---|---|---|
| x86_64 | `SUCCEEDED_EXISTING_UNCHANGED` | clang 22.1.8-18.1 | 既有 7 binary + 1 SRPM，逐 SHA 不变 | 无 |
| armv7l | `RED_STOP` | clang 22.1.8-18.1 | 无 | `UNCLASSIFIED`：`__gnu_unwind_frame` 未声明 |
| aarch64 | `SUCCEEDED` | clang 22.1.8-18.1 | 新归档 7 binary + 1 SRPM | 无 |

机器可读状态见 `build_status.tsv`，三架构实际 buildroot 工具链 NEVRA 全表见 `toolchain_nevra.tsv`。

### armv7l

新根为 `tmp/GBS-ROOT/LIBCXX-2218-armv7l-20260804-r2`，已标记 `RETIRED_SINGLE_USE`。构建在 1759 个 Ninja 动作的 `cxa_personality.cpp.o` 阶段失败；不是 `%files` missing/unpackaged，也不是 BuildRequires solver 失败，故不套用 C10 或 `BUILDREQ_VERSION_FORM`，按“其他”登记 `UNCLASSIFIED`。完整 GBS 日志、rpmbuild 日志、错误索引和上下文分别为 `build_armv7l.full.log`、`armv7l/rpmbuild_fail.full.log`、`armv7l/error_index.raw.log`、`armv7l/error_context.raw.log`。未修改任何输入，也未发起第二次 armv7l 构建。

### aarch64

新根为 `tmp/GBS-ROOT/LIBCXX-2218-aarch64-20260804-r2`，已标记 `RETIRED_SINGLE_USE`。rpmbuild 完成编译、安装、`Checking for unpackaged file(s)` 与写包，GBS 汇总 `Total succeeded built packages: (1)`，退出 0。制品已复制到 `artifacts/rebuild_2218/aarch64/`；完整 SHA256 见 `aarch64/artifact_sha256.tsv` 和汇总 `artifact_sha256.tsv`。其中 SRPM 为：

```text
dca0d4f5d01d250d25882312b34fe00545dc6fd967eb03f4d9be9c0c8ec05b74  libcxx-runtimes-22.1.8-1.src.rpm
```

所用配置以不覆盖旧档的名称 `gbs_llvm.r2.conf`、`tizen_unified_standard.r2.conf` 归档；SHA256 分别为 `8fab16c1...37d49cf`、`9eac0e23...fb736a5`。

## 3. 输入与既有结果保护

构建输入 spec SHA256 仍为 `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74`，map/manifest/SOURCE_PROVENANCE 也与 T3R 冻结值一致；`git diff --exit-code 89bc0b8... -- packaging` 退出 0。完整表见 `input_identity.tsv`。

x86_64 的 8 个既有 RPM 重新计算后与 `progress/T3R/x86_64/rpm_sha256.tsv` 逐项 diff 为空，证据见 `x86_64/expected_rpm_sha256.tsv`、`x86_64/actual_rpm_sha256.tsv` 和空的 `x86_64/unchanged.diff`。本次没有写入 x86_64 制品目录。

`plan_evaluation` 与备份目录未被访问为写目标；源码、spec 与既有 T3R 结论均未修改。

## 4. 交叉架构一致性门禁

任务规定“两架构成功后执行”。armv7l 没有 RPM/SRPM，前置条件不成立，因此子包构成、`%files` 路径集、Provides/Requires 和三架构 SOURCE_PROVENANCE 对照均登记 `NOT_RUN_TWO_ARCH_SUCCESS_REQUIRED`，没有用 aarch64/x86_64 的二架构结果冒充三架构门禁。机器可读状态见 `cross_arch_comparison_status.tsv`。

## 5. 红项与流程档案

唯一红项见 `red_items.tsv`：armv7l=`UNCLASSIFIED`，交人工裁决；本报告不评估影响、不提出处置建议。

流程档案已追加 `process_lesson.md`：构建类任务必须同时申报执行工作目录和全部路径参数，日志首行固定写 `WORKING_DIRECTORY`，并保留 depanneur 实际扫描起点。workspace 布局改变时，只保存命令行不足以复现扫描语义。

执行中有三项已完整留痕但不改变技术结论的取证事件：一次过宽 `find` 被终止并记录退出 143；首次用 `sudo rpm --root` 读取 NEVRA 因无交互终端退出 1，随后用只读 `rpm --root --dbpath /var/lib/rpm` 成功；首次 x86 SHA 归一化命令因 sed 转义错误退出 1，保留失败日志后改为 awk 并通过。相关命令原文与退出码均在 `commands/`。

## 6. 停止点

状态写入 `progress/state/T3R2.tsv` 为 `RED_STOP`。本次到此停止，不进入 T4。
