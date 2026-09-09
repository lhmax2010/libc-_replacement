# Unified 编译失败实际日志分析

## 结论

结论：**`PARTIAL`**。

QuickBuild build `1165447` 中，指定的 17 个包均取得至少一份失败日志，
可得架构的直接失败点均已分类。四个 GCC override 包的假设得到实际日志
支持：`lapack`、`openblas`、`onnxruntime`、`yaca` 均因全局注入的
`-stdlib=libc++` 进入 GCC/Fortran driver 而失败。

但原问题指向 build `1165448` 的 armv7l 轮次，该日志已经不可得；
`1165447` 提供的是 `standard-aarch64` 与 `standard-x86_64` 失败日志。
因此本文不能把两种架构的证据转述为 armv7l 实测。另有 `hdf5` 缺少
`config.log` 底层探针记录，根因保持未定；aarch64 GCC 的三个非
`-stdlib` 链接选项逐项本地探针也因 driver 运行故障保持
`NOT_OBSERVED`。

## 日志取得与身份

- 工具：`tools/quickbuild_logs.py`；凭据只由工具从被忽略且权限为 600 的
  凭据文件读取，未进入命令行或本报告。
- 列表命令与退出码：`raw/003_list_build_1165447.*`，退出码 0；列出
  1,059 个日志行，其中 41 个失败行、22 个去重失败包。
- 下载命令：首次网络超时见 `raw/004_*`（退出码 2），同参数重试见
  `raw/005_*`（退出码 0）。
- 下载结果：manifest 记录 41 个失败日志，共 67,531,114 字节，位于被忽略的
  `logs/quickbuild/build_1165447/`，**未提交**。
- 本任务 17 包对应 32 条失败日志记录（`lapack` 的页面列表含重复条目）；
  逐文件大小和 SHA256 见 `evidence/log_inventory.tsv`。
- 仅提交每个可得架构的一行直接证据摘录，见
  `evidence/log_excerpts.md`。
- 提交前对全部暂存文件执行凭据值与认证 header 扫描；最终扫描 76 个
  暂存文件，命中数 0，结果为 `CREDENTIAL_SCAN=PASS`。扫描器从凭据文件
  读取比对值但从不打印值；过程及一次规则自命中的技术性修正见
  `raw/014_*`、`raw/015_*` 与 `raw/014_recovery_note.md`。

build `1165447` 实际共有 22 个去重失败包，比本任务给定的 17 包多
`dynpart-tools`、`lmdb`、`oprofile`、`valgrind`、
`vulkan-validation-layers` 五个。本轮严格按指定 17 包分类，不把五个额外
包并入分母。

## 17 包逐项结果

| 包 | 可得失败日志架构 | 类别 | 直接观测到的失败 |
| --- | --- | --- | --- |
| `lapack` | aarch64、x86_64 | 注入选项进入 GCC | CMake 的 Fortran compiler link test 中，`gfortran` 明确拒绝 `-stdlib=libc++` |
| `openblas` | aarch64、x86_64 | 注入选项进入 GCC | 共享库链接命令中的目标 `gcc` 明确拒绝该选项 |
| `onnxruntime` | aarch64、x86_64 | 注入选项进入 GCC | CMake C compiler test 的目标 `gcc` 明确拒绝该选项 |
| `yaca` | aarch64、x86_64 | 注入选项进入 GCC | CMake C compiler test 的目标 `gcc` 明确拒绝该选项 |
| `tidl` | aarch64、x86_64 | 环境/依赖 | Clang 已编译到 60%，链接 `tidlc` 时 `ld` 找不到 `-lc++`；日志不能进一步证明搜索路径为何缺库 |
| `noise-suppression` | aarch64、x86_64 | 源码兼容 | `NoiseSuppression.cpp` 的 `std::copy` 不可见；不能仅凭日志把它进一步写成已证实的缺失 include |
| `tensorflow2` | aarch64、x86_64 | 迁移依赖/ABI | external-project 的 `flatc` 对象留下大量 `std::__cxx11` 未定义引用，而最终链接明确选择 libc++ |
| `openusd` | aarch64、x86_64 | 源码兼容 | `half.h` 使用未限定的 `size_t`，Clang/libc++ 要求 `std::size_t` |
| `protobuf` | aarch64、x86_64 | 迁移依赖/ABI | `libprotobuf` / `libprotoc` 对含 `std::__1` 类型的 Abseil 接口未解析 |
| `lottie-player` | aarch64、x86_64 | 源码兼容 | libc++ `<ostream>` 间接包含 `<format>` 时命中项目自己的 `../format` 文件，其首行是 `git` 命令并被当成 C++ 解析 |
| `libcynara-commons` | aarch64、x86_64 | 源码兼容 | 测试源码忽略 `nodiscard` 返回值，`-Werror` 将告警升级为错误 |
| `rive-tizen` | aarch64、x86_64 | 迁移依赖/ABI | 捆绑 Skia 对象留下 `std::__cxx11` / `std::__throw_*` 等 libstdc++ ABI 未定义符号 |
| `glog` | aarch64、x86_64 | 源码兼容 | 单元测试使用未声明的 `auto_ptr` |
| `c-mock` | aarch64、x86_64 | 迁移依赖/ABI | 测试对象要求含 `std::__cxx11` 类型的 GoogleTest 接口 |
| `docker-launcher` | aarch64 | 迁移依赖/ABI | 链接要求含 `std::__1::basic_string` 的 JsonCpp 接口，现有库未解析；本 build 无 x86_64 失败日志，不能推断该架构状态 |
| `hdf5` | aarch64、x86_64 | 其他/未定 | configure 最终称 Fortran compiler 不提供 `SIZEOF` 或 `STORAGE_SIZE`；无 `config.log`，不能确认是不是注入参数造成 |
| `hailo-rt` | aarch64 | 源码兼容 | 捆绑 spdlog/fmt 对 `char8_type` 实例化未定义的 `std::char_traits`；本 build 无 x86_64 失败日志 |

