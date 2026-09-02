# R96：libcxx-runtimes.spec 编译器选择修正与四格验证

## 结论

状态：**PASS；四格全部通过，已普通快进推送 sandbox。**

平台提交为 `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`，已从
`dd4beffc30be18841818651756f4dbe1f54b0016` 快进追加到
`sandbox/lhmax2025/libcxx-ehabi-backport`。提交只改
`packaging/libcxx-runtimes.spec`，未新建分支、未使用 force、未触碰正式
`tizen_base` 或其他 sandbox 分支，也未向 LLVM 上游提交。

最终四格均实际使用 Clang 22.1.8、完整构建成功；产出的 libc++ 与
libc++abi 都直接依赖 `libgcc_s.so.1`，规定的五处安装头文件检查全部通过。

## 1. 方案与预先判据

人工确认采用方案 A：删除无效的 `%define _toolchain_override clang`，将
CMake 的 C/C++/ASM 编译器明确设为目标前缀 Clang：

```spec
-DCMAKE_C_COMPILER=%{_host}-clang
-DCMAKE_CXX_COMPILER=%{_host}-clang++
-DCMAKE_ASM_COMPILER=%{_host}-clang
```

R97 后续证实，aarch64 的 GCC 默认环境注入 GCC 专用复合参数
`-mtune=cortex-a57.cortex-a53`，LLVM 环境则使用 Clang 可接受的
`-mtune=cortex-a53`。按人工确认，只在 aarch64 构建块内精确替换这一项，
并同时处理 CFLAGS/CXXFLAGS；CMake 的 ASM flags 也取自已处理的 CFLAGS。
其他环境参数原样保留。选择 `cortex-a53` 是与 LLVM project_config 的目标
状态对齐，不是另行选择新的平台调优值。

四格执行前统一判据为：实际编译器必须为 Clang 22.1.8、构建成功、两库
均直接依赖 `libgcc_s.so.1`、五处安装头文件全部命中；任一格失败或不可得
即不得推送。

## 2. 资源与 aarch64 编译器门禁

`tools/resource_gate.sh --level medium` 退出码 0、结果 PASS。构建均使用
`nice -n 15`、`ionice -c 3`、GBS `--threads 1` 与 RPM
`_smp_mflags -j2`。证据见 `raw/001_*` 与续跑时的 `raw/029_*`。

改动前已确认 aarch64 下 `%{_host}` 展开为
`aarch64-tizen-linux-gnu`，构建根内存在
`aarch64-tizen-linux-gnu-clang` 和 `-clang++`，均指向 `clang-22`；证据见
`raw/011_*`。没有换用其他名称。

## 3. 最终 spec 改动

