# R104 实施阶段：LLVM 首格阻断记录

状态：`BLOCKED_AWAITING_DECISION`

## 1. 已执行范围

- 仅修改了 `codes/R104/llvm/packaging/llvm.spec`；其他十个包未修改。
- 仅启动了 LLVM 的第一格：`armv7l × Tizen-Base-Toolchain 默认 Clang/libc++`。
- 构建命令使用 `nice -n 15`、`ionice -c 3`、GBS `--threads 1`、Ninja
  `-j2`；资源监控未发现内存或负载越界。
- 构建实际识别为 Clang 22.1.8，C++ 编译命令实际含
  `-stdlib=libc++`；由此证明新条件和 `LLVM_ENABLE_LIBCXX=ON` 已生效。

其余五个 LLVM 格未运行；`bcc-tools` 与 `bpftrace` 遵照“LLVM 有问题则
不要继续”而未修改、未构建。其他八个独立包也暂未修改，等待人工确认
本次新岔路的处置范围。

## 2. 失败结果

首格在链接 `lib/libLLVM.so.22.1` 时失败，GBS 退出码为 `1`，Ninja 停在
`2352/7147`。没有生成任何 runtime RPM。

失败不是 GCC 误收 `-stdlib=libc++`，也不是命令包装错误。链接命令明确是：

- driver：`armv7l-tizen-linux-gnueabi-clang++`；
- C++ 编译/链接参数：`-stdlib=libc++`；
- 失败点：`ld.lld` 合并新编译 LLVM 对象与 spec 捆绑的 MLGO/XLA
  预编译对象。

主要错误包括：

```text
undefined symbol: tensorflow::XlaCompiledCpuFunction::LookupResultIndex(
  std::__1::basic_string<...> const&) const
undefined symbol: tensorflow::XlaCompiledCpuFunction::LookupArgIndex(
  std::__1::basic_string<...> const&) const
undefined symbol: std::__cxx11::basic_string<...>::_M_create(...)
undefined symbol: std::__cxx11::basic_string<...>::_M_replace(...)
```

完整命令与完整输出分别在：

- `raw/089_build_llvm_armv7l_libcxx.command.txt`
- `raw/089_build_llvm_armv7l_libcxx.stdout`
- `raw/089_build_llvm_armv7l_libcxx.stderr`
- `raw/089_build_llvm_armv7l_libcxx.exitcode`
- `build_logs/llvm_armv7l_libcxx_fail.log`

## 3. 根因的独立符号证据

`llvm.spec` 将 `Source1002: mlgo_arm_model.tar.gz` 解开到
`mlgo_verify_assets`，并通过
`LLVM_MLGO_EMBED_TF_XLA_RUNTIME_OBJECTS` 把其中五个既有 `.o` 直接嵌入
LLVM 静态库及最终共享库。这些对象不是本轮用新 flags 重新编译的对象。

符号逐对象统计显示：

| 对象类别 | 代表对象 | `std::__cxx11` 行数 | `std::__1` 行数 |
| --- | --- | ---: | ---: |
| 捆绑对象 | `custom_call_status.cc.o` | 2 | 0 |
| 捆绑对象 | `executable_run_options.cc.o` | 3 | 0 |
| 捆绑对象 | `xla_compiled_cpu_function.cc.o` | 3 | 0 |
| 新编 LLVM | `MLInlineAdvisor.cpp.o` | 0 | 116 |
| 新编 LLVM | `MLRegAllocEvictAdvisor.cpp.o` | 0 | 69 |

同一接口的定义/引用更直接地证明 ABI 不匹配：

- 捆绑 `xla_compiled_cpu_function.cc.o` **定义**的
  `LookupArgIndex` / `LookupResultIndex` 参数是
  `std::__cxx11::basic_string`；
- 新编 `MLInlineAdvisor.cpp.o` 和 `MLRegAllocEvictAdvisor.cpp.o` **引用**的
  同名方法参数是 `std::__1::basic_string`；
- 因 mangled name 不同，两边不是可互相解析的同一符号。

完整符号记录见 `raw/119_*` 与 `raw/120_*`。这也意味着简单追加
`-lstdc++` 不能补出缺失的 `std::__1` 版本接口，而且会把两套 C++ ABI
同时带进 `libLLVM`，不能作为未经裁决的修复。

## 4. 需要人工裁决的岔路

当前获准的方案只改变 LLVM spec 的宿主构建标准库；失败揭示了此前设计
材料未识别的输入约束：三架构 MLGO/XLA 预编译资产本身也必须与宿主
LLVM 使用同一 C++ ABI。

可供裁决的方向至少包括：

1. 为三架构重新生成/替换使用 libc++ 构建的 MLGO/XLA 资产，再重跑
   LLVM 六格；这会改动或重新生成 spec 所引用的二进制资产，超出当前
   “只改 spec 与必要构建配置、不改源码”的已明确实施内容。
2. 在 libc++ 路径禁用 MLGO；这会改变交付功能，不应由执行方自行决定。
3. LLVM 暂留 libstdc++；这与已确认的迁移判定及 LLVM/BCC/bpftrace 的
   布局敏感边目标冲突。

需人工确认两个问题：

- 是否授权采用某一方向处理 LLVM 的 MLGO 资产；若是，采用哪一方向？
- 在 LLVM 链阻断期间，是否继续实施与它无依赖的其余八个包，还是整个
  R104 保持停止？

## 5. 未发生的操作

- LLVM 修改未提交、未推送；远端目标分支仍为
  `2d23367d74afbf2bb1e9e4013fce072b3a154109`。
- 其余十个仓库目标分支 SHA 均未改变。
- 未推送正式分支，未使用 force。
- 失败后没有启动第二个构建，也没有自行修改 MLGO 资产或绕过错误。
