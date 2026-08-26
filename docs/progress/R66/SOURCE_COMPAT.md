# R66 371 包实现私有 API 与扩展扫描

## 分母与方法

分母为 `progress/T1/extraction_results.tsv` 中 371 个 `EXTRACTED_OK` 源码
RPM 身份。`code/scan_source_compat.py` 一次扫描完整语料，输出 53,452 条
命中（加表头 53,453 行），wrapper 退出码 0，完整原始表 SHA256 为
`fdef19b2d9dcb378d758f40633334bebd0fe88e1ed0f4ab881ac0f6a94d6129a`。
命令见 `commands/024_corpus_full_scan_scoped.log`。

规则覆盖 libstdc++ 内部头、GNU 扩展命名空间、`std::__cxx11` 等 ABI
命名空间、`_GLIBCXX_*`、C++ ABI runtime API、保留前缀 `std` 成员和
显式 `-lstdc++`。结果分为 `PRODUCTION_LIKE`、`TEST`、
`BUNDLED_OR_VENDOR`、`BUNDLED_TOOLCHAIN`；scope 是路径启发式，不是
构建可达性证明。

## 机械候选结果

生产形态逐家族结果：

| 家族 | 包身份数 | 位置数 | 既有 45 外包身份数 |
|---|---:|---:|---:|
| C++ ABI runtime API | 32 | 302 | 14 |
| 显式 libstdc++ 链接 | 30 | 81 | 13 |
| GNU 扩展命名空间 | 19 | 113 | 0 |
| libstdc++ ABI 命名空间 | 3 | 26 | 0 |
| `_GLIBCXX_*` 配置宏 | 45 | 705 | 24 |
| 内部头路径 | 41 | 220 | 18 |
| `std` 保留成员 | 2 | 13 | 0 |

各家族去重后的并集为 89/371 个包身份，其中 39 个属于既有 45、50 个在
其外。**89/50 是高召回候选数，不是缺陷数。** 完整逐位置与 token 表见
拆分后的 `tables/source_compat_hits_*`、`source_compat_tokens.tsv`；聚合
见 `source_compat_*summary.tsv`。

## 所有权与等价物复核

`code/classify_source_candidates.py` 对 1,460 条生产形态行继续做机械
所有权复核：

- 内部头只有在 `codes/gcc/libstdc++-v3/include/<token>` 确实存在且
  libc++ 同路径不存在时才算 libstdc++ 依赖；Linux `include/debug/*`、
  glibc `bits/*`、PHP `ext/*` 均降为前缀碰撞；
- `cxxabi.h` 在本平台由 libc++abi 提供；`__cxa_demangle` 等已声明 API
  不是 libc++ 缺项；
- `_GLIBCXX_*` 位于 `#if/defined` 时按实现选择/feature probe 记录，
  不据此定为缺陷；
- GNU namespace、ABI namespace、保留 `std` 成员和硬编码
  `-lstdc++` 保留为实现相关引用。

复核发现已知 45 外有 **13 个源码 RPM 身份**包含可确认的硬编码
`-lstdc++` 构建引用，完整位置见
`tables/source_compat_outside_known45_curated.tsv`。它们是“源码树中存在
引用”的确定证据；本轮没有执行各包生产构建，引用是否落入当前目标以及
是否已被上层 flags 覆盖均为 `NOT_OBSERVED`，因此不把 13 写成 13 个构建
失败。

另三项候选经上下文排除：

- 两个 db4 版本的 `debug/debug.h` 只出现在解释 SunOS 缺头的 configure
  注释中，不是 include 或探测；
- libtbb 的 `abi::__cxa_get_globals()` 仅位于
  `__TBB_STD_RETHROW_EXCEPTION_POSSIBLY_BROKEN`，该宏明确要求
  `__GLIBCXX__`，libc++ 配置下分支不编译。

其余既有 45 外候选主要是兼容性分派、编译器 feature probe、由 libc++abi
继续提供的 `cxxabi.h`、本地同名头或前缀碰撞。逐行 disposition 保存在
`source_compat_classified_hits.tsv`，没有删除这些候选。

## 与既有 45 的对照

生产形态并集命中既有 45 中 39 个。未进该 scope 的六个身份是：

- 两个 GCC 身份：命中被正确分到 `BUNDLED_TOOLCHAIN`；
- `connectedhomeip`：source RPM payload 的 397 MB 内层 tar 在现有冻结树
  中未展开成 `unpacked/`，本轮不能逐文件扫描，记 `NOT_AVAILABLE`；
- 三个 jsoncpp 身份：T1 命中来自本轮未纳入 source suffix 的补丁/生成
  输入，本轮生产源码树未命中。

因此本轮方法不是既有 45 的替代分类器。T1 的范围还包括补丁、压缩 payload
和更宽 build-system 模式；本轮新增 13 项恰来自显式链接配置，说明此前
“需改源码”口径与“任何实现相关构建引用”口径并不相同。

## 暴露阶段与限制

- 内部成员/头/API 通常在编译期暴露；显式 runtime 链接在链接或加载期；
- 条件宏与兼容分支只有配置选中后才有影响；
- bundled/vendor 命中可能随其上游构建进入，也可能完全不参与平台产物；
- scope 对 `_test.cc` 等无目录标记测试会有误分，故完整表是候选清单而非
  可达性清单；
- 没有构建 13 个新增引用包，实际失败结果 `NOT_OBSERVED`；
- `connectedhomeip` 的内层源码不可得是本次 371 分母中的实质扫描盲点。

## 无效尝试记录

`commands/019` 至 `023` 保留了宽 Python 遍历、遗漏 `--no-ignore`、被终止
或结果为零的尝试；它们不作为结论证据。`commands/038` 的 awk 引号转义
错误以退出码 2 结束，也不作为所有权结论；相同目的由有完整源码的
`classify_source_candidates.py` 在 `commands/039` 正常完成。最终扫描只以
`commands/024` 的退出码 0、终态 SHA256 和 `commands/041` 的 53,452 行
拆分一致性核验为准。
