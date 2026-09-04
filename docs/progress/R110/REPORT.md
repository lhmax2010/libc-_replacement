# R110：50 处识别子句站点的逐条清单核对

## 1. 明确结论

“10 个源码 RPM、8 个包名、50 处独立识别子句”这一整句话不能由原始材料支持。
可支持的是：R32 的旧启发式筛出了 50 个 `CONFIRMED_RETHROW` token 行，分布在
10 个源码 RPM 实例、8 个包名。逐行核对后，50 行中只有 46 行是 catch 子句，
另 4 行是 `key-manager` 的日志字符串。

按本任务给出的语法定义重新统计原冻结源码，实际有 175 个识别 catch 子句：
172 个继续传播（46 个裸 `throw;`、126 个 libstdc++ 重抛宏），3 个 Dali handler
不重抛。172 个传播位置按包名/逻辑路径去除两个重复源码 RPM 变体后为 104。
因此没有为了对上 50 而修改口径。

## 2. 原始出处与统计口径

原始出处是：

- 报告：`docs/progress/R32/R32_report.md`；
- 逐 token 表：`docs/progress/R32/tables/forced_unwind_guard_locations.tsv`；
- 按源码 RPM 汇总：`docs/progress/R32/tables/existing_guard_packages.tsv`；
- 扫描器：`docs/progress/R32/tools/scan_impact.py`。

R32 以 T1 的 371 个成功解包源码 RPM 为分母，4 个包级 worker、每个
`rg --threads 1`，限定常见文本后缀。`guard_status()` 只在 token 附近的小窗口找
裸 `throw;`。原表共有 157 个 forced-unwind token 行；取
`guard_status=CONFIRMED_RETHROW` 且 `scope!=DOC` 后正好 50。

该规则有三处与“识别子句”的自然语言定义不相等：不要求当前行是 catch；不认识
libstdc++ 的重抛宏；扩展名白名单漏掉标准库的无扩展名头文件。具体代码与后果见
`METHOD_AND_LIMITS.md`。

## 3. 50 行逐条核对

`tables/reported_50_audit.tsv` 与 `REPORTED_50_AUDIT.md` 逐行给出源码 RPM、包内
路径、行号、实际代码和函数上下文。按包汇总如下：

| 包名 | 源码 RPM 实例数 | 旧表 50 中的行数 | 其中真实 catch 子句 |
|---|---:|---:|---:|
| boost | 1 | 22 | 22 |
| gcc | 2 | 6 | 6 |
| key-manager | 1 | 9 | 5 |
| libcynara-commons | 1 | 1 | 1 |
| llvm | 1 | 6 | 6 |
| security-manager | 1 | 1 | 1 |
| tensorflow2 | 2 | 4 | 4 |
| webauthn | 1 | 1 | 1 |
| **合计** | **10** | **50** | **46** |

四个误计行均为 `key-manager` 的 handler 日志行，位置是：

- `src/manager/client-async/client-manager-async.cpp:322`；
- `src/manager/client-capi/ckmc-manager.cpp:1371`；
- `src/manager/client/client-common.cpp:328`；
- `src/manager/client/client-common.cpp:350`。

## 4. 去重前后说明

R32 所谓 10 个“源码包”实际是 10 个 source RPM NEVR 实例。GCC 两个 Release
变体共享同一 VCS commit，TensorFlow2 两个 Release 变体也共享同一 VCS commit；
按包名归并即为 8。旧 50 行中重复实例造成 5 个重复位置，故旧表自身按包名、
路径和行号去重后为 45。完整 68 组严格传播站点实例重复见
`tables/source_instance_duplicates.tsv`；这里的 68 是 GCC 66 处加 TensorFlow2
2 处，不是新的包数。

“独立站点”若指独立源码位置，应采用去重后的逻辑位置，而不应把同一 Git 内容的
不同 RPM Release 当成不同实现。

## 5. 当前源码复核

当前八包的 URL、分支和 HEAD 在 `tables/current_source_identity.tsv`。只读取得
远端 HEAD 后，普通 Git 文本、LLVM 三个 MLGO tar 和 TensorFlow AOT tar 均进入
严格扫描。结果：

| 包名 | 原冻结源码的去重传播位置 | 当前 HEAD 传播位置 | 变化 |
|---|---:|---:|---:|
| boost | 22 | 22 | 0 |
| gcc | 66 | 66 | 0 |
| key-manager | 5 | 5 | 0 |
| libcynara-commons | 1 | 1 | 0 |
| llvm | 6 | 42 | +36 |
| security-manager | 1 | 1 | 0 |
| tensorflow2 | 2 | 2 | 0 |
| webauthn | 1 | 1 | 0 |
| **合计** | **104** | **140** | **+36** |

LLVM 增加的 36 个是当前 `tizen_base` 中 libc++ 强制展开修复的 34 个头文件本地
handler 和 2 个 `ios.cpp` 汇聚 handler。当前数字不是 50。由于这里只复核历史
阳性包，不是当前全平台源码全集，平台其余包是否新增位置记为
`NOT_OBSERVED`。

## 6. 产物

- 历史 50 行两种格式：`tables/reported_50_audit.tsv`、
  `REPORTED_50_AUDIT.md`；
- 严格传播站点两种格式：`tables/strict_rethrow_sites_original.tsv`、
  `STRICT_RETHROW_SITE_LIST.md`；
- 当前复核：`tables/current_rethrow_sites.tsv`、`CURRENT_RETHROW_SITE_LIST.md`；
- 纯语法 175 子句：`tables/strict_clause_sites_original.tsv`；
- 汇总与身份：`tables/package_summary.tsv`、`tables/audit_metrics.tsv`、
  `tables/current_source_identity.tsv`。

## 7. 自行判断、疑问与边界

本轮作出的口径判断只有两项：把“识别子句”严格限定为实际 catch 语法；把
“正确防护”限定为可观察到裸重抛或 libstdc++ 标准重抛宏。两项均单独保留数据，
没有用其中一个替代另一个。

尚存疑问是：总部所说的 `processing distinct sites` 究竟想表达“所有识别
catch”、还是“识别并继续传播的 handler”。两种严格答案分别为原冻结实例
175 和 172；无论哪一种都不是 50。建议人工对外时先确认术语，但不应再把旧 50
称为独立子句。

本轮没有修改源码、配置、补丁或历史判定材料，没有推送 Gerrit。当前全平台全集
复扫未执行，明确记为 `NOT_OBSERVED`。
