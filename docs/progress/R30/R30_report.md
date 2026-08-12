# R30：首批两处源码缺陷的补丁开发与验证

## 结论

`RED_STOP_SEMANTIC_VALIDATION`。

两份最小候选补丁均以 `__GLIBCXX__` 隔离 GNU 专有
`__cxxabiv1::__forced_unwind` handler，三架构编译日志中原诊断均已
消失；但该源码改写没有通过 pthread_cancel 语义门禁：

- libstdc++：x86_64 原生及 armv7l `qemu-arm-static -L` 均正常取消，
  析构一次、catch-all 零命中、退出 0；
- libc++：armv7l 与 x86_64 均终止。初始候选报
  `FATAL: exception not rethrown`（退出 134）；进一步诊断变体识别
  foreign exception 后执行 `throw;`，仍报
  `libc++abi: terminating due to uncaught foreign exception`（退出 134）。

因此任务背景中“libc++abi + libgcc_s 下删除该 catch 后强制展开只做
cleanup、不匹配 catch-all”的机制陈述已被本项目的两架构运行实测
证伪。候选 patch 仅作为失败证据归档，`INDEX.tsv` 保持“待开发”，
不得送 Gerrit。

完整 libc++ 包构建还在原错误之后发现多项独立源码/ABI 红项，故
“这两处是首批唯一阻塞”也未被实测支持。本任务没有越界修改它们。

## 1. 身份与改动前取证

| 包 | 版本 | 基线提交 | 目标 | 安装性质 |
|---|---:|---|---|---|
| libcynara-commons | 0.26.0-1 | `4b60376089951e8e379cc7a23f7509f433a67525` | `src/common/exceptions/TryCatch.h:65` | `cynara-devel` 对外安装头 |
| security-manager | 2.4.0-0 | `f6a98bfdbf86f48a66feb81e81e4c8b488c9a550` | `src/common/include/utils.h:70` | 内部头，spec 与 install 规则未安装 |

两处前后 30 行、分支/HEAD/remote/status、spec `%files` 与 CMake install
原文分别见 `commands/001_identity.log`、`002_source_context_and_specs.log`、
`003_install_and_include_chain.log`。原始编译错误见
`inputs/libcynara_error_context.raw.log` 与
`inputs/security_manager_error_context.raw.log`。

两处形态并不完全相同：libcynara 使用 `const` 引用，位于普通异常
之后且包含日志和解释注释；security-manager 使用非 const 引用，位于
普通异常之前且仅重抛。补丁分别保持了原顺序、const 性、日志与注释。

既有 44 条传播链以原记录 SHA256
`b3cba0f30a85047fcfc3062499cafab567574026e74aca772720e2c8e1cc0762`
为输入复核，`44/44 MATCH`；其中 libcynara 14 条（本包 13、
libcynara-dbus 1），security-manager 30 条。逐条结果见
`tables/forced_unwind_propagation_current.tsv`，命令与退出码见
`commands/008_propagation_44_verify_normalized.log`。

## 2. 补丁设计与改动

最终最小候选仅在原 GNU handler 外增加：

```cpp
#if defined(__GLIBCXX__)
    } catch (... abi::__forced_unwind ...) {
        throw;
#endif
```

采用正向 `__GLIBCXX__`，因为它证明 GNU 专有类型可用；不采用
`!defined(_LIBCPP_VERSION)`，因为“不是 libc++”不能证明是 GNU
libstdc++。完整设计、初始预期、语义否决及诊断变体见
`design/patch_design.md`。最终候选提交仅存在本地源码树，未推送：

- libcynara-commons：`5ba41186b38682290575ceff60143c9009e531f7`
- security-manager：`dffe51106564d8db0a40b6ff7c01d6fcece816af`

最终 diff 与作用域证据见 `commands/048_reject_revised_candidate_restore_minimal.log`。
只修改了任务指定的两个目标文件。

## 3. 构建验证