机器可读逐项表在 `tables/package_failure_classification.tsv`。

## 分类计数

| 主类 | 包数 | 包 |
| --- | ---: | --- |
| 注入选项打到 GCC 路径 | 4 | lapack、openblas、onnxruntime、yaca |
| 源码兼容 | 6 | noise-suppression、openusd、lottie-player、libcynara-commons、glog、hailo-rt |
| 迁移依赖/标准库 ABI 不匹配 | 5 | tensorflow2、protobuf、rive-tizen、c-mock、docker-launcher |
| 环境或依赖 | 1 | tidl |
| 其他、现有日志不足以定根因 | 1 | hdf5 |

合计 17。分类只描述失败性质，不给修复或优先级建议。

## 四个 GCC 包的明确核对

四包在 aarch64 与 x86_64 的日志均同时出现：

1. 环境导出的完整 `CXXFLAGS` / `LDFLAGS` 中含注入组；
2. 实际命令使用 `gfortran` 或目标前缀 `gcc`；
3. driver 直接报 `unrecognized command-line option '-stdlib=libc++'`。

所以在 build `1165447` 的可得范围内，因果链已由实际命令闭合，不是静态
检索推断。全部组选项给 GCC 时，最先明确失败的是 `-stdlib=libc++`；
三个 linker 选项并非此次四包的直接拒绝点。

## 选项探针

完整说明见 `OPTION_PROBE.md`，结构化 30 格见
`tables/trace_option_probe.tsv`。摘要：

- armv7l、x86_64 GCC 14.2.0 明确拒绝 `-stdlib=libc++`；其余三个链接
  选项单独使用时被接受并进入 linker argv；全组在 `-stdlib` 处失败。
- 三架构 Clang++ 22.1.8 接受 `-stdlib=libc++`，并实际选择 `-lc++`；
  其余链接选项均被转交。
- aarch64 GCC 本地复用 driver 在 `--version` 即退出 139，逐项探针不得
  判读；但 build `1165447` 的 aarch64 GCC 实际命令已独立证明
  `-stdlib` 的明确拒绝。
- 已完成范围内未发现“静默忽略”；Clang++ 是静默接受且实际切换，不能
  归为忽略。

## 未观测、不可得与自行判断

- `NOT_AVAILABLE`：build `1165448` armv7l 失败日志；因此 17 包在原
  armv7l 轮次的逐包同因性未核实。
- `NOT_OBSERVED`：aarch64 GCC 三个非 `-stdlib` 链接选项的独立本地
  trace；复用 driver 自身运行失败。
- `NOT_OBSERVED`：`hdf5` 的 `config.log` 底层失败命令，故不把最终
  Fortran 能力错误自行归因到全局注入。
- 自行判断仅限把日志中直接显示的 `std::__1` / `std::__cxx11` 未解析
  接口归为“迁移依赖/ABI”，以及按失败发生位置归入源码、环境或其他类；
  表中逐项保留了不能由日志证明的边界。
- 未实施任何修复，未给处置建议，未修改平台源码或配置。
