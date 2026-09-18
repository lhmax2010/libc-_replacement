# 去掉静态 libgcc：本次 x86_64 失败机制已消除

## 实际改动与构建方式

候选位于 `tmp/STATIC_0917/bpftrace-dynamic-libgcc`，未覆盖上一轮二进制、
Source1002、spec、平台源码或任何包仓提交。

通过 Ninja `-t commands bpftrace` 取得上一轮实际构建树的链接规则。复用原有
Clang 22.1.8 编译的对象、LLVM/BCC/libclang/libc++/libc++abi 静态归档，在同一只读
构建根重链接。**本轮是一个链接目标的重新构建，不是再次从头编译全包。**
只有链接配置需要改变，复用对象使本实验更直接地隔离该变量。

语义参数只删一项：

```diff
- -static-libgcc -static-libstdc++
+ -static-libstdc++
```

没有额外添加 `-shared-libgcc`、`--no-as-needed`、export-dynamic、预初始化代码或
无用符号。其余修改仅是二进制、map、链接依赖记录的输出路径迁至新目录。
本机 Clang driver 在这种参数组合下实际选择共享 libgcc_s，不能据此推断所有
driver/版本均有同样默认值。完整原命令、候选命令、输入对象哈希分别在
`relink_plan_v2.json`、`reused_object_identities_v2.json`。

首次链接因 `--dependency-file` 仍指向只读旧构建树而失败；没有放宽旧树写权限，
只把该输出路径迁走，第二次链接 15.01 秒、退出 0。旧脚本/旧结果保留。
这不是取消判据失败后的“调判据”，而是独立的输出路径技术错误。

## 候选产物检查

`candidate_checks.json`、`candidate-map-excerpts.txt`、`candidate-string-counts.json`：

- DT_NEEDED 恰为 `librt.so.1`、`libdl.so.2`、`libm.so.6`、`libpthread.so.0`、
  `libgcc_s.so.1`、`libc.so.6`、`ld-linux-x86-64.so.2`。这里将 rt/dl/pthread/loader
  按实际名称列全，没有用“只剩 libc/libm”省略它们；它们是该构建环境的 glibc 家族。
- 没有 libc++.so、libc++abi.so、libstdc++.so、libLLVM.so、libclang.so、libbcc.so。
- map 仍拉入静态 libc++abi 的 cxa_personality.cpp.o，LLVM/BCC 等仍由原静态输入提供；
  未拉入 libgcc_eh.a 或 LLVM libunwind.a 的成员。
- `_Unwind_SetGR` 变为 `U _Unwind_SetGR@GCC_3.0`，不再是程序内静态定义。
  personality 和 patched `__forced_unwind` 的类型/析构符号仍在可执行文件内。
- `_ZNSt3__1` 字符串匹配 23,218 行，`_ZNSt7__cxx11` 为 0；同时用链接输入和 map
  支持标准库来源，不把单一字符串检索当成所有 ABI 的穷尽证明。
- `--version`、`--help` 均退出 0，与**上一轮 x86_64 libc++ 候选**的 stdout/stderr
  逐字相同。原 Source1002 不含 x86_64，不能声称完成了同架构原发布资产对比。

| 体积 | 旧 libc++ 候选 | 新候选 | 差异 |
| --- | ---: | ---: | ---: |
| strip 前（字节） | 160,787,128 | 160,749,120 | -38,008 |
| strip 后（字节） | 120,865,088 | 120,832,008 | -33,080 |

strip 采用同一构建根的 `eu-strip --remove-comment -f`，保留分离调试文件；
这是与旧对照一致的 strip 核心步骤，不冒充完整 RPM 后处理/写包。

新候选 SHA256：`a47882b4396bba41dd7ad05b55e793992c8c2bb377815f64cc646f0cd2576fca`。
strip 后：`a0862a0f7333e32b24fe72221371d2dee394a916905db2ad38d74999112f978d`。

## 取消验证

用相同的 R111 探针和同两份 libc++.a/libc++abi.a，只去掉 `-static-libgcc`，
保留静态标准库；四种等待各 5 次，**20/20 全部断言通过**：
进入 wait、取消穿过、RAII 清理恰好一次、cancel/join 成功、返回取消值。
`libcxx-matrix.json` 和逐次 JSON 保留完整 stdout/stderr/退出码。
该探针额外有自身需要的 libatomic 动态依赖，bpftrace 候选没有这项依赖。

同样的单变量变更应用到 GNU 静态标准库探针，也 **20/20 通过**，见 W1 附加列。
这支持“消除静态/共享展开器私有状态混用”这一归因，而非仅换掉 libc++ 就碰巧通过。

**在本次 x86_64/glibc 2.40/libgcc 14.2 配置和四种 deferred cancellation 形态内，
该失败机制已消除。** 这不是 bpftrace 内直接取消线程的实验，不是 aarch64/armv7l、
异步取消、其他异常形态的全面结论。上轮动态 libc++ 对照 20/20 沿用原证据，不重复
计为本轮新执行。

## GCC 与未定义工具链回退

本轮未修改 spec，只重链接实验候选，回退逻辑自然没有新代码。仍独立执行了：

1. 用实际 RPM 宏分别展开 GCC / 未定义 `_toolchain` 的 baseline 和夜间候选 spec；
   去空行后逐字相同，`-stdlib=libc++`、`-lc++abi` 均为 0。
2. 按展开的 install 行重新安装 Source1002 的 aarch64 资产到本轮临时目录。
3. 新安装件、原 tar 内文件、上一轮实际 GCC/undefined 构建后的安装件逐字节相同，
   SHA256 均为 `4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1`。

证据：`fallback_recheck.json` 和逐项 expansion/install JSON。
本轮没有再编译 GCC 全包；复用的两次实际 GNU 14.2 全 CMake 目标构建见
`../BUILD_NIGHT_0917/fallback_result.json`，那两次也不是完整 RPM 写包。
不能把本次复查夸大为“三条路径完整 RPM 均已重新构建”。

## 发布边界

仅消除了本次已复现的取消阻断；尚不能宣布候选已具备替换全部发布资产的条件。
前轮未闭合的完整 RPM/安装与真实 BPF 功能验证不由本轮 CLI 检查替代；
aarch64 尚未重链接新配置，armv7l 为 `NOT_OBSERVED_BOARD_OFFLINE`。
是否纳入打包层、扩展架构、发布替换，由人工裁决；本轮不推包仓。