逐架构结果以 `tables/build_results.tsv` 为准，全部构建命令、工作目录、
完整输出及退出码在对应 `commands/*.full.log` 中。每次使用全新 `-B`
根与 `-C`，结束后标 `RETIRED_SINGLE_USE`。

已确认的结果：

- 两包 armv7l 默认 libstdc++ 基线完整构建退出 0；
- libcynara 候选三架构均不再出现 `__forced_unwind` 错误，但完整包
  因 19 处 nodiscard/Werror 及 `std::ios_base::iostate::_S_badbit`
  红停；
- security-manager 候选 libc++ 构建越过原目标头，随后命中
  `std::ofstream/std::ifstream::__filebuf_type`、`std::system_error`
  构造差异及 Boost.Test 的 `std::__1`/旧库 ABI 链接未定义符号。

这些红项均不是本任务授权的两个目标文件，未修改、未屏蔽、未放宽
`-Werror`。

## 4. libstdc++ 回归比对

armv7l 默认环境的基线与候选均以同一 buildconf、同一仓集合和独立根
构建，四次完整构建均退出 0。RPM 与解包 payload 的逐文件 SHA256
对照见两个 `tables/*_libstdcxx_regression_payload.tsv`：libcynara 为
680 MATCH / 26 DIFF，security-manager 为 274 MATCH / 14 DIFF。

对非 debug RPM 中全部 ELF 的 `.text`、`.rodata`、`.data` 再做逐节
SHA256：libcynara `57/57 MATCH`，security-manager `42/42 MATCH`，零
运行节差异。剩余差异包括公开 TryCatch.h 的预期文本变化、debug/
行号/build-id 元数据，以及 security 测试包构建时生成的 DB journal；
详见 `tables/libstdcxx_regression_summary.tsv` 与
`commands/057_*`、`058_*`、`059_*`。

## 5. pthread_cancel 语义验证

用例源码：`semantic/pthread_cancel_probe.cpp`。工作线程在带析构对象
的 try/catch 区域内执行 `pthread_testcancel()`；主线程 cancel/join，
同时计数精确 catch、catch-all、析构和正常返回。

执行方式与六项结果见 `tables/semantic_results.tsv`。关键原文：

- GNU armv7l：`commands/035_run_semantic_default_armv7l_qemu.log`；
- 初始 libc++ armv7l：`commands/038_run_semantic_libcxx_armv7l_qemu.log`；
- 初始 libc++ x86_64：`commands/041_run_semantic_libcxx_x86_64_native.log`；
- 诊断变体：`commands/046_*`、`047_*`。

x86_64 使用目标根动态加载器与 `--library-path` 原生执行；armv7l 使用
`qemu-arm-static -L <fresh-buildroot>`。两种载体得到一致的 libc++
失败方向，故不能归类为仅 QEMU 不可得。

## 6. 补丁归档与状态

归档文件：

- `docs/patches/libcynara-commons/patch/0001-Guard-libstdc-forced-unwind-handler.patch`
- `docs/patches/security-manager/patch/0001-Guard-libstdc-forced-unwind-handler.patch`

两份 README 已补充实测结果；建议 Gerrit message 位于
`patches/*_gerrit_message.txt`，首行明确 `[WIP, DO NOT SUBMIT]`。
没有向 Gerrit 或任何外部源码仓 push。

## 7. 红项清单

| 红项 | 状态 | 含义 |
|---|---|---|
| `RED_SEMANTIC_VALIDATION` | OPEN | libc++abi + libgcc_s 下 pthread_cancel 穿越 catch-all 会终止 |
| `LIBCYNARA_ADDITIONAL_LIBCXX_DEFECTS` | OPEN | `_S_badbit` 与 nodiscard/Werror，不在授权目标文件 |
| `SECURITY_MANAGER_ADDITIONAL_LIBCXX_DEFECTS` | OPEN | fstream 私有类型、system_error、Boost.Test ABI，不在授权目标文件 |

本报告只陈述实测状态，不给出处置建议。
