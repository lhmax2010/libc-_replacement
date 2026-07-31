# T2 波 1 五源包源码缺陷清点报告

状态：`DONE`；五包全树静态清点、传播面和注入预检完成。整包诊断编译因当前 standard 仓缺 `libc++-devel` 在依赖解析阶段 `NOT_AVAILABLE`，未把未编译伪装成无错误。

## 五个实测 Name

`Name:` 均从各 spec 唯一的原始字段逐字提取；目录名不参与判定（`progress/T2/source_identities_and_names.tsv:1`）。

| 目录 | spec `Name:` 实测值 | Git HEAD |
|---|---|---|
| `askuser` | `askuser-notification` | `120ef50633bf02ae40b504188a812900a9fa96e2` |
| `cynara` | `cynara` | `8aedf3fb6ca31b37d3900ef4588df5ffb760ee55` |
| `libcynara-commons` | `libcynara-commons` | `4b60376089951e8e379cc7a23f7509f433a67525` |
| `libcynara-dbus` | `libcynara-creds-dbus` | `9d2c03fc5744c3fa036e0765a380ebfde1aa6d94` |
| `security-manager` | `security-manager` | `f6a98bfdbf86f48a66feb81e81e4c8b488c9a550` |

五仓开工与结束均为同一 HEAD、工作树与 index 均 clean，结束时逐仓 `git diff --exit-code` 和 `git diff --cached --exit-code` 全为 0（`progress/T2/09_source_readonly_postcheck.log:1`）。未修改任何源码。

## 静态模式与缺陷清单

按 T1 同一模式集扫描五个完整源码树（含 `packaging/`），得到 21 个逐命中：`pthread_cancel` 10、`<cxxabi.h>` 6、`abi::` 3、`__forced_unwind` 2；其中 6 个是 changelog 历史文本，15 个位于活动源码或测试。逐命中含三行上下文（`progress/T2/static_hits.tsv:1`），逐包汇总见 `progress/T2/static_per_package_summary.tsv:1`。没有命中 T1 保守“确定需改”类；这不覆盖下面由 LLVM 22 头声明面对照确认的缺陷。

LLVM 22.1.8 源码包原始 `libcxxabi/include/cxxabi.h` 的非权威最小语法对照中，`abi::__forced_unwind` 探针退出 1，而同头 `abi::__cxa_demangle` 控制例退出 0；对照门 PASS（`progress/T2/08_header_surface_diagnostic.log:1`）。因此清单列出两处同类源码缺陷：

- `libcynara-commons`：`codes/libcynara-commons/src/common/exceptions/TryCatch.h:65`（`LOCAL_ONLY`，`codes/` 禁止上传，SHA256 `592b0adf93bbf1ce300114c9addd2eb213b6789cdc714aa5537bcd25058c1fe9`）；
- `security-manager`：`codes/security-manager/src/common/include/utils.h:70`（`LOCAL_ONLY`，`codes/` 禁止上传，SHA256 `6f3979fdfbc083afa82a0a17c5535c6a5d4da7b6788e149fa8b1b4177010ed4c`）。

该实验用宿主 GCC 13 对当前 LLVM 22.1.8 源码头做 `-fsyntax-only`，明确标为 `NON_AUTHORITATIVE_HEADER_SURFACE_DIAGNOSTIC`，不替代 T3/T4。首次实验因 GCC 不认识 Clang `__has_feature` 且断言脚本错误打印 PASS，失败原文保留；修正后控制例才真正为 0（`progress/T2/08a_header_surface_diagnostic_failed.log:1`）。

缺陷全量表见 `progress/T2/defect_inventory.tsv:1`，类别汇总见 `progress/T2/error_category_summary.tsv:1`：

| 类别 | 次数 | 涉及包 | 涉及文件 | 证据状态 |
|---|---:|---:|---:|---|
| `LIBCXXABI_NO___forced_unwind_DECLARATION` | 2 | 2 | 2 | 非权威头声明面对照确认 |
| `OPTFLAGS_INJECTION_NO_OP_SUSPECTED` | 4 | 4 | 4 | 静态宏流 |
| `BUILD_LDFLAGS_INJECTION_NO_OP_SUSPECTED` | 5 | 5 | 5 | 静态宏流 |
| 整包编译器 TU 诊断 | 0 | 0 | 0 | `NOT_AVAILABLE_MISSING_LIBCXX_DEVEL` |

