# 功能对照：PARTIAL

实际执行命令和全部逐行输出在 w1_commands.json；aarch64 原件/候选 stdout、stderr
逐行差异在 w1_comparisons.json。x86_64 为原生，aarch64 为 QEMU 用户态。

- 两架构候选 --version / --help 退出 0；aarch64 新旧输出相同，版本为 0.24.2。
- 新旧 --info、-l、三个 BEGIN 脚本（输出文字、算术、map 操作）都实际运行，
  均退出 1，提示只支持 root 用户。相同的权限错误不证明功能相同。
  内核 BPF 能力尚未进入检测，不能把“权限不足”改写为“内核不支持”。
- x86_64 原资产不存在，记 NOT_AVAILABLE；已查原 Source1002 解包路径及前轮清单。
- CLI 帮助和 main.cpp 的选项表没有列出内置函数清单的接口。仅附同版本源码
  docs/stdlib.md 的章节索引 builtin_documentation_index.txt，不将文档列表冒充
  旧二进制内置函数的实测清单。内置函数功能等价仍为 NOT_OBSERVED。
- 上轮已观察的可选依赖缺失（BFD/opcodes、DWARF、pcap、blazesym）是否造成
  新旧功能差异仍未证明；不改写为版本差异或断言一定缺失某个 probe。

## strip 对照

对副本执行平台 find-debuginfo.sh:248–280 所用的 eu-strip --remove-comment -f。
原候选未覆盖，strip 后 --version 均退出 0。完整 RPM 的 debug/source/压缩等
后处理尚待 W2，不能将此单步骤声称为已经执行整套正式 RPM 后处理。

| 架构 | 原候选字节 | strip 后字节 | 原发布资产字节 | 比例 |
| --- | ---: | ---: | ---: | --- |
| x86_64 | 160787128 | 120865088 | NOT_AVAILABLE | 无同架构基准 |
| aarch64 | 151746120 | 108009736 | 129681256 | 0.832886 |

strip 后 SHA256 见 w1_sizes.json；没有出现大于两倍的已可比体积差异。
原资产自身构建/strip 配方仍不明，不能把体积变化全部归因于标准库切换。

需要 root 与可用 BPF 内核环境下的新旧 --info、probe 和脚本输出才能闭合功能等价。
本轮未提权、未伪装身份、未连接离线板子；armv7l 为 NOT_OBSERVED_BOARD_OFFLINE。
