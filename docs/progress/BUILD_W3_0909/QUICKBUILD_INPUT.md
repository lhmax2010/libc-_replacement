> **本文档按既有理解编写，未与 QuickBuild 实际界面核对；若格式与实际不符，请告知我们调整。**

# Base libc++ 适配 QuickBuild 输入

## 当前可用性

11 个目标包目前只有八个独立包具备已验证并推送的适配提交。LLVM 门禁未
闭合，`llvm`、`bcc-tools`、`bpftrace` 不可作为 READY 输入。因此本文是
后续验证的输入清单与顺序说明，**不是一份可以立即触发 11 包全量验证的
完成态清单**。

| 包 | 分支 | 提交 | 状态 |
| --- | --- | --- | --- |
| libcxx-runtimes | `sandbox/lhmax2025/libcxx-ehabi-backport` | `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa` | READY；四格验证完成 |
| abseil-cpp | `sandbox/lhmax2025/libcxx-migration` | `7a4f04dad6d30ba09c406837cdd23ad779a69210` | READY |
| boost | 同上 | `4168e873584ae50a12733262026c007dce18ec89` | READY |
| icu | 同上 | `dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330` | READY |
| jsoncpp | 同上 | `9cbab147f44097d5b8da2b3ef38f75376a282d01` | READY |
| libsigc++ | 同上 | `4f2f592d2f88eb6a3a949cec8970a4cd5d40165b` | READY |
| pcre | 同上 | `09a5bfc9a96c4235628ce800db3b95bf28ef2f23` | READY |
| taglib | 同上 | `8b552bdf27c913a93bca664c41c3fafb214962a1` | READY |
| tensorflow2 | 同上 | `6d76139029ca4162249667f0db65a73387a94cfe` | READY；armv7l/GCC 为相对未改基线无回归 |
| llvm | 同上 | `NOT_AVAILABLE` | NOT_READY；候选未提交/未推送 |
| bcc-tools | 同上 | `NOT_AVAILABLE` | NOT_READY；依赖新 llvm |
| bpftrace | 同上 | `NOT_AVAILABLE` | NOT_READY；依赖新 llvm 与 bcc-tools |

## 构建顺序

依赖链必须按以下顺序：

```text
libcxx-runtimes → llvm → bcc-tools → bpftrace
```

其余八个包不依赖该链，可在 libcxx-runtimes 可用后独立安排：
`abseil-cpp`、`boost`、`icu`、`jsoncpp`、`libsigc++`、`pcre`、`taglib`、
`tensorflow2`。

## 产品核查点

1. 动态 C++ 产物用 `readelf -d` 核对 `libc++.so.1`，并确认不再依赖
   `libstdc++.so.6`。模板型 `libsigc++` 需结合实际编译命令中的
   `-stdlib=libc++` 与 `std::__1` 符号痕迹，不能为满足判据强制留下无用依赖。
2. `tensorflow2` 是静态归档；复跑精确归档成员归属与真实 consumer 链接
   检查，不能用 `DT_NEEDED` 判断 `.a`。
3. LLVM 链完成后，确认 `bcc-tools → llvm` 的 584 个与
   `bpftrace → llvm` 的 201 个布局敏感 C++ 符号两端均使用 libc++。
4. 对 `libc++-devel` 复查五处头文件防护；对 `libLLVM.so.22.1` 核对新
   AOT 对象使用 `std::__1`，并实际观察 release-mode MLGO 决策。
5. 保留 GCC/非迁移配置对照，确认构建命令没有引入 libc++ 选项，并按各
   包原有产品行为核查回归。

## 尚未核对

本任务未触发 QuickBuild，也未用实际 QuickBuild 页面验证输入格式或调度
行为。P2 工具用于日志拉取，不证明创建构建时的界面字段与本文一致。