## 传播面

`libcynara-commons` 的 CMake 安装整个 `common/exceptions/*.h` 目录，spec 的 `cynara-devel` `%files` 也打包 `%{_includedir}/cynara/exceptions/*.h`，故 `TryCatch.h` 是对外安装头（`progress/T2/header_surfaces.tsv:1`）。它在 `libcynara-commons` 内有 13 条 include，在 `libcynara-creds-dbus` 有 1 条跨包 include；逐 TU 原文见 `progress/T2/forced_unwind_propagation.tsv:1`。`askuser` 的同名 `TryCatch.h` 是包内另一份私有头，不含该模式，不并入传播链。

`security-manager` 的 `utils.h` 未出现在安装/公开头规则内，为包内头；五包范围内没有跨包传播，但在本包内有 30 条 include 指令。两条传播链合计 44 条 include 证据，均在同一传播表列明。

`<cxxabi.h>` 的其他命中包括 `abi::__cxa_demangle` 等；控制例已证明该符号在所测 LLVM 22 头声明面存在，故未将这些命中自动升级为缺陷。pthread 取消调用/注释只保留为行为关联清点，不据此给修复结论。

## 注入有效性预检

当前 Base primary 元数据定位的 armv7l `cmake-3.31.2-1.14`、`rpm-4.14.1.1-2.8`、`rpm-build-4.14.1.1-2.8` 均下载后按元数据尺寸/SHA256 验证，`rpm2cpio/cpio` 均为 0（`progress/T2/current_macro_rpms.tsv:1`；`progress/T2/current_macro_rpm_verification.tsv:1`）。从当前 RPM 原文取得的 `%cmake` 在 `CXXFLAGS` 为空时才使用 `%optflags`，不处理 LDFLAGS（`progress/T2/current_macros.cmake:8`）。当前 RPM `%build` prelude 只设置 `RPM_OPT_FLAGS`；`%set_build_flags` 虽能设置 CXXFLAGS/LDFLAGS，但五个 spec 调用的 `%cmake` 不调用它（`progress/T2/current_rpm_macros:822`；`progress/T2/current_rpm_macros:1054`）。

逐包结果见 `progress/T2/injection_effectiveness.tsv:1`：

- `askuser-notification`、`cynara`、`libcynara-commons`、`libcynara-creds-dbus` 都在 `%cmake` 前先 `CXXFLAGS+=...`，列 `OPTFLAGS INJECTION_NO_OP_SUSPECTED`；
- `security-manager` 未预设 CXXFLAGS，`%cmake` 可消费 `%optflags`；
- 五包都在 `%cmake` 前先 `LDFLAGS+=...`，而 `%cmake` 不消费 `%build_ldflags`，五包全部列 `BUILD_LDFLAGS INJECTION_NO_OP_SUSPECTED`；
- 扫描未发现 `CXXFLAGS=`/`LDFLAGS=` 的直接覆盖赋值，但上述 `+=` 在变量原先为空时会使默认注入分支不再触发。

## 诊断构建边界

诊断根为 `tmp/GBS-ROOT/DIAG-20260731`。最初从外层工作仓调用时，GBS 把嵌套五仓误判为外层 repo，五包均在导出前报 `No source package found`；失败原文保留。改为逐仓 cwd 后，五个 source root 均正确，但 D5 buildconf 的全局 `Required: ... libc++-devel` 无法由当前 Base/Unified standard 仓满足，五包均在 `DEPENDENCY_RESOLUTION` 阶段报 `nothing provides libc++-devel`，成功编译包数均为 0（`progress/T2/diagnostic_correct_cwd_status.tsv:1`；构建全程日志 `progress/T2/05_diagnostic_libcynara-commons.log:1` 为 `LOCAL_ONLY`，SHA256 `6723167c00c1fe13cda9068c879bac6924f0c03e62ba78878c7520a66a9389b5`）。

因此没有 TU 编译原文可汇总；未借用已作废的 21.1.1 libc++ 伪造 22.1.8 诊断。根内已写 `RETIRED_DIAGNOSTIC_ONLY` 与 `NON_AUTHORITATIVE` 标记，其 SHA 见 `progress/T2/diagnostic_root_markers.sha256:1`。任务未起草补丁、未修改源码。
