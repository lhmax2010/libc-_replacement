# W3：Unified armv7l 编译失败初步归类

## 结论

`PARTIAL`。

已完成不依赖总部日志的编译器选项探针，并把 17 个失败包按现有静态证据列出候选类别。但当前工作区没有总部本轮 Unified armv7l 构建的逐包失败日志，因此 17 个包的确切失败原因全部为 `NOT_OBSERVED`，不能判为 `CLASSIFIED`。

逐包表见 `tables/failure_classification.tsv`。

## 1. 输入身份与日志可得性

### Unified 配置改动

本地原始配置：

- `codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config`
- SHA256：`a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086`

该只读原文件尚未包含迁移注入。实际拟应用的改动来自已经验证的：

- `docs/progress/R84/unified_project_config.patch`
- SHA256：`22ed0a3f6b75bff9b9bd3924e81c8eecc0434c4ff2e3495c3e632484de374547`

注入内容为：

```sh
export CXXFLAGS="${CXXFLAGS} -stdlib=libc++"
export LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed"
```

证据见 `raw/009_unified_patch_content.*`、`raw/010_current_unified_config_context.*`、`raw/028_exact_config_patch_hash.*`。

### 当前构建日志

在工作区顶层、既有 `docs/progress/R99/` 位置及可见交付候选中均未取得这次“449/1034”构建的 17 份逐包日志；`docs/progress/R99/` 本身不存在。因此：

- 17 包实际错误文本：`NOT_AVAILABLE`；
- 失败阶段：`NOT_OBSERVED`；
- 是否由全局注入直接导致：`NOT_OBSERVED`。

没有使用旧快照的构建日志替代本轮证据。

## 2. 编译器选项探针

### 2.1 环境

使用既有 Tizen x86_64 GBS buildroot：

`tmp/GBS-ROOT/R104-matrix-x86_64-libcxx/local/BUILD-ROOTS/scratch.x86_64.0`

实际工具：

- GCC：Tizen GCC 14.2.0；
- Clang：22.1.8，target `x86_64-tizen-linux-gnu`。

探针只调用 driver、语法编译和单个目标文件编译，没有执行包构建。

### 2.2 结果

| 工具与选项 | 结果 | 证据 |
|---|---|---|
| GCC，未注入 | PASS | 最小 C++ 输入语法检查退出 0 |
| GCC，`-stdlib=libc++` | FAIL | `g++: error: unrecognized command-line option '-stdlib=libc++'` |
| GCC，仅 linker 组选项 | 接受并转发 | `-###` 中出现 `--no-as-needed -lc++abi --as-needed`，同时仍自动加入 `-lstdc++` |
| Clang，未注入 | PASS，默认 libstdc++ | `-###` 使用 GCC C++ include，并自动加入 `-lstdc++` |
| Clang，完整注入 | PASS，切到 libc++ | 使用 `/include/c++/v1`，link trace 出现 `--no-as-needed -lc++abi --as-needed` 和 `-lc++` |

原始记录：`raw/018_*` 至 `raw/025_*`。

### 2.3 “静默切换”阳性验证

为避免仅凭 driver trace 下结论，又用同一份含 `std::string` 的源文件生成两个目标文件：

- Clang 默认对象出现 `std::__cxx11`，不出现 `std::__1`；
- Clang 加 `-stdlib=libc++` 的对象出现 `std::__1`，不出现 `std::__cxx11`。

断言全部通过，见 `raw/031_*` 至 `raw/035_*`，临时文件已在 `raw/036_cleanup_symbol_probe.*` 对应步骤删除。

因此在本次实测环境中：

- GCC 对关键编译选项是明确失败，不是静默忽略；
- Clang 不报错，但产物 ABI 命名空间实际改变，属于“静默接受并生效”；
- LDFLAGS 中的 linker 组选项本身可被 GCC 转发；但完整配置还会通过 CXXFLAGS 注入 GCC 明确拒绝的 `-stdlib=libc++`，所以完整 GCC C++ 编译路径不会静默成功。

