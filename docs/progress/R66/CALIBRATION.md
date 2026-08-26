# R66 方法校准

## 口径

本轮先校准方法，未把四项已知缺陷的标识符写入检索规则。校准脚本只识别三类结构：

1. 标准库实现中“捕获具体类型并重抛”的防御性结构，以及另一实现中是否出现同一类型末级 token；
2. libc++ 构建的编译/链接诊断中“实现成员不存在”和 `std` ABI 命名空间不匹配；
3. 多 ELF 中同名 RTTI 定义重复、且并非全部进入 `.dynsym` 的形态。

脚本为 `docs/progress/R66/code/calibrate_methods.py`。其规则中不含四项已知结果的专名。

## 校准结果

| 已知差异类别 | 机械方法 | 未注入答案时的实际命中 | 结果 | 证据 |
|---|---|---|---|---|
| 标准库自身防御缺失 | 对 libstdc++ 全树抽取 typed catch-and-rethrow，再检查 libc++ 是否有同类型 token | 缺少 peer token 的候选中出现 65 个同类型防护位置 | REDISCOVERED | `tables/calibration_defensive_candidates.tsv`；命令 006 |
| 实现私有成员直接引用 | 对真实 libc++ 构建诊断做结构解析，并从诊断源码行抽取末级成员 | 找到 `_S_badbit` 与 `__filebuf_type` | REDISCOVERED | `tables/calibration_diagnostics.tsv`；命令 009 |
| 第三方预编译库 ABI 不匹配 | 对链接失败逐符号识别 `std` ABI 命名空间 | Boost.Test 未定义引用携带 `std::__1`，暴露消费者与预编译 DSO 的标准库 ABI 边界 | REDISCOVERED | 同上；命令 009 |
| typeinfo 跨边界不统一 | 对同一实验的消费者/DSO 做 RTTI 符号表和动态符号表二维检查 | 同名 `_ZTI*`/`_ZTS*` 在多个对象中均有定义，R38 消费者定义未进入 `.dynsym` | REDISCOVERED | `tables/calibration_rtti.tsv`；命令 008 |

## 方法能力边界

- 源码 token 扫描本身不能证明预编译 DSO 的 ABI，也不能证明动态加载后的 RTTI 实际绑定；这两类必须分别由链接诊断/ELF 和运行时加载证据覆盖。
- 编译诊断能发现“某成员不存在”，但只有再回查两套标准库头文件的归属，才能定性为 libstdc++ 私有扩展；诊断解析不单独承担所有权判断。
- RTTI 静态形态是风险发现器，不单独证明运行时故障；运行时绑定或单变量链接对照仍是最终定性手段。
- typed-catch 差分以末级 token 作为低成本对应性检查，存在同名异义和实现采用不同机制的假阴性/假阳性；全量阶段必须逐项读代码复核。

四项均可由至少一种机械方法重新发现，未出现“任何机械方法都无法找到”的校准失败。因此无需在全量扫描前停报；后续采用组合方法，并分别申报各层分母与盲区。

## 输入身份与异常记录

- 371 包分母来自 `progress/T1/extraction_results.tsv` 中 `EXTRACTED_OK`；目录树另含提取失败的 `chromium-efl-1.1.144-1.src.rpm`，不计入分母。
- `commands/003_freeze_inputs.log` 中对两个不存在路径的 SHA256 尝试失败，但外层复合命令末项成功而记录退出 0；正确文件随后定位为 `docs/progress/R39/R39_report.md` 与 `docs/progress/R40/R40_report.md`。该次尝试不作为成功取证。
- `commands/004_calibration_evidence_inventory.log` 的宽范围 `find` 扫描耗时过长，被人工终止且日志无正常尾标；后续改用明确文件路径。该日志只登记尝试，不作为结论证据。
