# R96：libcxx-runtimes.spec 编译器选择修正与四格验证

## 结论

状态：**BLOCKED；未提交、未推送 sandbox。**

确认后的方案 A 在两格 armv7l 环境中全部通过，也在 aarch64/GCC 环境中
正确选择了 Clang 22.1.8；但该 aarch64 构建随后被 GCC 标准环境注入的
`-mtune=cortex-a57.cortex-a53` 阻断。Clang 22.1.8 不接受该参数值，CMake
连简单 C 编译器测试都未通过，构建退出码为 1。

这不属于“仍选到了 GCC”，而是方案 A 与 aarch64 GCC 标准环境的现有
编译 flags 不兼容。由于四格必须全部成功是推送前硬门禁，失败后立即停止；
aarch64/LLVM 格未启动，sandbox 没有 commit、没有 push，正式分支和其他
sandbox 分支均未触碰。

## 1. 方案与预先判据

人工已确认采用 R94 选项 A：删除无效的
`%define _toolchain_override clang`，把本 spec 传给 CMake 的三个编译器
直接设为目标前缀 Clang：

```spec
-DCMAKE_C_COMPILER=%{_host}-clang
-DCMAKE_CXX_COMPILER=%{_host}-clang++
-DCMAKE_ASM_COMPILER=%{_host}-clang
```

选择机制和排除 B/C/D 的理由见 `STAGE1_DECISION.md`。四格在执行前已统一
申报为：实际编译器必须为 Clang 22.1.8、完整构建成功、libc++ 与
libc++abi 均直接依赖 `libgcc_s.so.1`、五处头文件全部命中；任一项失败
即停止且不推送。

## 2. 资源门禁

执行 `tools/resource_gate.sh --level medium`，退出码 0，结果 PASS。完整记录
见 `raw/001_resource_gate_medium.*`。所有构建均使用 `nice -n 15`、
`ionice -c 3`、GBS `--threads 1` 和 RPM `_smp_mflags -j2`。

## 3. aarch64 编译器名先行确认

在改动和构建前确认：

- `%{_host}` 在 aarch64 展开为 `aarch64-tizen-linux-gnu`；
- 构建根中存在 `aarch64-tizen-linux-gnu-clang` 与
  `aarch64-tizen-linux-gnu-clang++`；
- 两者均指向构建根内的 `usr/bin/clang-22`；
- 同构建族的实际历史 CMake 日志显示，使用这两个名称时 C/C++ 编译器均
  被识别为 Clang 22.1.8。

证据见 `raw/011_aarch64_compiler_name_gate_final.*`。因此满足“若二进制
不存在则停”的先行门禁，没有替换成其他名称。

## 4. spec 候选改动

只改了 `packaging/libcxx-runtimes.spec`：

```diff
diff --git a/packaging/libcxx-runtimes.spec b/packaging/libcxx-runtimes.spec
index a8a2e8ea078a..3e686becdf09 100644
--- a/packaging/libcxx-runtimes.spec
+++ b/packaging/libcxx-runtimes.spec
@@ -1,6 +1,5 @@
 %define llvm_major 22
 %define llvm_version 22.1.8
-%define _toolchain_override clang
 
 Name:           libcxx-runtimes
 Version:        22.1.8
@@ -68,9 +67,9 @@ cd build
 
 cmake -G Ninja \
   -DCMAKE_BUILD_TYPE=Release \
-  -DCMAKE_C_COMPILER=%{__cc} \
-  -DCMAKE_CXX_COMPILER=%{__cxx} \
-  -DCMAKE_ASM_COMPILER=%{__cc} \
+  -DCMAKE_C_COMPILER=%{_host}-clang \
+  -DCMAKE_CXX_COMPILER=%{_host}-clang++ \
+  -DCMAKE_ASM_COMPILER=%{_host}-clang \
   -DCMAKE_C_COMPILER_TARGET=%{_host} \
   -DCMAKE_CXX_COMPILER_TARGET=%{_host} \
   -DCMAKE_ASM_COMPILER_TARGET=%{_host} \
```

