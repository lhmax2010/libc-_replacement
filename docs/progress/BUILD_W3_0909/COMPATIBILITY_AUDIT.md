# 11 包兼容性交叉核对

## 结论

八个已推送包中，libc++ 的依赖声明、编译选项与链接选项均受统一判据
保护：仅当 `_toolchain` 已定义且 `toolchain_is clang` 时启用；
`tensorflow2` 还要求其既有 `USE_CLANG=ON`。结构扫描共检查八份远端提交
中的 spec，`build_with_libcxx` 定义保护与每一处 libc++ payload 保护均
为 PASS。结合既有实构建证据，GCC 路径没有引入 libc++ 相关选项。

LLVM 的本地未提交候选使用相同保护；已完成的 armv7l GCC 构建中禁止选项
计数为 0，但另外两架构兼容格未完成，因此只能记 PARTIAL。`bcc-tools`、
`bpftrace` 尚无适配提交，记 NOT_AVAILABLE，不把“还没改”写成通过。

## “所有改动都在条件内”的准确解释

若“改动”专指 libc++ 的启用行为，八个已推送包全部满足。若把提交中的
每个文本行都算入，则不能说所有文本改动都位于条件块内：

- `boost` 在 `%install` 阶段无条件读取和传递已保存的 flag 变量，但只有
  条件块会向变量追加 libc++ 选项；GCC 三架构完整构建通过；
- `tensorflow2` 还包含一处不在 spec 条件内的标准兼容源码修正，把歧义的
  `std::abs<T>` 写法改为显式函数指针类型。aarch64/x86_64 GCC 完整构建
  通过，armv7l 与未改基线在到达该文件前同点同因失败；
- `jsoncpp`、`pcre` 各有一个文件末尾空行的文本变化，无行为含义。

因此本报告不使用“提交的所有行都被条件包住”这一过宽表述；可核实的事实
是“所有 libc++ 激活与选项均被条件保护，GCC 路径未引入这些选项”。

逐包结果见 `COMPATIBILITY_AUDIT.tsv`，扫描器与原始输出见 `code/` 和
`raw/004_spec_guard_audit.md`。
