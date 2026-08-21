# W3 / R63：例外形态消费者审计

## 结论

口径为既有、已解包且状态为 `SCAN_OK` 的 371 个 SRPM 身份（347 个去重包名），不下载、不构建、不扫描平台仓。宽口径文本扫描产生 45 个自动“确认”身份，但人工复核证明该自动标签不可直接使用：大量命中只是工具链能力定义、测试配置或泛型模板参数。

复核后确认 3 个例外消费者（分母 371，0.81%）：`capi-context-motion` 与 `ncnn` 启用 C++ LTO，`boost` 使用具体的 `basic_ostream<char16_t/char32_t>` 特化。另有 2 个条件命中：`app-installers` 与 `tpk-backend` 只在 Release/空 `CMAKE_BUILD_TYPE` 下启用 LTO，而既有材料未保存 `%{build_type}` 的实际展开值，因此不得判作已确认或未命中。

确认例外触及 R47 的 `S16`、`S17`、`S33` 三个库内站点，按任务给定规则转入头文件批后，34/16 变为 **31/19（确认下界）**。条件包中只有 `app-installers` 观察到命中 `S08`；若其实际按启用 LTO 的配置构建，则变为 **30/20**。因此本次只读语料不能给出唯一精确值，精确结果记 `NOT_DETERMINED`，而不是在 31/19 与 30/20 之间猜测。

## 数据来源与扫描口径

- 分母：`progress/R32/tables/scan_status.tsv` 中 371 行 `SCAN_OK` SRPM 身份；去重包名 347。
- 语料：`tmp/corpus/extracted/<source_rpm>/` 的既有解包内容。
- 基线：R47 的 34 个非汇聚站点分类表；其中 18 个原为库交付、16 个原为消费者头文件。加 `ios.cpp` 的 16 个汇聚站点后形成落章的 34/16。
- 宽扫描：C/C++ 源与头、spec、CMake/Meson/Autotools/GN/Bazel 等构建文件；排除路径名明确为 test/tests/testing/testsuite、benchmark、doc、example/sample 的目录。
- 复核：只有平台 spec 能到达的无条件设置、或 spec 明确开启的架构条件，才记 `CONFIRMED`。仅定义能力、默认关闭、其他操作系统/工具链、测试配置、注释均不计。非标准字符只接受具体类型（本次为 `char16_t/char32_t`），不把 `Char`、`CharType`、`Ch` 等模板参数当作具体特化。
- 输入 SHA256 见 `tables/input_sha256.tsv`。原始候选保留在 `tables/raw_hits.tsv`，最终判定以 `tables/reviewed_exception_consumers.tsv` 为准。

## 五类结果

| 条件 | 已确认 SRPM | 条件 SRPM | 分母 | 说明 |
|---|---:|---:|---:|---|
| 静态链接 libc++ | 0 | 0 | 371 | 12 条原始候选均为 provider/test、WASI 或非 Tizen 工具链材料 |
| LTO | 2 | 2 | 371 | 确认 `capi-context-motion`、`ncnn`；条件为 `app-installers`、`tpk-backend` |
| 同名符号插入 | 0 | 0 | 371 | 未观察到活动的显式实例化或同名定义 |
| 关闭 extern-template | 0 | 0 | 371 | 两个禁用宏均零命中 |
| 非标准字符特化 | 1 | 0 | 371 | `boost` 的 10 行具体 `char16_t/char32_t` 流证据 |

逐包证据、架构/条件范围与站点见 `tables/reviewed_exception_consumers.tsv`；自动假阳性收敛过程见 `tables/automatic_false_positive_review.tsv`。

## 逐包事实

- `capi-context-motion`：顶层 CMake 对目标编译和链接无条件加入 `-flto`，spec 直接执行该 `%cmake`。扫描未观察到 R47 34 个站点的明确调用，故它计入例外消费者数，但不据此移动站点。
- `ncnn`：spec 对 aarch64/i586/x86_64 明确设置 `NCNN_ENABLE_LTO=ON`，对其他架构设为 OFF；源码观察到算术输入，触及 `S16/S17`。
- `boost`：Boost.Log 有 10 行具体 `basic_ostream<char16_t/char32_t>` 声明/定义；相关实现调用 `write`，后者构造该特化的 `basic_ostream::sentry`，故 `S33` 不再由 `char/wchar_t` 的库内显式实例化覆盖。
- `app-installers`：C++ Release flags 含 `-flto`，观察到 `std::ifstream::read`（`S08`）；实际 `%{build_type}` 未保存，故为条件命中。
- `tpk-backend`：C++ Release flags 含 `-flto`，实际 `%{build_type}` 未保存；生产源码未观察到对应层 C 站点，故即使 LTO 生效也不据此移动站点。

## 影响评估

| 场景 | 库内批 | 头文件批 | 状态 |
|---|---:|---:|---|
| R47 基线 | 34 | 16 | 已有实测基线 |
| 只计确认例外 | 31 | 19 | `S16/S17/S33` 转入头文件批 |
| 同时计入条件配置 | 30 | 20 | 再加入 `app-installers` 的 `S08` |
| 平台精确值 | `NOT_DETERMINED` | `NOT_DETERMINED` | 缺实际 `%{build_type}` 展开 |

本表按站点去重，不按消费者包数重复计数；两批分母始终为 50 个层 C 站点，库内批与头文件批之和均为 50。

## 无法覆盖的情形

- 未保存的宏展开、实际 configure/CMakeCache、编译数据库和最终链接命令，导致两个条件包不能定性。
- 既有语料没有迁移后 libc++ 消费者 ELF；源码扫描无法验证链接器最终选取、LTO 后的实际内联、符号插入或隐藏符号。
- 运行时 `dlopen` 选择、外部生成源码、下载期生成物、私有未解包源码不可见。
- 泛型模板声明只证明“可接受某字符类型”，不证明平台实际实例化了非标准字符，因此全部从确认计数剔除。
- 文本级设施识别可能漏掉宏、别名和封装调用；无法明确映射到 R47 站点的包只计消费者例外，不移动站点。

## 过程异常申报

最初两个扫描版本因逐包重复启动 `rg` 或未过滤大型测试/文档树而耗时过长，被显式终止；退出与终止记录完整保留在 `commands/003*`、`commands/004*`。最终扫描使用单次全语料 `rg`、2 线程及目录过滤，退出 0。未把被终止的输出用于最终计数。
