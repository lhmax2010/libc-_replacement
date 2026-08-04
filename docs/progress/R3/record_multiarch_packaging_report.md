# libc++/libc++abi 21.1.1 三架构入仓候选报告

日期：2026-07-25  
结论：armv7l、aarch64、x86_64 的 `21.1.1-2` 候选 RPM 均已由
`gbs_llvm.conf` clean build 产出；三架构逐一通过版本化、依赖闭包和
红绿绑定三组硬门。当前没有 `PENDING_RUNTIME_CARRIER`。本任务未向
Gerrit、OBS 或任何平台仓提交。

## 1. 输入身份与构建命令

- 源仓 commit：
  `67e4b73a5f6a40545555ba6b1318807967f86678`。
- 三架构实际 GBS source tarball SHA256 完全相同：
  `867a37f81994efb213228e3475c3c51930187c6980e7ac36b8a5ecd5e4485026`。
- 最终 spec SHA256：
  `25db13b28204be28d21bab4a54cfbfc96b2abfc9e3f114f1041175a967f13f4f`。
- `gbs_llvm.conf` SHA256：
  `a32c1aba714714bfba8aba1b4e78bf015f297f4162bcff9652250c6d5b81f2b7`。
- 三条命令均有独立 buildroot、`-A`、`-C`，且 `--define` 数为 0。
  命令原文、SHA 和机械断言见 `command_integrity.tsv`、
  `armv7l/build/command.txt`、`aarch64/build/command.txt`、
  `x86_64/build/command.txt`。
- 三架构 debuginfo 的 `DW_AT_producer` 都是
  `clang version 21.1.1`；GNU AS 2.43 仅作为汇编 producer 同时出现。

三条最终构建退出码均为 0。逐字日志保存在各架构
`build/gbs_build.log`，最终门表见各架构 `gate_verdict.tsv`。

## 2. spec 复核与最小适配

### 数据支撑的结论

1. 原 spec 的 compiler、C++ compiler、ASM compiler target 和
   `LLVM_DEFAULT_TARGET_TRIPLE` 都取 `%{_host}`，无需硬编码架构
   triple。
2. ARM EHABI、LP64/DWARF EH 和架构 builtins 由 LLVM 上游按 target
   宏选择；aarch64/x86_64 不应继承 armv7l 的 EHABI 分支。
3. `--rtlib=libgcc` 与 `LIBCXXABI_USE_LLVM_UNWINDER=OFF` 可直接复用。
4. LLVM 21 runtimes 的最终安装目录由
   `LIBCXX_INSTALL_LIBRARY_DIR` 与 `LIBCXXABI_INSTALL_LIBRARY_DIR`
   决定；只设置 `CMAKE_INSTALL_LIBDIR` 对这两个 DSO 无效。

### 最终实际 diff

在初始 spec 上只发生以下候选打包改动：

- `Release: 1` → `Release: 2`；
- 增加 `ExclusiveArch: armv7l aarch64 x86_64`；
- 显式设置
  `LIBCXX_INSTALL_LIBRARY_DIR=%{_lib}`、
  `LIBCXXABI_INSTALL_LIBRARY_DIR=%{_lib}`；
- 增加 Release 2 changelog。

最终逐行 diff 为 `spec_review/spec_actual_v3.diff`。

适配严格遵守“先提案、后修改”：

- 初始提案：`spec_diff_proposal.patch`；
- v2 提案：`spec_diff_proposal_v2.patch`；
- 最终 v3 提案：`spec_diff_proposal_v3.patch`，修改前
  `git apply --check` 退出 0。

失败历史没有删除：

- armv7l 首轮失败是三个 `--include-all` 同时导出同一 Git 工作树，
  争用 `.git/gbp_index.lock`，见
  `armv7l/build/gbs_build_attempt1.log`；
- 64 位首轮和 v2 均完成编译，但库仍装入 `/usr/lib`，RPM 清单要求
  `/usr/lib64`，见各架构 `gbs_build_attempt1.log` 与
  `gbs_build_v2.log`；
- v3 显式使用 LLVM runtimes 专用变量后，armv7l 实装 `/usr/lib`，
  aarch64/x86_64 实装 `/usr/lib64`，三者均成功。

`spec_review/spec_review_followup_v2.md` 已明确标
`SUPERSEDED`，最终依据为 `spec_review_followup_v3.md`。

## 3. 候选制品

每架构产出 7 个二进制 RPM 和 1 个 SRPM：

- `libc++`、`libc++-devel`、`libc++-debuginfo`；
- `libc++abi`、`libc++abi-devel`、`libc++abi-debuginfo`；
- `libcxx-runtimes-debugsource`；
- `libcxx-runtimes` SRPM。

| 架构 | NEVRA 基线 | RPM 数 | RPM 字节数 | digest |
|---|---:|---:|---:|---|
| armv7l | 21.1.1-2.armv7l | 8 | 25,560,728 | 8/8 OK |
| aarch64 | 21.1.1-2.aarch64 | 8 | 25,646,186 | 8/8 OK |
| x86_64 | 21.1.1-2.x86_64 | 8 | 25,667,405 | 8/8 OK |

逐 RPM NEVRA、payload size、SHA256 在各架构 `rpm_manifest.tsv`。
候选目录为各架构 `artifacts/`；其中已生成本地 `repodata/`，可直接作为
后续 wave 构建的本地 repo。`rpm -K --nosignature` 原文在
`evidence/rpm_digest_check.log`，三架构均 8/8 digest OK。

