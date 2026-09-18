# 取消问题的既有证据与本轮待补部分

本文是证据索引，不是本轮板上结果；本轮 armv7l 数据待 W3 实测后补入。

## 复用输入

本轮 raw/200 实际读取以下已归档报告，未将其中实验重计为新执行：

| 材料 | 已有结果 | 不能由此证明的内容 |
| --- | --- | --- |
| `docs/progress/BUILD_STATIC_0917/W1_REPORT.md` | x86_64、Tizen Clang 22.1.8、glibc 2.40-1.10、libgcc/libstdc++ 14.2.0-1.10；GNU 静态标准库＋静态 libgcc 四种等待各 5 次 SIGABRT；动态组和静态 GNU＋共享 libgcc 各 20/20 通过 | 本轮不是 GCC driver 编译该 GNU 探针；不是所有架构或任意静态 libgcc 程序必崩 |
| `docs/progress/BUILD_STATIC_0917/W4_REPORT.md` | x86_64 libc++ 探针只去掉静态 libgcc 后 20/20 通过；实际候选同类重链接后保留共享 libgcc_s，无共享 libc++/LLVM/BCC | 不是当时已完成 ARM/aarch64、完整 RPM、真实 BPF 脚本或原发布资产功能等价验证 |
| `docs/progress/BUILD_STATIC_0917/W3_REPORT.md` | v0.24.2 静态 CI 用 Alpine 3.22/musl 和 `-static-libgcc`；官方 AppImage 内部另为包含共享 GNU 标准库、libgcc_s 等 18 项依赖的程序 | 不能把 musl CI 与 AppImage 当成同一种形态；没有实测证明任意 musl 场景都安全，也没有确认官方产品取消复现 |

## 机制的表述范围

已有 x86_64 回溯中，glibc 取消路径使用共享 `libgcc_s.so.1::_Unwind_ForcedUnwind`，
静态 personality 调用却落在可执行文件内的 `_Unwind_SetGR`；该份展开器的私有
`dwarf_reg_size_table` 未初始化，触发 abort。GNU/libc++ 两组均有这一实测链条。

这应写为“所测动态 glibc 取消路径混用了静态与共享展开器”，而不是“动态 glibc
程序必然有此故障”。上游 musl CI 不经过所测的 glibc 路径，也不等于已验证所有
musl 或所有上游发布资产。原 Source1002 已 strip、构建配方不明，近似链接形态
探针只能给条件性结论，不能改写为“原二进制必崩”。

## 本轮必须独立补充

- armv7l 物理板：同源探针、两种展开器形态各 20 次；静态对照的实际结果优先于预期。
- 若静态对照失败：尽可能收集既有 GDB 的回溯；没有回溯时不宣称机制与 x86_64 相同。
- 若静态对照通过：如实登记架构差异，不把 x86_64 的寄存器尺寸表机制外推到 ARM EHABI。
- 候选与原资产的真实功能、安装、RPM 载荷检查独立列出；探针不能替代它们。
