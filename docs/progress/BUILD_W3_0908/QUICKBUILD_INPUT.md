# Base libc++ 适配 QuickBuild 输入

## 可用提交

| 包 | 分支 | 提交 | 状态 |
| --- | --- | --- | --- |
| libcxx-runtimes | `sandbox/lhmax2025/libcxx-ehabi-backport` | `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa` | 已完成四格验证，可作为先决包 |
| abseil-cpp | `sandbox/lhmax2025/libcxx-migration` | `7a4f04dad6d30ba09c406837cdd23ad779a69210` | READY |
| boost | 同上 | `4168e873584ae50a12733262026c007dce18ec89` | READY |
| icu | 同上 | `dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330` | READY |
| jsoncpp | 同上 | `9cbab147f44097d5b8da2b3ef38f75376a282d01` | READY |
| libsigc++ | 同上 | `4f2f592d2f88eb6a3a949cec8970a4cd5d40165b` | READY |
| pcre | 同上 | `09a5bfc9a96c4235628ce800db3b95bf28ef2f23` | READY |
| taglib | 同上 | `8b552bdf27c913a93bca664c41c3fafb214962a1` | READY |
| tensorflow2 | 同上 | `6d76139029ca4162249667f0db65a73387a94cfe` | READY（armv7l/GCC 采用相对基线无回归判据） |
| llvm | 同上 | `NOT_AVAILABLE` | NOT_READY；候选未提交/未推送 |
| bcc-tools | 同上 | `NOT_AVAILABLE` | NOT_READY；依赖新 llvm |
| bpftrace | 同上 | `NOT_AVAILABLE` | NOT_READY；依赖新 llvm 与 bcc-tools |

当前不能把 11 包作为一轮“全部适配完成”的 QuickBuild 输入。可用的是
`libcxx-runtimes` 与上表八个独立包；LLVM 链三包仍不得冒充 READY。

## 构建顺序

依赖链固定为：

```text
libcxx-runtimes → llvm → bcc-tools → bpftrace
```

独立于该链的实际包共有八个：`abseil-cpp`、`boost`、`icu`、`jsoncpp`、
`libsigc++`、`pcre`、`taglib`、`tensorflow2`。任务条文写“其余 7 个”，
与 11 包清单减去 LLVM 链三包的算术结果不一致；本材料没有擅自漏掉任一包，
该计数疑问已登记供人工确认。

## QuickBuild 后的核查

1. 每个动态 C++ 产物用 `readelf -d` 核对 `libc++.so.1`，并确认不再依赖
   `libstdc++.so.6`；模板型 `libsigc++` 还须以实际编译命令中的
   `-stdlib=libc++` 与 `std::__1` 符号痕迹作为判据，不能强制制造无用依赖。
2. `tensorflow2` 是静态归档：复跑精确归档成员归属及真实 consumer 链接
   检查，不能用 `DT_NEEDED` 判断 `.a`。
3. LLVM 链完成后，确认 `bcc-tools → llvm` 的 584 个和
   `bpftrace → llvm` 的 201 个布局敏感 C++ 符号两端均为 libc++。
4. 对 `libc++-devel` 复查五处头文件防护；对 `libLLVM.so.22.1` 复查 AOT
   对象为 `std::__1` 且实际 release-mode MLGO 决策可观测。
5. 保留 GCC/非迁移配置的对照构建，确认没有引入 `-stdlib=libc++`，并按
   各包原有行为核查回归。

## 未覆盖

QuickBuild 尚未在本任务中触发。本文件只是人工后续执行所需的输入身份、
顺序与核查点，不把本地结果外推为 QuickBuild 结果。
