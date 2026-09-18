# 运行中接续检查点（不是完成报告）

更新时间：2026-09-18 18:23 +08 左右；精确时刻以 raw 日志为准。

## 当前优先事项

ARM libc++ 已在 18:15:54 按子窗口受控停止（raw/355=241，子进程 -15），不是仍在运行。407 保存了 104 个对象的断点清单，0 个 RPM。408 已生成 W1 最终矩阵：四个回退格通过，aarch64 libc++ 因额外 libbpf 失败，ARM libc++ 未完成。W1_REPORT.md 已收口为 PARTIAL。

W1 已在 18:20:28 普通推送项目提交 5576313e4ab957b1ff009308451c2b04b5fe6297；419 暂存自检通过，426 确认其他三个分支未变、旧 HEAD 仍为祖先。不要重跑 W1。

W2 已开始：427 开工检查通过，开始 18:21:21，固定截止 **09-19 00:21:21**；不得重置。当前原执行句柄 **90693**，raw/432，cell **llvm-arm-whole-retry**；带失败 syscall strace 的完整 -bb，复用已批准的旧 LLVM BUILD 树，输出 tmp/WEEKEND_0918/rpm-arm-llvm/。433 确认已进入 %build。先查进程/日志，不重复启动。

若原命令完整退出 0，运行 cell `verify-llvm-arm-retry`（新 JSON 已准备）核验整轮及所有新 RPM；若失败，保存原错误与 syscall，不能把部分 RPM 升为完整。历史 18 RPM 身份继续保留。W2 收口后归档、自检、项目 commit/push，再进入 W3。最新 raw 编号 433，后续从 434 起。

## 不得重置的时钟与限制

- W1 开始 12:28:14，截止今天 18:28:14；总截止明天 09:28:14。
- W2 已开始，截止 09-19 00:21:21；W3/W4 尚未开始，分别至多 6/3 小时，且不得超过总截止。
- 构建单并行、nice 19、idle I/O，实际 cgroup `MemoryMax=16536457216`；Git 普通优先级。
- codes/ 只读；候选只在 tmp/；只提交本任务项目材料，不推任何包仓，不覆盖 Source1002。

## W1 历史启动与保存的断点（不要重复启动）

- ARM libc++ 完整构建：raw/355，cell `bpf-armv7l-libcxx`，约 2%。
- 本会话执行句柄 13121 已返回 241；406 未匹配遗留构建进程。不要再次启动或试图继续旧会话。
- 使用 `guard_arm_w1_window.py`，构建子窗口至约 18:16:14（提前 20 秒停止宽限），给 W1 的 18:28:14 总截止预留 12 分钟归档、核查、commit/push；未延长时限。SHA256 0a1694e1e608d82a2da4031f7b7a199084008392a359d88326dc819200d4d7e1。
- 本次未完整退出 0，故未运行 check-armv7l-libcxx，不伪造 RPM 通过。
- ARM 未定义格已经完整构建并核查通过（276/293），不要重跑。两 GCC 的 HAL v2 补查已通过（296/298），299 的 I/O 检查通过；不要再次使用同名 cell 创建目录。
- I/O 检查若超时，按 10 分钟、最多三次规则等待，不直接启动下一构建。
- ARM libc++ 已启动，不得重复启动或自行加入待裁决的 libbpf 修正；使用 QEMU Clang，不自行切换 ARM 原生工具集。358 已观测 C/CXX compiler Clang 22.1.8，CXXFLAGS 含 -stdlib=libc++。

## 已有结果

- ARM GCC：153 完整构建=0；224 的 RPM/宏检查通过；static 原件 SHA256 一致。补充 HAL 旧 helper 路径有误，296 已经 v2 重查通过，不能引用旧退出 0 为有效载荷检查。
- aarch64 GCC：226 完整构建=0，265 核查通过；298 的 v2 补查通过。
- ARM 未定义：276 完整构建=0，293 核查通过；120 条实际编译/链接命令无 libc++ 选项，static SHA 与原件一致。
- aarch64 未定义：300 完整构建=0，350 核查通过；同样 120 条无 libc++ 选项，static SHA 与原件一致。354 I/O 通过后开始 ARM libc++。
- aarch64 libc++：076 构建=0；105 static 额外动态依赖 libbpf.so.1，门禁失败。三份 RPM 保留，详见 first-rpm-set 记录。
- ARM libcxx-runtimes：109 全量 1833/1833、四份 RPM；136 实际静态归档核验通过，未上板。
- CLI `--undefine _toolchain` 在实际 rpmbuild 中未维持状态，269=1。275 已用 test-only `%undefine` + `%include` 入口实测两架构解析状态为 0；原候选 SHA 不变。274 预检误用 `-bb --nobuild` 的失败也保留，不覆盖。

`executed-matrix-v2.py` 为已快照、记 SHA 的汇总脚本，阶段结束运行它更新矩阵；353 的表为四格通过的中途检查点。原始版本恢复说明见 MATRIX_SCRIPT_VERSION_NOTES.md。`W1_REPORT.md` 也是草稿，要按最后结果修正。原始 cpio 与 rsync 错误已整理到 W2_FAILURE_PROVENANCE.md（309 原文证据）。357 已保存 36 个 guard 的精确匹配 SHA 版本。最新 raw 编号 360，后续用 361 起。

## 后续阶段

W1 完成或截止后如实写阶段报告、archive、限定路径安全自检、commit/push 到项目当前分支，然后进入 W2。现阶段尚未进行本任务的 commit/push。

- W2：`task_preflight.py W2`；`prepare_llvm_arm_retry.py` 创建新输出；cell `llvm-arm-whole-retry`。旧错误是 cpio ENOSYS，另有复制 rsync EPERM，不能合并归因。若 strace 技术上不可用，记录后可不带 strace 重试，保留原失败。完整构建成功才运行 `verify_llvm_retry.py`，不能把部分 RPM 升为整轮成功。
- W3：`task_preflight.py W3`；先 `board_preflight.py`，再准备并编译 `prepare_arm_cancel_probes.py`，执行 `board_cancel_check.py`。尚未联系物理板。若 ARM libc++ RPM 没有通过载荷门禁，安装/功能格不能假写完成；取消探针可独立做。`board_install_compare.py` 要求候选及原件 RPM 核验结果、正常依赖检查，不绕过依赖。所有板上连接异常按规则停报，检查清理记录。
- W4：`task_preflight.py W4`；执行 `audit_eleven.py` 和 `check_published_heads.py`；本轮所有新状态与已发布 11 包分开。取消机制范围参见 `CANCELLATION_EVIDENCE_SCOPE.md`，ARM 对照结果不得预填。

## 待人工答复

见 QUESTIONS.md：libc++ static 分支裸 `bpf` 改成已定位的静态归档变量；ARM 原生工具集切换。均未获答复，未实施。

项目已有无关脏文件必须保留。只 stage `docs/progress/BUILD_WEEKEND_0918/`；普通 push 被拒即停，不 force，不合并其他分支。