本探针直接证明编译器行为，不证明任何一个具体包在总部本轮构建中的实际失败点。

## 3. 17 包的静态候选归类

### 3.1 全局 Clang 选项进入 GCC 路径的候选

现有静态材料把下列 4 包标为 `_toolchain_override gcc`：

- `lapack`
- `openblas`
- `onnxruntime`
- `yaca`

探针已证明这类路径只要实际收到 `-stdlib=libc++` 就会失败。但因缺本轮日志，对这 4 包只能记为 `GLOBAL_CLANG_OPTIONS_TO_GCC` 候选，不能写成已确认失败原因。

### 3.2 既有 45 个需改源码包的成员

按固定表 `docs/progress/R19_21/R21/tables/t1_hard_45_packages.tsv` 的源码包名口径，17 包中有 7 个包名命中：

- `onnxruntime`
- `tensorflow2`
- `openusd`
- `protobuf`
- `libcynara-commons`
- `glog`
- `hailo-rt`

其中 `onnxruntime` 同时属于 GCC override 候选，两个原因不能在无日志时二选一。

该表是历史固定快照的源码兼容性材料；它能证明包曾属于需源码适配集合，不能证明本轮具体失败就是相同源文件或相同原因。

### 3.3 当前无可用候选证据

以下 7 包既不在上述 GCC override 四包中，也未命中固定 45 包表：

- `tidl`
- `noise-suppression`
- `lottie-player`
- `rive-tizen`
- `c-mock`
- `docker-launcher`
- `hdf5`

统一标为 `UNDETERMINED`。没有把“未命中旧表”解释成不存在源码问题。

### 3.4 基础设施类别

任务输入说明整轮构建在 449/1034 后因调度器异常退出，余下 585 包未构建。这是本轮覆盖范围限制；当前没有证据把 17 个已列出的包中任一个具体失败归为基础设施原因。

## 4. 计数汇总

候选类别可重叠：

| 候选类别 | 包数 | 说明 |
|---|---:|---|
| GCC override + 全局选项冲突 | 4 | 机制经独立 probe 证实，逐包原因未由本轮日志证实 |
| 固定 45 包源码适配集合成员 | 7 | 历史静态证据；其中 1 个与上一类重叠 |
| 两类候选的并集 | 10 | `4 + 7 - 1` |
| 无上述候选证据 | 7 | 仍需当前日志，不能推断安全 |
| 已由当前日志确认原因 | 0 | 日志不可得 |

## 5. 方法边界与自行判断

### 自行判断

- 选择已有 libc++ 验证 buildroot做选项探针，因为其中同时存在 Tizen GCC 14.2.0、Clang 22.1.8 与 libc++ 头文件，可在同一环境做受控对照。
- 将固定 45 包表按去掉版本后的源码包名匹配；`tensorflow2` 的多个 source RPM revision 合并为一个包名。
- 把“编译器机制已证实”和“逐包失败原因已证实”严格分开。

### 尚存问题

要把本报告从 `PARTIAL` 收口为 `CLASSIFIED`，需要人工提供总部本轮 Unified armv7l 构建中 17 个包各自的完整构建日志，并带有能识别该轮快照/任务的元数据。取得后应逐包记录首个实质错误、编译器、命令行和失败阶段。

## 6. 技术性异常

最初用于按 17 个包名循环枚举 `docs` 与 `tmp` 文件名的命令会重复遍历大目录，已主动终止，未使用其不完整输出；随后改为检查明确的 R99 位置、工作区顶层候选以及已知表。该异常不改变日志不可得的结论。

## 7. 边界确认

- 未修改 project_config、平台源码或任何包；
- 未执行完整包构建；
- 未提出修复或处置建议；
- 未推送 Gerrit 或任何外部源码仓。
