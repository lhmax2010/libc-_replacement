# P1 出口非阻塞限制条件 / 开放问题

评审日期：2026-07-27

## 1. D5 机制边角未覆盖

- 阴性对照只验证了一个 C++ 包的命令字节与 RPM payload 逐字节一致；未覆盖：
  - 含静态归档（`*.a`）的 source package；
  - 使用自定义 ld 脚本或 `-nostdlib` 的包；
  - 同一 source RPM 下同时产出 C-only 与 C++ 子包的情况。
- 这些场景可能在 allowlist 注入 `-lc++ -lc++abi` 后引入非预期 NEEDED 或链接失败。
- 建议：在 D5 正式落地前补充上述至少一个代表性负面对照。

## 2. 门禁矩阵未覆盖所有工具内部 RED 码

- 13 项 fixture 矩阵覆盖了 `gates/fixture_catalog.tsv` 中列出的 10 RED / 3 GREEN 场景。
- 工具代码中还存在以下 RED 码未在 fixture 中验证：
  - `tunnel_registry_check.py`：`AMBIGUOUS_REGISTRY_ENTRY`、`REGISTRY_NOT_SIGNED`、`REGISTRY_EXPIRED`
  - `g7_exception_gate.py`：`AST_EVIDENCE_INCOMPLETE`、`RIDER_CARDINALITY_INVALID`、`RIDER_NOT_EXECUTED`、`CLOSURE_LEAK_REMAINS`、`PARTIAL_RESULT_ACCEPTED`
- 这些码不是当前 catalog 的声明项，但属于 fail-closed 合约的一部分；如果未来有人改动工具逻辑，可能引入回归。
- 建议：非阻塞，可在后续门禁维护迭代中补齐 fixture。

## 3. wave4 通知的 fallback 细节

- 通知兜底方案为“双运行时镜像”。但双运行时意味着部分域用 libc++、部分域用 libstdc++，跨域 C++ ABI 边仍可能断裂。
- 通知未明确 fallback 也需要满足“跨域 C++ 边为零”或“通过 bundle compat 等桥接隔离”。
- 建议：非阻塞，但在正式通知 owner 前补充一句“fallback 仍需通过边差分门验证无跨域 C++ ABI 边”。

## 4. MANIFEST 项数与提示口径

- 提示写 170 项；实际 `MANIFEST.sha256` 168 项（全部校验通过），`SOURCE_MAP.tsv` 156 行。
- 可能是概数或目录计数口径不同，不影响内容完整性。
- 建议：在 README 中明确“170 为概数，实际清单以 MANIFEST.sha256 为准”。

## 5. D5 与 S6 重新冻结的协调窗口

- D5 落地后 S6 必须重开并重新冻结。排期 8 月同时包含 D5 落地与波 1 开工。
- 建议：把“S6 重新取证并冻结”作为 8 月 D5 落地交付物的显式子项，避免波 1 在 S6 未冻结时开工。

## 6. 证据缺口（EVIDENCE_GAP）

- D5 的三架构完整 G1–G5 对照证据未在 corpus 中落盘；`d5/contrast_verdict.tsv` 只给出 case-level PASS，未展开 aarch64/x86_64 的 G1–G5 原始 `LD_DEBUG=bindings` 日志。
- 建议：在 `p1/d5_allowlist/evidence/` 中补充 aarch64/x86_64 的 G1–G5 原始日志，或在本 repo 中增加索引。
