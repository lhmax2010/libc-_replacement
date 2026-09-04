# 检索方法与边界

## 原始方法

原始输入为 R32 复用的 T1 已解包语料：372 个候选源码 RPM 中 371 个
`EXTRACTED_OK`，本轮扫描分母为 371。输入身份由以下冻结 SHA256 约束：

| 输入 | SHA256 |
|---|---|
| `progress/T1/extraction_results.tsv` | `5498c430baecd81e689a44d8266d5597a92779680105ede0c320f9b1ea013031` |
| `progress/T1/corpus_hits.tsv` | `cd2a91832f5a548b4e762c9cc42c4349c50d4f82e559058760f1cf6f83a6c7c2` |
| `progress/T1/scan_status.tsv` | `5c15fe0097f5dc6b37a755ce25cb6d01846050fa653a4ca5fe8e74803ed85723` |

R32 使用每包一个 `rg --threads 1`、4 个包级 worker，匹配
`abi::__forced_unwind`/`__cxxabiv1::__forced_unwind` token 以及其他风险标记。
源码、测试、示例和构建文件均保留，文档不参加包分类。原始代码位于
`docs/progress/R32/tools/scan_impact.py`，原表为
`docs/progress/R32/tables/forced_unwind_guard_locations.tsv`。

原 `guard_status()` 从 token 行向前 3 行、向后最多 15 行取窗口，在下一处
catch 前查找裸 `throw;`。它没有先证明“当前 token 行本身就是 catch 子句”，
也不认识 `__throw_exception_again`，因此造成四个误计和大量漏计。

## 本轮严格复核

严格表达式允许空白和换行，覆盖：

- `catch (abi::__forced_unwind&)`；
- `catch (const __cxxabiv1::__forced_unwind&)`；
- `catch (abi::__forced_unwind const&)`；
- libstdc++ 的 `__catch(__cxxabiv1::__forced_unwind&)`。

随后解析 handler 的平衡花括号，分别识别裸 `throw;` 和
`__throw_exception_again;`。原始阳性相关源码 RPM 的所有文本文件均参与，
不再按扩展名过滤，所以能覆盖无扩展名标准库头文件。每个输出位置又回到实际
源码取行，并用 Universal Ctags 给出最近函数/模板/lambda 上下文；不能安全识别
的上下文应标 `NOT_OBSERVED`，本次最终表没有这种行。

当前复核使用 `tables/current_source_identity.tsv` 中八个仓库的明确 HEAD；普通
Git 文本通过 `git grep -I` 找 token 后逐 blob 解析，LLVM 的三个 MLGO 模型包和
TensorFlow AOT 包则逐 tar member 检查。没有用当前工作区的未提交内容替代指定
commit。

## 已知盲区

- 表达式要求能在预处理前源码中看见 `catch`/`__catch`、目标类型名和引用符号。
  若类型通过别名隐藏、catch 由多层 token-pasting 宏生成，可能漏掉。
- 当前没有运行预处理器，因此条件编译失活分支也会被计入；表中是源码出现次数，
  不是某个架构最终编译实例数。
- 同一头文件复制进多个架构资产时按不同包内路径分别计数；源码 RPM Release
  变体则另给去重口径。改变这两条会改变总数。
- 本轮未把当前平台所有源码包重新冻结；当前 140 只覆盖历史阳性八包，无法发现
  其他包后来新增的站点。
- handler 中通过项目自定义函数间接重抛、而非 `throw;` 或
  `__throw_exception_again` 的写法，语法子句仍会列入 175 表，但传播行为会记为
  `NO_RETHROW_OBSERVED`；要证明间接传播需另做调用图分析。
