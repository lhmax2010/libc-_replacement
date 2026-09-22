# bpftrace static：libbpf 静态链接核验

两架构候选的 **libbpf 链接缺口已闭合**：最终 ELF 均不再依赖 `libbpf.so.1`，只比原资产多预期的 `libgcc_s.so.1`。x86_64 同配置取消探针 20/20 通过。本轮仅修改 tmp 中两份 CMake 的同一行，重链接 static，未重编主包、未改原 spec、未推包仓、未覆盖 Source1002、未上板。

## 结果

| 架构 | ELF 验收 | 字节数（strip -g） | `_ZNSt3__1` / `_ZNSt7__cxx11` 行数 | libbpf 归档与候选的已定义符号交集 |
|---|---|---:|---:|---:|
| armv7l | PASS | 90,870,376 | 22,952 / 0 | 770 |
| aarch64 | PASS | 136,139,424 | 16,480 / 0 | 764 |

armv7l 的 DT_NEEDED 恰为 `libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux.so.3`；aarch64 恰为 `libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux-aarch64.so.1`。无动态 libc++、libLLVM、libbpf；较宽的 `St7__cxx11` / `std::__cxx11` 检索也为 0。这是本轮产物证据，不推及未测配置。

候选 SHA256：

- armv7l: `a68f5c8c44051769e1e4718935fc3c5e9e147c5ea56e1a91b86ab902414346d5`
- aarch64: `26ecab8874f842db053a227a92b93e000fcf818ded8cd957daa0e716f36e0079`

完整路径、大小与依赖：[CANDIDATES.tsv](CANDIDATES.tsv)；原始 readelf/nm/strings 命令及退出码：[ARM](verify-armv7l/commands.json)、[aarch64](verify-aarch64/commands.json)；逐项验收：[ARM](verify-armv7l/result.json)、[aarch64](verify-aarch64/result.json)。

## 为什么以前还依赖 libbpf.so.1

不是缺少静态归档。两架构输入根中均有 `.a`，实际 RPM 归属及 SHA256 见 `cells/input-*/build.log`：

- armv7l: `libbpf-static-1.6.2-1.1.armv7l`，`/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/input-armv7l-origin/usr/lib/libbpf.a`，SHA256 `6fd57ddccbb5e09e72957330da26614bf120f190003b913a91138b117ba20a84`。
- aarch64: `libbpf-static-1.6.2-1.1.aarch64`，`/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/input-aarch64-v3/usr/lib64/libbpf.a`，SHA256 `1c03e0b4774a466d2cfd7119ddda177dc135b78727fb35d25b029358bddf091d`。

缓存已经将 `LIBBPF_LIBRARIES` 指向对应 `/usr/lib[/64]/libbpf.a`。但本地 `src/ast/CMakeLists.txt:86` 的 `STATIC_LINKING` → `LIBCLANG_STATIC_PATH` 扩展又写了裸库名：

```cmake
target_link_libraries(ast PUBLIC ${LIBCLANG_STATIC_ABS_PATH} bpf clangIndex clangDriver clangFrontend clangCodeGen)
```

这让实际 driver 行同时含绝对路径 `libbpf.a` 与 `-lbpf`。原 ARM ELF 的动态未定义项包括 `bpf_linker__add_file/finalize/free/new@LIBBPF_0.4.0`（raw/076 输出），不能说原来所有 libbpf 代码都动态链接；可确认的是仍有动态边界。修正为：

```cmake
target_link_libraries(ast PUBLIC ${LIBCLANG_STATIC_ABS_PATH} ${LIBBPF_LIBRARIES} clangIndex clangDriver clangFrontend clangCodeGen)
```

这复用已经解析为 `.a` 的 CMake 变量，不新建开关。两个副本 diff：[ARM](armv7l-static-libbpf.patch)、[aarch64](aarch64-static-libbpf.patch)。没有新增 libbpf 构建或改其 spec。

## 上游 0.24.2 的实际做法

