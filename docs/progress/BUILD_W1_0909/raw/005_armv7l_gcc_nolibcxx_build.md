# armv7l GCC / 不启用 libc++ 完整构建

同一次实验覆盖两个相互重叠但判据不同的矩阵格：

- GCC 路径：实际编译器必须为 GCC，且 libc++ 选项不得进入命令；
- 不启用 libc++：`_toolchain` 未定义，必须继续选择原有资产与
  libstdc++ 产物。

实际命令原文：

```bash
nice -n 15 ionice -c 3 gbs -c /home/toolchain/development/libc++_replacement/config/gbs.conf build -A armv7l -B /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l -C --fail-fast --threads 1 --define '_smp_mflags -j2' --define 'mlgo_build_jobs 2' --release 109.3 --style git --include-all --packaging-dir packaging --spec llvm.spec
```

完整输出临时落盘于
`tmp/BUILD_W1_0909/armv7l_gcc_nolibcxx.log`。构建完成 7,148/7,148
个 Ninja 步骤，随后完成安装、RPM 后处理与写包；GBS 最终退出码为
`0`。构建日志报告的包内构建时间为 7,745 秒。

配置阶段已观测事实：

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-DCMAKE_CXX_COMPILER=armv7l-tizen-linux-gnueabi-g++
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- Performing Test LLVM_USES_LIBSTDCXX - Success
```

展开后的 CMake 命令未含 `-stdlib=libc++`、`-lc++abi` 或
`LLVM_ENABLE_LIBCXX=ON`。实际构建进程为 `/bin/ninja -j 2`；没有超过
本任务并行度 2 的限制。

配置完成后，将构建根实际选中的 `mlgo_verify_assets` 与原始
`packaging/mlgo_arm_model.tar.gz` 分别按相对路径和文件 SHA256 生成清单，
再用 `cmp -s` 比较：两侧均为 10,866 个文件，`cmp` 退出码 `0`。这项
实测证明未启用 libc++ 时选取的资产内容与原有输入逐文件一致，而不仅是
从 spec 条件分支推断。

## 产物核查

第一次核查调用原文（执行环境在进程启动前拒绝，故没有进程退出码）：

```bash
set -o pipefail
root=tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
out=tmp/BUILD_W1_0909/product_checks/armv7l_gcc
rm -rf "$out"
docs/progress/BUILD_W1_0909/code/verify_llvm_gcc_nolibcxx_build.sh "$root" armv7l "$out" 2>&1 | tee tmp/BUILD_W1_0909/armv7l_gcc_product_check.log
rc=${PIPESTATUS[0]}
printf 'verify_exit=%s\n' "$rc"
exit "$rc"
```

记录：`NO_EXIT_CODE (process not started)`。修正方式不是放宽判据，而是
去掉清理动作、改用从未存在的唯一输出目录。实际成功调用原文：

```bash
set -o pipefail
root=tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0
out=tmp/BUILD_W1_0909/product_checks/armv7l_gcc_20260909
mkdir -p "$out"
docs/progress/BUILD_W1_0909/code/verify_llvm_gcc_nolibcxx_build.sh "$root" armv7l "$out" 2>&1 | tee tmp/BUILD_W1_0909/armv7l_gcc_product_check.log
rc=${PIPESTATUS[0]}
printf 'verify_exit=%s\n' "$rc"
exit "$rc"
```

输出与退出码：

```text
LLVM_GCC_NOLIBCXX_BUILD_PASS arch=armv7l
verify_exit=0
```

核查覆盖：实际编译器为 GCC；`libLLVM.so.22.1` 直接依赖
`libstdc++.so.6`，不依赖 `libc++.so.1` 或 `libc++abi.so.1`；
`CMakeCache.txt` 与 `build.ninja` 中均无 `-stdlib=libc++` 和
`-lc++abi`；MLGO 运行时接口含原有 `std::__cxx11` 形态而不含
libc++ 资产的 `std::__1` 形态。

## 与固定基线的对照

固定基线是 `Tizen/Tizen-Base/tizen-base_20260827.214809` 中的
`libllvm-22.1.8-2.3.armv7l.rpm`（VCS 为
`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`）。候选是本次生成的
`libllvm-22.1.8-109.3.armv7l.rpm`。

命令原文：

```bash
set -o pipefail
baseline=tmp/BUILD_W1_0909/baseline_rpms/libllvm-22.1.8-2.3.armv7l.rpm
candidate=tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l/local/repos/tizen_unified_standard/armv7l/RPMS/libllvm-22.1.8-109.3.armv7l.rpm
out=tmp/BUILD_W1_0909/product_checks/armv7l_baseline_compare_20260909
mkdir -p "$out"
docs/progress/BUILD_W1_0909/code/compare_llvm_nonlibcxx_baseline.sh "$baseline" "$candidate" "$out" 2>&1 | tee tmp/BUILD_W1_0909/armv7l_baseline_compare.log
rc=${PIPESTATUS[0]}
printf 'compare_exit=%s\n' "$rc"
exit "$rc"
```

输出与退出码：

```text
file_list_diff_lines=0
requires_diff_lines=37
defined_dynsymbols_diff_lines=15020
mlgo_interface_diff_lines=0
compare_exit=0
```

因此可确认：RPM 文件路径集合相同，MLGO 接口清单相同，且两者均使用
libstdc++。不能声称整个产物逐字一致：固定快照与 2026-09-09 的滚动
reference 构建环境不同，依赖版本集合及导出动态符号集合存在实际差异。
依赖差异包括候选不再直接需要 `libdl.so.2`/`librt.so.1`，以及若干
GCC/libstdc++ 符号版本差异；定义动态符号对照为基线 56,188 个、候选
48,839 个。这些差异不属于本次 spec 条件分支或 MLGO 资产选择本身的
逐字等价证据，已保留完整 diff，不把它们推断为相同或无影响。

本次同一完整构建得到以下两项结果：

- armv7l / GCC 路径：`PASS`；
- armv7l / 不启用 libc++：`PARTIAL`。资产选择、标准库依赖与禁止参数
  均通过，但任务要求的“产物与现状完全一致”不能由跨日期固定快照
  证明；动态符号与依赖清单存在实际差异，故不将该格标为闭合。
