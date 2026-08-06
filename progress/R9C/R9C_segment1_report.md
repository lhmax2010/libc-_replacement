# R9-C 第 1 段：Source0 锚定方案修订前的构建影响取证

日期：2026-08-06  
执行范围：只读取证，不构建、不 export、不修改 `codes/llvm`、spec、配置或制品。  
最终结论：`NO_BUILD_IMPACT`（严格限定于 `libcxx-runtimes-22.1.8` 构建消费范围）。  
阶段状态：第 1 段完成；按任务书停止，未进入第 2 段。

## 1. `.git_archival.txt` 两版实测

完整三方字段和 SHA 见 `archival_versions.tsv`，原文分别见 `inputs/*.git_archival.txt`。

| 来源 | 归档 SHA256 | 文件 SHA256 | `describe-name` |
|---|---|---|---|
| 平台冻结 tarball | `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e` | `ff3062a8c3a6bc5b817afe7ab58ef9dc162f06fec4c5d3f25a281776091349f2` | `%(describe:tags=true,match=llvmorg-*[0-9]*)` |
| 本机标准导出 tarball | `1f5fe8da0a18255a63cde36f1570c88265bb8ffc6458deae06d04e77abc6f7d2` | `4262cb68194f38e3a11aa48b55a177067f868ee414ea955728018971e6d69b2b` | `llvmorg-14-init-170484-gca7933e47d3a` |
| 上游官方 release | `922f1817a0df7b1489272d18134ee0087a8b068828f87ac63b9861b1a9965888` | `c15a4452dd41aae4140e41918ba8871b53d3db3c4c29b4bb8ea69ba9588ee4db` | `llvmorg-22.1.8` |

平台版与本机版逐行 diff 在 `inputs/platform_vs_local.git_archival.diff`。仓内属性原文为：

```text
codes/llvm/.gitattributes:1:clang/bindings/python/.git_archival.txt  export-subst
```

此处只记录内容与属性事实，不对平台 tarball 的生成来路作结论。

## 2. 是否进入构建

### 2.1 源码引用与版本生成机制

对 `codes/llvm` 全树检索文件名，排除文件自身后仅有上述 `.gitattributes:1` 一处命中；CMake、Python 和脚本中没有消费者。逐处结果见 `source_reference_evidence.tsv`，带行号的完整原文见：

- `raw/version_from_vcs_numbered.stdout.txt`
- `raw/generate_version_from_vcs_numbered.stdout.txt`
- `raw/find_vc_file_numbered.stdout.txt`
- `raw/version_generation_callers.stdout.txt`

`GenerateVersionFromVCS.cmake:38-57` 的来源顺序是强制 VC 变量、命名 VC 变量、再调用 `get_source_info`；`VersionFromVCS.cmake:6-75` 的 `get_source_info` 读取 Git 的 `rev-parse HEAD` 和 remote URL。`AddLLVM.cmake:2621-2649` 跟踪 Git 的 `logs/HEAD`。三处均没有读取 `.git_archival.txt`。

### 2.2 R6 实际消费记录

R6 成功构建的 Source0 和解包后的源码树中确实存在该文件；这只能证明“随源码包存在”，不能证明“被构建消费”。对 R8 已从 R6 Ninja/日志提取的实际输入集合重新检索，结果如下：

| 实际记录 | 数据行 | `.git_archival.txt` 命中 |
|---|---:|---:|
| 编译 TU | 78 | 0 |
| 对象依赖边 | 32,217 | 0 |
| CMake 输入 | 43 | 0 |
| 去重消费源码路径 | 25,657 | 0 |
| R6 完整构建日志 | — | 0 |

生成这些集合的原方法与原报告摘录已逐字复录到 `raw/r8_consumption_method_original.stdout.txt`；本次零命中断言见 `raw/build_consumption_zero_hit_assertion.stdout.txt`。完整逐项表见 `build_input_consumption.tsv`。

### 2.3 R9-B 记录边界

R9-B 没有实际构建记录可供追加核对：`progress/R9B/execution_status.tsv:5-7` 三个架构均为 `NOT_RUN_COMMON_INPUT_RED_STOP`，原文明确为“no buildroot created; no compiler invoked”。本次将它登记为 `NOT_AVAILABLE`，没有用 export-only 记录冒充构建消费记录。