[根 CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/CMakeLists.txt#L81) 在 STATIC_LINKING 时把 `CMAKE_FIND_LIBRARY_SUFFIXES` 设为 `.a`；[FindLibBpf.cmake](https://github.com/bpftrace/bpftrace/blob/v0.24.2/cmake/FindLibBpf.cmake#L23) 通过 `find_library` 设置 `LIBBPF_LIBRARIES`；[src/CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/src/CMakeLists.txt#L98) 使用该变量。上游 [src/ast/CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/src/ast/CMakeLists.txt#L81) 没有本地 `LIBCLANG_STATIC_PATH` 加裸 `bpf` 的扩展。下载原文与哈希在 `upstream/`、raw/033，差异在 raw/057；未猜测该扩展的作者或引入时间。

## 实际链接命令与构建边界

改前在独立副本中实际执行 `cmake -E cmake_link_script ... --verbose=1`，改后实际执行 `cmake --build .../build-static --target bpftrace --verbose -- -j1`，两架构前后均退出 0。不是只读 link.txt 充当执行记录。

- ARM 完整 driver 原文：[改前](verify-armv7l/link-before.txt)、[改后](verify-armv7l/link-after.txt)、[全部 token 计数差异](verify-armv7l/link-token-diff.json)。
- aarch64 完整 driver 原文：[改前](verify-aarch64/link-before.txt)、[改后](verify-aarch64/link-after.txt)、[全部 token 计数差异](verify-aarch64/link-token-diff.json)。

CMake 重生成还会消除原链接行重复的 libbpf.a/libbcc.a/libbcc_bpf.a 项，不能声称命令行只有一个 token 变化。人工修改仅上述一行；`-shared-libgcc` 保留，`-static-libgcc` 不存在。两架构改后 C/CXX 编译目标公告均为 0 / 0，只进行了 static 重链接。两套保护清单分别 5007 / 5007 项哈希全部一致（含主包构建目录、未改源文件和原副本 CMake）；收尾额外 10 项原 spec、原 ELF 与归档身份核对通过。

原 spec 已有 `_toolchain` 已定义且 clang 才生成 static 的分支。此次局部 CMake 改动只在 STATIC_LINKING 且自定义 libclang 分支内生效；主包 STATIC_LINKING=OFF 不受该行控制。GCC/未定义路径继续安装 Source1002。本轮未重跑这四格，引用前轮 `BPF_W1_0921` 的实测，不把静态核查冒充重测。W4 纳入配方仍需保留 libc++ 条件。

两架构隔离环境 payload 预检实测 `w5.xzdio`（`cells/input-*/build.log`）。**本轮输出是 ELF，不是新 RPM；ELF 本身没有 payload 压缩属性**。既有六格 RPM 不被改写，不能拿旧 RPM 声称已安装本轮新 ELF。是否追加 static-only 写包已询问；未获得确认，未自行扩展为整包重构建。

## 体积对照

| 架构 | 原 Source1002 ELF | 上轮 libc++ static ELF | 本轮 ELF | 较上轮变化 |
|---|---:|---:|---:|---:|
| armv7l | 98974296 | 90847040 | 90870376 | +23336 |
| aarch64 | 129681256 | 136127672 | 136139424 | +11752 |

原资产和上轮数字及 SHA 取自 `docs/progress/BPF_W1_0921/ELF_COMPARISON.tsv`，本轮重新校验对应文件身份，见 `INPUT_IDENTITY_FINAL.json`。本轮 strip 使用正式 brp 的 `-g`，不是全剥离；这不是与原资产剥离策略完全等价的证明。每架构只交一个最终 ELF；未剥离与中间全剥离文件保留在 tmp，不冒充最终候选。

## 取消探针

原样使用既有 `condition_cancel_probe.cpp`，源码 SHA `5942cf9cbb335e57b2660e71c6f7db521af8fce01d708413a4841fcd4e7fa805`。同套 x86_64 libc++/libc++abi 静态归档、共享 libgcc，附加 `--whole-archive libbpf.a --no-whole-archive`，确保 libbpf 代码实际进入探针；nm 确认 `bpf_object__open` 为已定义符号。四种等待形态各 5 次，20/20 取消穿过、清理恰好一次、join 取得取消值。源文件、编译器、归档身份、完整编译与运行命令/退出码见 `probe/`。

探针依赖 libatomic、libpthread、libm、libgcc_s、libc，不是 bpftrace 候选的精确四依赖契约；该实验仅证明已测 x86_64 配置未重现原静态展开器失败，不能证明 ARM 板上行为或所有取消场景。未运行板子。

## 资源与交接

开工 2026-09-22 13:04 +08，硬截止 17:04。实际内存 cgroup 上限 16536457216 字节、nice19、ionice idle、串行构建；已存在 Makefiles，使用 `-j1`，未改成 Ninja。实际 `/proc/PID/cgroup` 与 `memory.max` 在各 cell events。开工磁盘153GiB，I/O 探测0.175秒；aarch64 前151GiB、0.209秒，无 I/O 暂停。构建目标不足500，没有触发该阈值探测。脚本执行快照及SHA保留。超大 ELF、复制树和完整 nm/strings 输出留 tmp，报告附路径与SHA，不提交平台二进制。

上板输入与缺口见 [W3_INPUTS.md](W3_INPUTS.md)。修法、链接与本轮 ELF 门禁通过，**发布替换仍须新资产写包/安装验证、功能等价和 ARM 物理板验证以及人工审阅**。本轮不启动 QuickBuild。

自行判断和技术性异常完整记录于 [DECISIONS.md](DECISIONS.md)：包括 strip 策略校正、曾在链接未完成时提前读取 ELF 的技术性非零、CMake 去重与 probe 强制保留归档。本轮未把这些记作构建失败。未修订原判定。