## 4. 硬门 A：全部导出为 `@@LLVM_21`

统计口径是 dynsym 中已定义、GLOBAL/WEAK、DEFAULT/PROTECTED、
非 UND、非 ABS 的实际可导出符号；ABS 的版本节点记录本身不算函数或
对象导出。

| 架构 | DSO | 导出数 | `@@LLVM_21` | 单 `@LLVM_21` | 无版本/其他 | 结论 |
|---|---|---:|---:|---:|---:|---|
| armv7l | libc++.so.1.0 | 1,965 | 1,965 | 0 | 0 | PASS |
| armv7l | libc++abi.so.1.0 | 371 | 371 | 0 | 0 | PASS |
| aarch64 | libc++.so.1.0 | 1,966 | 1,966 | 0 | 0 | PASS |
| aarch64 | libc++abi.so.1.0 | 369 | 369 | 0 | 0 | PASS |
| x86_64 | libc++.so.1.0 | 1,966 | 1,966 | 0 | 0 | PASS |
| x86_64 | libc++abi.so.1.0 | 369 | 369 | 0 | 0 | PASS |

三架构覆盖率均为 100.000%。`unversioned_exports.tsv` 和
`single_at_exports.tsv` 都只有表头，故“未版本化符号全量清单”为
空集。`readelf --dyn-syms` 与 `--version-info` 逐字输出在各架构
`evidence/*_dynsyms.log` 和 `evidence/*_version_info.log`。

## 5. 硬门 B：依赖闭包完整

三架构共同满足：

- `libc++.so.1` 直接依赖 `libc++abi.so.1` 和 `libgcc_s.so.1`；
- `libc++abi.so.1` 直接依赖 `libgcc_s.so.1`；
- `LD_BIND_NOW=1 ldd -r` 的完整闭包中，`libstdc++` 为 0，
  `libunwind` 为 0，强未解析为 0；
- 两个正式 DSO 都带 `BIND_NOW/NOW`。

aarch64/x86_64 另直接需要 `libatomic.so.1`；这是两架构实际产物数据，
不影响上述禁止项。完整 NEEDED 表见 `direct_needed.tsv`，运行闭包原文
见 `evidence/needed_closure_ldd_r.log`。

运行时归属也逐架构通过：

- `__cxa_finalize` → glibc；
- `_Unwind_RaiseException` → libgcc_s；
- `__cxa_throw`、`__cxa_allocate_exception`、
  `__gxx_personality_v0` → 候选 libc++abi。

机械判读见 `provider_family_gate.tsv`，绑定原文见
`evidence/smoke_provider_bindings.log`。

## 6. 硬门 C：混链红绿 fixture

每个架构都重新构建并执行同一对探针：

- 绿探针使用正式 `@@LLVM_21` 候选库。其
  `__cxa_throw`/`__cxa_allocate_exception` 两条绑定均落到候选
  `libc++abi.so.1`，判 `PASS_GREEN`。
- 红探针故意保留无版本 `__cxa_*` UND，并先全局装载
  `libstdc++.so.6`。两条绑定均错误落到 libstdc++，机械门将该候选
  判为 `EXPECTED_RED_DETECTED`，即负面对照被成功拒绝。

红探针进程本身退出 0 正是该缺陷的危险之处：动态链接器允许错误绑定；
“拒绝”由门禁看到错误 provider 后执行，而不是等待进程崩溃。

逐架构编译/运行命令及 SHA 在 `runtime_fixture_commands.txt`，
判读见 `binding_gate.tsv`；完整 `LD_DEBUG=bindings` 原文及摘录在
`evidence/{green,red}_bind.*` 和
`evidence/{green,red}_relevant_bindings.log`。

## 7. 运行载体与冒烟

冒烟程序覆盖 `std::string`、异常抛接和 `dynamic_cast`。

| 架构 | 本轮载体 | 退出码 | 状态 | 作用域 |
|---|---|---:|---|---|
| armv7l | QEMU user/binfmt + GBS chroot | 0 | PASS | 新候选本轮重跑；另有既往 RPI4 板上先例 |
| aarch64 | `qemu-aarch64-binfmt` + GBS chroot | 0 | PASS | 实际执行证据，不是静态推断 |
| x86_64 | x86_64 宿主原生 GBS chroot | 0 | PASS | 实际执行证据 |

宿主及 binfmt 原文在 `runtime_carrier_probe.log`；逐架构输出在
`runtime_smoke.tsv` 和 `evidence/smoke_run.log`。

aarch64 当前载体是 QEMU user，不等价于 aarch64 真机上的内核、驱动和
整镜像集成验证；因此它足以关闭本任务的运行冒烟载体缺口，但不能替代
入仓后的目标板系统测试。当前不存在因“完全无执行载体”而产生的
`PENDING_RUNTIME_CARRIER`。

## 8. 汇总与交付边界

权威汇总是 `multiarch_gate_matrix.tsv`：

- build：3/3 PASS；
- 版本化：7,006/7,006 个导出均为 `@@LLVM_21`，单 `@` 0，
  无版本 0；
- 闭包：三个架构的 libstdc++、libunwind、强未解析均为 0；
- 绿探针：3/3 PASS；
- 红负面对照：3/3 被正确识别并拒绝；
- 冒烟：3/3 实际运行 PASS。

制品、命令、逐字 readelf/ldd/LD_DEBUG 日志和判读表均已落在本目录。
入平台仓、Gerrit 提交、OBS 提交及仓权限动作不在本任务范围内，均未执行。