### 2.4 三选一结论

`NO_BUILD_IMPACT`。

依据是：文件名除 `export-subst` 属性外无代码/脚本引用；LLVM 的版本生成机制不读取该文件；R6 实际 TU、头依赖、CMake 输入和完整日志全部零命中。结论仅覆盖当前 `libcxx-runtimes-22.1.8` 构建；不外推到可能由外部工具直接检查源码归档元数据的场景。机器可读结论见 `build_impact_verdict.tsv`。

## 3. 三份归档的来路取证材料

官方资产来自 GitHub release `llvmorg-22.1.8` 的 `llvm-project-22.1.8.src.tar.xz`；API 返回的 SHA256 digest 与本机下载实测值均为 `922f1817…5888`。tag 的 peeled commit 实测为 `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`。完整 URL、API 原文、下载命令和退出码见 `commands/02_tarball_feature_comparison.log`。

| 特征 | 平台冻结 | 本机标准导出 | 上游官方 release |
|---|---|---|---|
| 顶层目录 | `llvm-22.1.8` | `llvm-22.1.8` | `llvm-project-22.1.8.src` |
| 成员数 | 184,826 | 184,826 | 184,826 |
| 解压后字节数 | 2,161,899,520 | 2,161,899,520 | 2,161,899,520 |
| 归档成员时间戳种类 | 1 | 1 | 1 |
| 时间戳（本机时区显示） | `2026-06-16 06:33:53` | 同左 | 同左 |
| 压缩 | gzip/deflate | gzip/deflate | xz/LZMA2，CRC64，86 blocks |
| 压缩字节数 | 258,010,724 | 258,009,434 | 167,061,596 |
| gzip 头 | `CM=8 FLG=0 MTIME=0 XFL=0 OS=3` | 同左 | 不适用 |

三份归档的成员数、解压后大小和单一成员时间戳一致；顶层目录、压缩封装和 `.git_archival.txt` 内容如表所示。平台与本机解包树的原有完整 diff 仍只报告 `clang/bindings/python/.git_archival.txt`；本次把 R9-B 原文复录到 `raw/r9b_exact_single_file_tree_difference.stdout.txt`。以上均作为推断材料保留，本报告不据此判定平台 tarball 的具体生成工具或流程。

更完整的机械对照见 `tarball_features/comparison.tsv`，原始压缩头、时间戳分布和 listing 样例均在 `tarball_features/`。

## 4. 命令账本、异常与边界终检

- `commands/00_freeze_and_source_search.log`：输入、SHA、源码搜索，`OVERALL_EXIT_CODE=0`。
- `commands/01_build_consumption_evidence.log`：首轮因 `find` 遍历退役根的不可读系统目录而退出 1；目标文件同时已被输出。该失败现场保留未覆盖。
- `commands/01_build_consumption_evidence.attempt2.log`：将遍历范围收窄到 `rpmbuild/BUILD` 后重取证，`OVERALL_EXIT_CODE=0`。
- `commands/02_tarball_feature_comparison.log`：官方资产取得与三归档特征，`OVERALL_EXIT_CODE=0`。
- `commands/03_finalize_segment1_checks.log`：消费零命中、未执行 export/build、无 R9C buildroot、源码身份终检，`OVERALL_EXIT_CODE=0`。
- `commands/04_validate_segment1_report.log`：首轮目录 diff 通过，但官方 digest 校验命令因 `jq` 引号转义错误退出 1；失败现场保留。
- `commands/04_validate_segment1_report.attempt2.log`：只修正校验命令的引号后复跑，目录 diff、GitHub API digest、本地 SHA、停止边界及源码身份全部通过，`OVERALL_EXIT_CODE=0`。
- `commands/05_final_manifest_and_boundary.log`：最终只读边界断言与稳定输出 SHA256 清单，`OVERALL_EXIT_CODE=0`。

终检原文：`codes/llvm` 仍在 `tizen_base`，HEAD 为 `3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b`，`git status --porcelain=v1` 零输出。项目仓既有未跟踪文件 `gbs_llvm.conf` 在任务前后均存在，本任务未触碰。

## 5. 停止点

第 1 段已完成。未修改 `SOURCE_PROVENANCE`，未执行标准导出、三架构重建、T4 门禁、旧制品隔离、提交或推送。等待人工确认后再进入第 2 段。
