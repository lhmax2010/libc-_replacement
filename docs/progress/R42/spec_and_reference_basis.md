# R42 规范与参照依据

## 来源身份

- Arm 官方 `ARM-software/abi-aa`：`main` 固定到
  `ee4b3c12d57c8424ff60c2ae56e10690d0604ab6`；
- `ehabi32/ehabi32.rst` SHA256：
  `a500c2bf0a419f04f229a2d14889b5977a643a1d18710935230908ea7bfe79cf`；
- GCC 只读源码：`codes/gcc` HEAD
  `49c5060508ce8a6ed6beca375233688a6bdd9140`；
- libc++abi 实验输入：R40 strong 副本，来源与逐文件 SHA 见
  `commands/010_static_reference_extract.log`、
  `commands/014_implementation_diff_and_scope.log`。

## Arm EHABI 原文位置

- `ehabi32.rst:954-968`：handler 是传播终点并保留控制；cleanup 完成后
  返回 unwinder；两者均可通过 landing pad 进入，但语义不同；
- `ehabi32.rst:1425-1436`：`_URC_INSTALL_CONTEXT` 可用于 cleanup 或
  phase 1 找到的 handler；只有预期 cleanup 返回时才需保存
  `cleanup_cache`；
- `ehabi32.rst:1448-1461`：cleanup 完成后通过 `_Unwind_Resume`，C++
  可由 `__cxa_end_cleanup` 完成该动作；
- `ehabi32.rst:1858-1867`：C++ cleanup landing pad 最终恢复当前
  unwind；handler landing pad 最终重新进入应用代码；
- `ehabi32.rst:2012-2026`、`2043-2054`：catch handler 使用
  `__cxa_begin_catch` / `__cxa_end_catch`；
- `ehabi32.rst:2064-2069`、`2098-2110`：cleanup 使用
  `__cxa_begin_cleanup` / `__cxa_end_cleanup`，Arm personality 在进入
  cleanup landing pad 前调用 begin；
- `ehabi32.rst:2771-2784`：cleanup descriptor 的 phase 2 路径调用
  `__cxa_begin_cleanup`；catch descriptor 的 handler 路径设置 VRS 并
  返回 `_URC_INSTALL_CONTEXT`，没有该 cleanup 调用；
- `ehabi32.rst:2822-2824`：再次限定 begin_cleanup 用于 cleanup
  landing pad。

完整规范原件在 `reference/arm/ehabi32.rst`，带行号的相关段落原文在
`commands/006_arm_ehabi_relevant_text.log`。

官方 EHABI 文本没有检索到 `_Unwind_ForcedUnwind` 或
`_US_FORCE_UNWIND` 条目；forced-unwind 类型匹配属于 GCC/libstdc++
扩展实现层。本项没有以规范未写出的内容作推断补全。

## GCC/libstdc++ 参照

- `eh_arm.cc:39-73`：ARM `__cxa_type_match` 在 forced 标志存在时把
  throw type 映射为 `abi::__forced_unwind`；
- `eh_personality.cc:366-371`：显式区分 `found_cleanup` 与
  `found_handler`；
- `eh_personality.cc:540-567`、`643-665`：forced 类型匹配后保留
  `found_handler`；真 cleanup 单独记为 `found_cleanup`；
- `eh_personality.cc:734-737`：仅当 `found_type == found_cleanup` 时
  调用 `__cxa_begin_cleanup`，随后安装 context。

原文与行号在 `commands/010_static_reference_extract.log`。

## libc++abi 配对规则

`cxa_exception.cpp:325-419` 显示：`__cxa_begin_cleanup` 把异常压入
`propagatingExceptions`，`__cxa_end_cleanup_impl` 从同一栈取出并弹栈，
随后汇编 wrapper 跳转 `_Unwind_Resume`。完整原文同样在
`commands/010_static_reference_extract.log`。

## 第 1d 项回答

在“foreign + forced unwind + EHABI + 明确匹配
`__forced_unwind` handler”组合下，应按 catch handler 安装 landing
pad，不调用 `__cxa_begin_cleanup`；landing pad 由编译器生成的
`__cxa_begin_catch` / `__cxa_end_catch` 管理。依据强度为：

1. Arm EHABI 对 handler/cleanup 的规范性区分，以及只要求 cleanup
   descriptor 调用 begin_cleanup；
2. GCC/libstdc++ 对 forced 类型匹配的实现参照，明确保持
   `found_handler` 并避免 begin_cleanup。

由于 Arm EHABI 没有规定 forced-unwind 扩展本身，forced 类型识别的
依据来自 GCC 实现；handler 与 cleanup 的分流依据来自 Arm 官方规范。