```diff
diff --git a/packaging/libcxx-runtimes.spec b/packaging/libcxx-runtimes.spec
index a8a2e8ea078a..f2a0c0c0e8ab 100644
--- a/packaging/libcxx-runtimes.spec
+++ b/packaging/libcxx-runtimes.spec
@@ -1,6 +1,5 @@
 %define llvm_major 22
 %define llvm_version 22.1.8
-%define _toolchain_override clang

 Name:           libcxx-runtimes
 Version:        22.1.8
@@ -63,14 +62,20 @@ cp %{SOURCE2} .
 cp %{SOURCE3} .

 %build
+%ifarch aarch64
+CFLAGS="$(printf '%%s\n' "$CFLAGS" | sed 's/-mtune=cortex-a57\.cortex-a53/-mtune=cortex-a53/g')"
+CXXFLAGS="$(printf '%%s\n' "$CXXFLAGS" | sed 's/-mtune=cortex-a57\.cortex-a53/-mtune=cortex-a53/g')"
+export CFLAGS CXXFLAGS
+%endif
+
 mkdir -p build
 cd build
 
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

最终 spec SHA256 为
`155701f3da8d54d649ccd05620a9f20cda82cd5a249eaa232d5e614c3fb085a5`。
完整 diff 另存 `SPEC_CHANGE.diff`，范围核验见 `raw/030_*`、`raw/045_*`。

删除 `_toolchain_override` 的原因：Tizen-Base 的实际消费链不会因包内定义
该宏而重选 `%{__cc}` / `%{__cxx}`；R94 已实测加上该行后仍选择 GCC
14.2.0。新方案不依赖该机制，而直接把目标前缀 Clang 交给 CMake。

## 4. 四格验证矩阵

| 架构 | 环境 | 实际编译器 | 构建 | libc++ / libc++abi 的 `libgcc_s.so.1` | 五处头文件 | 判定 |
| --- | --- | --- | --- | --- | --- | --- |
| armv7l | GCC 默认环境 | Clang 22.1.8 | PASS | PASS / PASS | PASS | PASS |
| armv7l | LLVM 环境 | Clang 22.1.8 | PASS | PASS / PASS | PASS | PASS |
| aarch64 | GCC 默认环境 | Clang 22.1.8 | PASS | PASS / PASS | PASS | PASS |
| aarch64 | LLVM 环境 | Clang 22.1.8 | PASS | PASS / PASS | PASS | PASS |

机器可读结果见 `RESULT_MATRIX.tsv`。

### 4.1 armv7l 两格

两格均退出码 0。GCC 默认环境证据为 `raw/015_*`、`raw/016_*`；LLVM
环境为 `raw/017_*`、`raw/018_*`。CMake 明确使用目标前缀 Clang 并识别为
22.1.8；两库的动态段均含 `NEEDED libgcc_s.so.1`，五处头文件的实际代码
片段均已原样落盘。由于本次新增规范化受 `%ifarch aarch64` 限定，续跑时
没有无意义地重跑这两格，沿用同一最终候选中已经完成的结果。

### 4.2 aarch64 / GCC 默认环境

规范化后的构建退出码 0，完整输出见 `raw/031_*`。日志证明：

- 进入规范化前 CFLAGS/CXXFLAGS 含 `-mtune=cortex-a57.cortex-a53`；
- 进入 CMake 时 C/C++/ASM flags 都只含 `-mtune=cortex-a53`；
- 规范化后复合参数命中数为 0；
- C/C++ 编译器均识别为 Clang 22.1.8。

定位证据见 `raw/035_*`。产物核验 `raw/036_*` 退出码 0、结果 PASS：两库
均直接依赖 `libgcc_s.so.1`，五处安装头文件均匹配到实际防护代码。

该环境继承的 CFLAGS 中还含 `-Wl,-z,relro,--as-needed`，Clang 在若干编译
步骤报告其为未使用的 linker input。它不是本次参数替换产生的告警，构建
仍成功；完整告警保留在 `raw/031_build_aarch64_gcc_normalized.stdout`。

### 4.3 aarch64 / LLVM 环境

完整构建退出码 0，见 `raw/037_*`。环境原本已经使用
`-mtune=cortex-a53`，精确替换前后内容不变；CMake 的 C/C++/ASM flags
保持该值，复合参数命中数为 0，C/C++ 编译器识别为 Clang 22.1.8，证据见
`raw/040_*`。产物核验 `raw/041_*` 退出码 0、结果 PASS：两库均直接依赖
`libgcc_s.so.1`，五处安装头文件全部命中。

## 5. 提交、推送与分支核对

推送前远端目标为 `dd4beffc30be18841818651756f4dbe1f54b0016`，本地 HEAD
相同，工作树仅修改 spec。提交者按要求为
`hao.lin <hao.lin@samsung.com>`。新提交：

```text
c3f8578a4db871d9d6de96d751f4c2ea7b1638fa
packaging: select Clang explicitly for libc++ runtimes
Change-Id: I088643ca32ce84bd4f065309a3cd32fb5a6e7a19
```

普通命令 `git push origin
HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport` 成功；未使用任何
force 形式。推送后核对确认：

- 目标远端 HEAD 为 `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`；
- `dd4beffc…` 是新 HEAD 的祖先，之间恰好 1 个提交，既有提交未丢；
- `mlgo=08ffd8cd…`、`mlgo_aot=88ff57ca…`、
  `llvm_only_clang=f8277158…` 均未变化；
- 正式 `tizen_base=6771dbc4…` 未变化；
- 平台工作树干净。

原始记录见 `raw/042_*` 至 `raw/049_*`。

## 6. 给总部的说明材料

可直接用于 Gerrit 留言：

> The previous package-local `_toolchain_override` did not affect the RPM
> compiler macros in the Tizen-Base build environment, so GCC was still
> selected and rejected the Clang-only `--rtlib=libgcc` option.
>
> The updated spec passes the target-prefixed Clang drivers directly to CMake.
> For the transitional combination where the default environment is GCC but
> this package explicitly selects Clang, the aarch64 GCC-specific composite
> `-mtune=cortex-a57.cortex-a53` is normalized to `-mtune=cortex-a53`. This is
> the value already used by the LLVM project configuration; other environment
> flags are left unchanged. No platform-side configuration change is needed,
> because the LLVM environment already supplies the compatible value.
>
> We built all four combinations: armv7l and aarch64 in both GCC-default and
> LLVM environments. Every build used Clang 22.1.8 and completed successfully.
> In every cell, both libc++ and libc++abi directly depend on `libgcc_s.so.1`,
> and all five packaged-header checks for the forced-unwind changes pass.

本任务未自行在 Gerrit 留言。

## 7. 自行判断、疑问与未覆盖范围

自行判断：

1. 将规范化限制为 `%ifarch aarch64`，并只精确替换已证实不兼容的完整
   参数值，避免改变其他架构或其他 flags；取值采用人工建议且与 LLVM
   配置一致。
2. armv7l 两格不重跑，因为其结果已来自同一最终候选，而续跑新增代码仅在
   aarch64 生效；原始结果继续作为四格证据。
3. aarch64/GCC 环境的 linker-input warning 被完整保留并申报，但不判为
   本次规范化失败，因为它来自未改动的环境 flags，且构建及所有硬判据通过。

尚存疑问：无阻塞疑问。

未覆盖范围：本任务只验证 libcxx-runtimes 包的两架构、两环境构建及规定的
产物判据，不等同于全平台软件包重编或运行时全量回归测试。

## 8. 技术性命令错误

续跑期间两次把相对日志器路径用于平台子工作目录，shell 在执行主体前报
“No such file or directory”；均未执行 GBS/Git 主体、未改变状态，随后改用
绝对路径重跑。另一次本地 `.git/hooks/commit-msg` 可执行性探测返回 1，
原因是该工作树的 hook 实际通过共享仓库/配置路径生效；最终提交已自动带
Change-Id。项目推送后的首次只读核验还因 awk/sed 转义错误漏打两个远端
字段；推送本身已成功，随后用简化命令重跑，`raw/061_*` 明确显示项目与
平台两组本地/远端 SHA 分别一致。上述技术错误均不属于判据失败。

## 9. 最终纪律确认

- 平台源码仅改 `packaging/libcxx-runtimes.spec`；
- 四格全部通过后才提交并推送；
- 只普通快进推送既有 sandbox 分支；
- 未新建分支、未使用 force；
- 未修改 project_config、未推送正式分支、未向 LLVM 上游提交；
- 所有实际执行的构建、核验和 Git 操作均以完整命令、stdout、stderr、退出码
  保存于 `raw/`。
