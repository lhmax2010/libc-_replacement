# llvm/libcxx 与 gcc 多 spec 场景差异

## 内容关系

gcc 的五份 spec 内容同源：四份派生 spec 等于两行架构宏前缀、一个空行与完整 `gcc.spec` 的拼接。llvm 场景的 `llvm.spec` 与 `libcxx-runtimes.spec` 是两份独立的完整打包描述，子包、BuildRequires、构建目标、安装清单和 changelog 均不同，不能由给 `llvm.spec` 增加宏前缀机械生成。

由此产生的事实影响：

1. 两份 spec 必须分别维护和解析，不能用 gcc 的前缀生成算法保证正文同步。
2. 唯一必须同步的版本约束由本任务单独断言，而不是由拼接天然继承。
3. 两个目标必须各自携带关联输入；libcxx 目标需要 `libcxx-llvm22.map`、`libcxx-runtimes.manifest` 和 `SOURCE_PROVENANCE`。
4. `llvm.spec` 的既有 MLGO 模型、`llvm.manifest` 和其他内容保持原样；libcxx 文件只做同目录并存。
5. QuickBuild 是否实际接受此类“独立内容的两个 spec”仍待既有 sandbox 分支验证，本地 `rpmspec` 不能替代该项平台验证。

## 是否需要 pre_checkin.sh

结论：`NO_GENERATOR_REQUIRED_FOR_R18`。

依据是 gcc 脚本的唯一变换为“添加机械前缀后拼接同一基准正文”，而我方两份 spec 没有这样的生成关系。若新增脚本，它只能复制两个彼此独立的源文件或重新表达其内容，既没有 gcc 对应的确定变换，也会额外引入需同步的第三份来源。因此本次不新增 `pre_checkin.sh`、`_multibuild`、`_service` 或其他配套文件。

## Name 与 Version

原文见 `commands/10_raw_name_version_check.txt`，断言见 `commands/13_local_selfcheck_summary.txt`：

| spec | Name | Version | 版本宏 |
|---|---|---|---|
| `llvm.spec` | `llvm` | `22.1.8` | `%define llvm_version 22`（主版本宏） |
| `libcxx-runtimes.spec` | `libcxx-runtimes` | `22.1.8` | `%define llvm_major 22`；`%define llvm_version 22.1.8` |

两份 Name 不同、Version 相同。`libcxx-runtimes.spec` 的 Version 行为硬编码 `22.1.8`，同时定义同值 `%llvm_version` 供 Source0 和 `%setup` 使用。

## SOURCE_PROVENANCE 字段边界

按任务要求，四个输入必须与 R9-C seg2 构建输入逐字一致，因此 `SOURCE_PROVENANCE` 保留 SHA256 `1cd5a45e…`，其中 `local_tizen_base_vcs` 仍指向最后一个源代码变更提交 `3d3e3da…`。本次新提交 `5ed6c772…` 只新增 packaging 文件，不改变 LLVM 源码树；两者的区别在审阅材料中显式申报，不推断 QuickBuild 导出结果。
