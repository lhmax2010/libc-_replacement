# aarch64 完整构建、产物核查与 MLGO QEMU 用户态验证

载体说明：构建在本机 GBS aarch64 构建根完成；运行验证使用
`qemu-arm64-static` 用户态仿真。后者只证明目标用户态二进制及其 AOT
模型接口可运行，**不能替代 aarch64 物理板和目标内核行为验证**。

## 1. 增量续跑的技术性失败

首先尝试沿用上一阶段停在 `7376/7545` 的构建根执行 `--incremental`。
命令进入重新配置，CMake 报告：

```text
CMake Error at CMakeLists.txt:1193 (add_subdirectory):
  add_subdirectory given source
  "/home/abuild/rpmbuild/BUILD/llvm-22.1.8/mlgo_verify_assets/mlgo_sysroot/xla_aot_runtime_src"
  which is not an existing directory.
```

GBS 退出码：`1`。失败后在同一构建根内核对，该目录、其
`CMakeLists.txt` 和相应归档均存在。故该次结果记录为旧增量构建根重新
配置状态不一致，不作为候选代码失败；没有修改候选代码，随后执行完整
非增量重建。

完整原始输出保存在工作区临时区
`tmp/BUILD_W1_0909/aarch64_libcxx_resume.log`，未作为长期证据提交。

## 2. 完整非增量重建

完整命令：

```bash
nice -n 15 ionice -c 3 gbs \
  -c /home/toolchain/development/libc++_replacement/config/gbs_llvm.conf \
  build -A aarch64 \
  -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-aarch64 \
  --fail-fast --threads 1 \
  --define '_smp_mflags -j2' \
  --define 'mlgo_build_jobs 2' \
  --release 108.2 --style git --include-all \
  --packaging-dir packaging --spec llvm.spec
```

退出码：`0`。关键原始输出：

```text
[ 6902s] toolchain-2026 finished "build llvm.spec"
=== Total succeeded built packages: (1) ===
```

完整日志位于临时区
`tmp/BUILD_W1_0909/aarch64_libcxx_rebuild.log`（1,867,815 字节）。

## 3. 产物核查

使用上一阶段落盘的脚本：

```bash
docs/progress/BUILD_W1_0908/code/verify_llvm_libcxx_build.sh \
  /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0 \
  aarch64 \
  /home/toolchain/development/libc++_replacement/tmp/BUILD_W1_0909/product_checks/aarch64
```

退出码：`0`。脚本 SHA256：
`172c50ef03044b159bcd14e9b9520f9c6c256297e8db14d8d5349ad596806f83`。

实际证据：

```text
CMAKE_CXX_COMPILER:STRING=/bin/aarch64-tizen-linux-gnu-clang++
LLVM_ENABLE_LIBCXX:BOOL=ON
NEEDED Shared library: [libc++abi.so.1]
NEEDED Shared library: [libc++.so.1]
NEEDED Shared library: [libgcc_s.so.1]
```

同时确认：

- `libLLVM.so.22.1` 链接成功；
- 五个指定 XLA runtime 对象均被配置为嵌入；
- MLGO 相关符号使用 `std::__1`，未出现 `std::__cxx11`；
- 五处 libc++ 头文件防护检查全部命中（共输出 42 行匹配证据）。

## 4. QEMU 用户态 MLGO 实测

包装器：`docs/progress/BUILD_W1_0909/code/qemu_aarch64_opt_wrapper.sh`，
SHA256 `4b90959cbcfe66a7a18ca7e6bb526a482843f7cc0100c8f7a3fa1da429db657f`。

验证命令：

```bash
docs/progress/BUILD_W1_0908/code/verify_mlgo_release_mode.sh \
  docs/progress/BUILD_W1_0909/code/qemu_aarch64_opt_wrapper.sh \
  aarch64-tizen-linux-gnu \
  docs/progress/BUILD_W1_0908/fixtures/mlgo_inliner_input.ll \
  tmp/BUILD_W1_0909/runtime_checks/aarch64_qemu
```

退出码：`0`。关键输出：

```text
default_rc=0
release_rc=0
Pass: inline-ml count=6
ShouldInline count=6
AOT_MODEL_EXECUTION_PASS
```

release 输出另经 `opt -passes=verify -disable-output` 验证成功。默认与
release 最终 IR 的 SHA256 均为
`ad529c7439d60bc2887da821630f2fa3c7a3a1a558d5de0781c86ffd7160b5c5`；
remarks YAML SHA256 为
`3c1ff81406c67da240777b722ef88e06543ca70ca11b09da5c508f135b851ded`。

结论：aarch64 的 libc++ 构建、产物正确性和 QEMU 用户态 MLGO 运行格
均通过；物理板行为仍为 `NOT_OBSERVED`，符合本任务预先指定的 aarch64
载体口径。
