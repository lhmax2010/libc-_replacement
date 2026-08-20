# R50 判定方法与口径

## 沿用的 R47 方法

输入为 R47 已确认的 34 个非 `ios.cpp` 汇聚站点。对每个站点定位其
所在函数，并以两个互相独立的维度判定：

1. 库侧：`nm -D --defined-only --with-symbol-versions libc++.so.1.0`，
   记录动态导出、绑定类型与版本节点；不存在则记 `NOT_EXPORTED`。
2. 消费者侧：同一探针 TU 分别以 `-O0`、`-O2` 编译为目标文件，
   用 `nm -C` / `readelf -Ws` 判定站点函数是 `UND`、消费者本地定义，
   还是因完全内联而不再形成独立符号。
3. `S16/S17` 是隐藏模板 helper，不以 helper 自身是否导出作最终结论；
   其显式实例化载体成员在库侧导出且消费者为 `UND`，故记
   `LIBRARY_VIA_CARRIER`。
4. `S23` 仅存在于 C++20，单独使用 C++20 探针。符号缺失但调用仍存在
   时，结合优化档与源代码确认是否为完全内联，不把它误记为未覆盖。
5. 去重单位为站点 ID；构造函数/析构函数的 ABI 别名不重复计站点。

R47 原输入、分类表与探针 SHA256 见
`commands/007_r47_method_and_inputs.log`。R50 使用的探针源码逐字复用
R47，SHA256 仍为 `47699a59…` 与 `19394f8a…`。

## armv7l 必要调整

探针仍使用 R47 的 `-fPIC`、异常、pthread、`nostdinc++` 与 O0/O2
口径，但增加 C2 buildroot 实际记录的 armv7l 生产选项：

- `-march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp`
- `-mthumb -mlittle-endian -Wp,-D__SOFTFP__`
- `-fvisibility=hidden -fvisibility-inlines-hidden`
- `--rtlib=libgcc`

完整逐选项命令见 `commands/011_build_arm_probes.log`。选项来源为
`LIBCXX-2218-armv7l-20260806-c2` 的 `CMakeCache.txt` 与
`build.ninja`；未改站点、探针调用或去重规则。

## 批次换算

34 个非汇聚站点中，`LIBRARY` 与 `LIBRARY_VIA_CARRIER` 都不要求
消费者重编；`HEADER` 要求消费者重编。再把 R47/R44 已核实的 16 个
`ios.cpp` 汇聚站点加入库内批：

`库内批 = 16 + 非汇聚库侧站点数`，`头文件批 = 非汇聚头侧站点数`。
