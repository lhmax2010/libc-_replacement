# T3 LLVM 22.1.8 libc++ / libc++abi 构建报告

状态：`RED_STOP_VERSION_SKEW`；T3-0 fail-closed，T3-1 三架构构建全部 `NOT_RUN`。

## 当前平台 clang 身份

从 T1 冻结的 Base Toolchain primary 元数据实际解析到：

- armv7l：`clang-22.1.8-18.1.armv7l`；
- x86_64：`clang-22.1.8-18.1.x86_64`。

URL、尺寸和 SHA256 见 `progress/T3/clang_runtime_rpms.tsv:1`。x86_64 的 clang、libllvm 与唯一缺失运行依赖 libxml2 均从同一快照下载，尺寸/SHA256 与 primary 完全一致，`rpm2cpio/cpio` 均为 0（`progress/T3/native_clang_rpm_verification.tsv:1`）。隔离 `LD_LIBRARY_PATH` 下原生执行输出 `clang version 22.1.8`、Target `x86_64-tizen-linux-gnu`，exit 0（`progress/T3/02_clang_version.log:1`）。第一次只放入 clang/libllvm 时因缺 `libxml2.so.16` 退出 127，失败原文保留，未以宿主近似 SONAME 替代（`progress/T3/02a_clang_version_missing_libxml2.log:1`）。

## 源码候选身份

当前平台 `llvm-22.1.8-18.1.src.rpm` 的完整源码树在 `llvm/CMakeLists.txt` 第 19 行包含 `LLVMVersion.cmake`，实际值位于该模块：Major 22、Minor 1、Patch 8、Suffix 空。源码 tarball SHA256 为 `16a932e1bad4e1ed68938b48aa9fe5613992426584a75dd17dacabf2151d122e`；平台 spec 的 VCS 字段为 `platform/upstream/llvm#c12cbe887ff08f04628e1b1165f8d02ce69a9146`（`progress/T3/03_current_source_identity.log:1`）。上游 `llvmorg-22.1.8` tag 可查询到且 peeled commit 为 `ca7933e47d3a3451d81e72ac174dcb5aa28b59d1`（`progress/T3/06_upstream_release_tag_probe.log:1`）。

但是该 SRPM tarball 不含 `.git`；`git describe`、本地 tag 与实际树 commit 均为 `NOT_AVAILABLE_NO_GIT_METADATA`。平台 spec VCS 与上游远端 tag 不能机械证明该已展开树的 Git 身份，因此不能用数值版本相等替代 T3-0 要求的源码 tag/commit 断言。

唯一可发现、带 Git 元数据且含 libc++ 自定义 packaging 的 worktree 是只读参考路径 `/home/toolchain/plan_evaluation/p1/libcxx_packaging/source/libcxx-runtimes-21.1.1`。其原文为：

- `git describe`: `upstream/21.1.1-6-g67e4b73a-dirty`；
- HEAD: `67e4b73a5f6a40545555ba6b1318807967f86678`；
- worktree spec 声明 `llvm_major 21`、`llvm_version 21.1.1`、`Version: 21.1.1`；
- `llvm/CMakeLists.txt` 不存在；
- worktree 有一项既存修改 `M packaging/libcxx-runtimes.spec`。

原文、spec 两版 SHA 与完整 diff 见 `progress/T3/04_legacy_source_identity.log:1`。该 Git 已识别构建输入与平台 clang 的 22.1.8 主次修版本不一致，直接满足 `RED_STOP_VERSION_SKEW` 条件；dirty worktree 也不能被视作 release 身份。

## 额外输入缺口与停止范围

当前唯一工作克隆的 `packaging/` 仅含 `multiarch_packaging_report.md` 与 `rpm_manifest.tsv`，`*.spec` 计数为 0（`progress/T3/05_workspace_packaging_input.log:1`）。因此即使忽略上面的版本红停，T3-1 所要求的“取自工作树 `packaging/` 的 spec commit + spec SHA256”仍缺失，必须停报；未自行从旧 21.1.1 spec 推导或制造 22.1.8 spec。

版本断言逐项见 `progress/T3/version_assertion.tsv:1`。armv7l、aarch64、x86_64 均未创建 buildroot，未解析构建根工具链 NEVRA，未生成或归档 RPM（`progress/T3/architecture_status.tsv:1`）。旧只读 worktree、当前 SRPM 源码与工作根 packaging 均未修改。