候选 spec SHA256：
`f17402b373a9d74841faf28e364b23c48bc54771a5ff8e96173c5a530b0d350e`。
完整 diff 与工作树范围核验见 `raw/013_*`、`raw/014_*`。除该文件外没有
平台文件改动，也没有平台未跟踪文件。

删除 `_toolchain_override` 的原因：Tizen-Base 实际消费链不会用这个包内
宏重选 `%{__cc}` / `%{__cxx}`；R94 已实测加入它后仍选择 GCC 14.2.0，
故它是无效修复。因本轮未通过推送门禁，没有创建相应 commit message；
上述原因已在本报告固化，未来获准提交时必须写入提交说明。

## 5. 四格结果

| 架构 | 环境 | 实际编译器 | 构建 | 两库 `libgcc_s.so.1` | 五处头文件 | 判定 |
| --- | --- | --- | --- | --- | --- | --- |
| armv7l | GCC 标准仓 | Clang 22.1.8 | PASS | PASS | PASS | PASS |
| armv7l | LLVM Toolchain 仓 | Clang 22.1.8 | PASS | PASS | PASS | PASS |
| aarch64 | GCC 标准仓 | Clang 22.1.8 | **FAIL** | NOT_OBSERVED | NOT_OBSERVED | **FAIL** |
| aarch64 | LLVM Toolchain 仓 | NOT_OBSERVED | NOT_RUN_AFTER_STOP | NOT_OBSERVED | NOT_OBSERVED | NOT_RUN_AFTER_STOP |

机器可读表见 `RESULT_MATRIX.tsv`。

### 5.1 armv7l / GCC 标准仓

完整构建退出码 0。CMake 实际收到
`armv7l-tizen-linux-gnueabi-clang` / `-clang++`，识别为 Clang 22.1.8；
RPM 全部生成。产物核验退出码 0：

- `libc++.so.1.0` 与 `libc++abi.so.1.0` 的动态依赖均含
  `libgcc_s.so.1`；
- `__ostream/basic_ostream.h`、`future`、`istream`、`string` 中的
  `__forced_unwind` 防护及 `cxxabi.h` 的类定义全部命中；
- 实际代码片段完整记录于 `raw/016_verify_armv7l_gcc_env.stdout`。

命令与完整构建输出见 `raw/015_*`，核验见 `raw/016_*`。

### 5.2 armv7l / LLVM Toolchain 仓

完整构建退出码 0。实际编译器同样为目标前缀 Clang 22.1.8；RPM 全部
生成。两库 `libgcc_s.so.1` 依赖和五处头文件检查全部 PASS。命令与完整
输出见 `raw/017_*`，产物核验及实际代码片段见 `raw/018_*`。

### 5.3 aarch64 / GCC 标准仓

完整构建尝试退出码 1。方案 A 已经生效：CMake 命令明确为
`aarch64-tizen-linux-gnu-clang` / `-clang++`，C/C++ 均识别为
Clang 22.1.8。失败发生在 CMake 的简单 C 编译器测试：

```text
aarch64-tizen-linux-gnu-clang: error: unsupported argument
'cortex-a57.cortex-a53' to option '-mtune='
```

实际命令行中的来源是 GCC 标准环境注入的：

```text
-march=armv8-a+fp+simd+crc+crypto -mtune=cortex-a57.cortex-a53
```

所以没有生成可核验 RPM，展开器依赖与五处头文件均为 `NOT_OBSERVED`。
完整输出在 `raw/019_*`，定位摘录在 `raw/020_*`。

### 5.4 aarch64 / LLVM Toolchain 仓

`NOT_RUN_AFTER_STOP`。第三格已不符合预先判据，按“任一格失败即停、不得
以其余格通过为由推送”的明确纪律，没有启动第四格。终态检查也确认不存在
任何名称包含 aarch64/LLVM 的本轮构建日志，见 `raw/022_*`。

