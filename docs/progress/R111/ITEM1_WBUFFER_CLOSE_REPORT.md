# `wbuffer_convert::__close()` 调用面与影响核查

## 结论

本项为静态核查。当前 libc++ 主头文件和 C++03 镜像中，`__close()` 都只有一个
实际调用点，即 `wbuffer_convert` 析构；该调用以独立表达式 `__close();` 出现，
返回值被丢弃。没有其他成员或源码路径调用 `__close()`，也没有任何调用方读取其
返回值。

若只取消析构对 `__close()` 的调用，或只取消 `__close()` 对 `sync()` 的调用，
源码中负责释放该对象所拥有 codecvt 和缓冲区的语句仍在析构内，不依赖
`__close()` 的结果。变化涉及输出收尾，不涉及当前可见的资源释放语句、数据成员
布局或函数签名。本文只陈述影响面，不选择是否改变。

`wstring_convert` 没有同样的析构同步路径：其析构只执行
`delete __cvtptr_`；转换方法会在 `to_bytes` 内调用 codecvt `unshift`，但析构不
调用 `sync()`、`__close()` 或 stream buffer。因此它不属于此前识别的五处
non-throwing 站点。

## 源码身份

- 源码仓 revision：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`。
- 主头文件：`codes/llvm/libcxx/include/__locale_dir/wbuffer_convert.h`，SHA256
  `73be26e86222438221cb69cddb52a0efe80a897651b5beeeaf2f8c87209e8d1d`。
- `wstring_convert` 主头文件：
  `codes/llvm/libcxx/include/__locale_dir/wstring_convert.h`，SHA256
  `e69bb0754715e5fc318273fec9323e4580c91c3a93929e9a1d8e5decb18f0d6f`。
- C++03 镜像不是独立的 `wbuffer_convert.h`，而是合并在
  `codes/llvm/libcxx/include/__cxx03/locale`；首次按独立路径读取失败后，已用文件
  清单定位到该实际路径。
- 三个被核查文件的 tracked diff 为 0；未修改源码。

## `__close()` 的全部出现与调用点

同一条索引化 `git grep` 对整个 `libcxx/include` 和 `libcxx/src` 搜索
`__close(`，只得到下列六处。主实现和 C++03 镜像各有一组：

| 变体 | 文件、行号 | 分类 | 上下文 |
|---|---|---|---|
| 主实现 | `__locale_dir/wbuffer_convert.h:96` | 声明 | private，返回 `wbuffer_convert*`，带 `_LIBCPP_HIDE_FROM_ABI_VIRTUAL` |
| 主实现 | `__locale_dir/wbuffer_convert.h:120` | 唯一调用 | 析构首句为 `__close();` |
| 主实现 | `__locale_dir/wbuffer_convert.h:410-418` | 定义 | 输出模式调用 `sync()`，以 `this`/`nullptr` 表示成功/失败 |
| C++03 | `__cxx03/locale:3380` | 声明 | 与主实现相同的 private 声明 |
| C++03 | `__cxx03/locale:3404` | 唯一调用 | 析构首句为 `__close();` |
| C++03 | `__cxx03/locale:3694-3702` | 定义 | 与主实现同一控制流 |

结构化结果见 `tables/item1_close_occurrences.tsv`。分析脚本断言每个变体恰有一个
声明、一个定义、一个“返回值被丢弃”的调用，其他引用为 0。析构调用本身同时是
正向对照，证明规则确实命中了目标；不是空规则导致的“无其他调用”。

## 返回值是否被使用

`__close()` 初始化返回值为 `nullptr`；只有 codecvt 和 wrapped stream buffer 都
非空时先设为 `this`，输出模式的 `sync()` 返回非零又改回 `nullptr`。唯一调用点
是析构中的独立表达式，没有赋值、条件判断或 return 转发。因此：

- `sync()` 以返回码报告的失败被 `__close()` 转成 `nullptr`；
- 析构不读取该指针；
- libc++ 源码树中没有第二个调用方能读取它；
- 普通返回码失败后，析构仍继续执行后续 delete；抛出或强制展开不是普通返回码，
  其控制流另论。

## 其他成员与 `sync()`

当前类内直接出现的 `sync()` 路径为：

- `seekoff` 在实际 seek 前调用 `sync()`，失败则返回无效位置；
- `seekpos` 在实际 seek 前调用 `sync()`，失败则返回无效位置；
- `__close()` 仅在已经进入输出模式时调用 `sync()`；
- `sync()` 自身在输出模式刷新 put area、执行 codecvt `unshift`、写入 wrapped
  stream buffer 并调用其 `pubsync()`；在输入模式回退未消费字节并清理 get area。

这些路径都不调用 `__close()`。源码中也没有成员读取“析构将来是否会同步”的
状态或由析构回写的结果。静态核查只能证明成员实现之间没有直接控制流依赖；外部
调用方是否依赖作用域结束时自动写出缓冲数据，属于使用语义，本项没有新的真实
调用方样本，记 `NOT_OBSERVED`。

## 假设取消析构同步时的影响面

### 资源释放

析构的顺序是：调用 `__close()`，随后 `delete __cv_`，再按 `__owns_eb_` 和
`__owns_ib_` 释放两个动态缓冲区。`__close()` 不 delete 任何成员，也不改变两个
ownership flag。wrapped stream buffer 指针 `__bufptr_` 在当前析构中本来就不被
删除。

因此，在“只删除析构调用”或“只让 `__close()` 不调用 `sync()`”这两个限定假设
下，没有一条当前可见的 owned-resource 释放语句会被删掉或失去条件。可能未写出
的转换尾部和缓冲字节属于输出收尾差异，不是该对象的 heap resource 释放路径。

当前实现还有一个不同事实：若 `sync()` 内的开放虚调用抛出或强制展开在
non-throwing 析构处导致终止，位于其后的三个 delete 路径不会在该对象中执行；
这不是 `sync()` 返回 `-1` 的情形。

### 其他代码路径

- `seekoff`、`seekpos` 和外部经 `pubsync()` 进入的显式同步路径不依赖
  `__close()`，限定改动不会删除这些调用。
- `overflow` 的 put-area 写出逻辑仍存在；限定改动只取消析构时最后一次自动触发。
- 主头与 C++03 镜像中的旧消费者已经内联现有析构体；只更新共享库不能改变它们。
- 本项没有平台使用者样本，因而实际受影响调用点仍为 `NOT_OBSERVED`；既有冻结
  覆盖范围内为零命中，外部产品/应用材料为 `NOT_AVAILABLE`。

### ABI 标注

析构声明带 `_LIBCPP_HIDE_FROM_ABI`，`__close()` 带
`_LIBCPP_HIDE_FROM_ABI_VIRTUAL`；宏定义落实为 hidden / exclude-from-explicit-
instantiation 语义。两者都是头文件模板实现。

限定为只改函数体控制流时，数据成员、函数签名、虚函数集合和上述标注本身均不
发生源码变化；新旧消费者之间仍会有内联行为差异。若连 private `__close()` 函数
本身一起删除，那是超出前述两个限定假设的另一种源码改动，本项没有实施或验证。

## `wstring_convert` 对照

主实现 `wstring_convert.h:124-126` 和 C++03 镜像 `locale:3199-3201` 的析构均只
执行 `delete __cvtptr_`。类中没有 `__close` 或 `sync`。`to_bytes` 在显式转换调用
内部用 `unshift` 形成输出字符串，并通过返回值或 `range_error` 报告转换结果；这
不是析构时对外部 stream buffer 的自动同步。

所以该类没有 `wbuffer_convert` 的同类析构同步问题，也没有作为该问题的一处被
计入五个站点。这里没有把 codecvt 自身析构可能具有的通用 C++ 析构规则扩写成一
个已观测取消站点。

libstdc++ 对照文件 `bits/locale_conv.h:442` 的 `wbuffer_convert` 析构为
`= default`，文件 SHA256 为
`d204ceedcf78f7c1289321f86bfd1f8765a94d6a869b467e0c0b0aefd038c562`。

## 证据和限制

- 全树索引搜索：`raw/008_git_grep_all_close_libcxx.*`。
- 主实现、C++03 镜像和 `wstring_convert` 上下文：`raw/009_*`、`raw/010_*`。
- ABI 标注、全部直接 sync 路径和资源字段：`raw/014_*`。
- libstdc++ 对照：`raw/011_*`。
- 自动分类脚本：`code/analyze_wbuffer_family.py`，SHA256
  `c2a8e61f49e472c0f6eb940efe9d87dbd1b07d09521eed562cdc0995f2c1253c`。

初次对 `codes/` 全根检索在 60 秒超时，退出 124；随后改用既有证据中的精确仓库
和 `git grep` 索引完成核查。一次 `git status` 因源码仓 I/O 较慢被精确终止；以
三个目标文件的 `git diff --quiet` 完成只读完整性检查。失败尝试均保留，未据超时
作零命中结论。

本项没有修改平台源码或补丁，没有实施任何假设改动，也不作处置建议。