## 6. 推送门禁与分支对照

推送前基准和停止后的状态均为：

- sandbox 本地 HEAD：`dd4beffc30be18841818651756f4dbe1f54b0016`；
- sandbox 远端 HEAD：`dd4beffc30be18841818651756f4dbe1f54b0016`；
- 正式 `tizen_base`：`6771dbc48b7e3db2ec53919a8eebd3514374137b`。

推送前记录的其他 sandbox 分支为：

| 分支 | SHA |
| --- | --- |
| `sandbox/lhmax2025/mlgo` | `08ffd8cd7c72f5dd6e612d8395362def96d84029` |
| `sandbox/lhmax2025/mlgo_aot` | `88ff57cadcff52ab50ca6865db7faed7388358fe` |
| `sandbox/lhmax2025/llvm_only_clang` | `f8277158cf44f872e2b2d62a2e839cb33bd0afea` |

本轮没有执行任何 platform commit 或 push，因而目标分支、其他 sandbox
分支和正式分支均未被本轮改变。完整前态见 `raw/012_*`，停止核验见
`raw/022_*`。候选 spec diff 保留在本地工作树供人工审阅，未进入任何提交。

## 7. 给总部的说明材料

由于四格未全部通过且没有新提交，本轮不应在 Gerrit 声称修复完成。可直接
用于同步当前阻断事实的措辞如下：

> We replaced the ineffective package-local `_toolchain_override` with explicit
> target-prefixed Clang drivers in the CMake invocation. The previous macro did
> not affect compiler selection in the Tizen-Base build environment, where the
> RPM compiler macros still resolved to GCC.
>
> The candidate succeeds in both armv7l standard-repository and LLVM-toolchain
> environments with Clang 22.1.8. Both produced libc++ and libc++abi shared
> libraries depend on `libgcc_s.so.1`, and all five packaged-header checks pass.
>
> The aarch64 standard-repository build selects Clang 22.1.8 as intended, but
> fails during CMake's compiler test because that environment supplies the
> GCC-specific option `-mtune=cortex-a57.cortex-a53`, which Clang rejects. We
> stopped at that point, did not run the fourth cell, and did not push the
> candidate change.

这段仅是人工材料；本任务未在 Gerrit 留言。

## 8. 自行判断、未覆盖范围与尚存疑问

自行判断：在第三格失败后只做只读日志定位，没有尝试过滤、改写或替换
`-mtune`，因为那会超出已确认的方案 A 和“只处理编译器选择”的最小改动
范围，并引入新的方案裁决。

未覆盖范围：aarch64/LLVM 构建；aarch64 两格的产物依赖与头文件核验。

尚存疑问：aarch64 GCC 标准环境中，后续应当让 spec 针对 Clang 规范化
该环境注入的 `-mtune=cortex-a57.cortex-a53`，还是应从平台 aarch64
构建 flags 的来源修正。两种做法的作用域不同，需要人工裁决后再继续，
本轮未自行选择。

## 9. 技术性命令错误

终态核验时两次误用日志包装器：第一次把输出目录重复放入前缀，包装器在
执行主体前因目标目录不存在而失败，且自身未成功落盘；第二次少传了标签后
的命令字段，由 `raw/021_stop_audit_bash.*` 记录为退出码 127。两次均未
执行核验主体、未改变 Git 或构建状态。随后按脚本声明的
`NUMBER LABEL COMMAND...` 接口正确重跑，`raw/022_stop_audit.exitcode` 为 0。

## 10. 最终纪律确认

- 只改了候选 `packaging/libcxx-runtimes.spec`，未改其他平台文件；
- 四格未全部通过，未提交、未推送 sandbox；
- 未新建分支，未使用 force；
- 未推送正式分支，未向 LLVM 上游提交；
- 所有已执行构建的命令、完整输出与退出码均保存在 `raw/`。
