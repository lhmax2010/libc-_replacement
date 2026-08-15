# R41：armv7l EHABI 重抛失败的机制定位

## 结论先行

责任归属为 `LIBCXXABI_SIDE`，具体限定为本项目 S1 实验实现与
libc++abi 既有 EHABI cleanup bookkeeping 的不完整衔接；未实现修法。

失败不是 `_Unwind_Resume_or_Rethrow` 忽略了强制展开状态。板上实测
`reserved1` 非零，libgcc 随后确实进入 forced phase 2，并以
`STARTING|FORCE`（`state=0x9`）重新调用 personality。直接失败链为：

1. 首次 forced unwind 中，S1 的类型映射匹配 `__forced_unwind`
   handler，`ttype=2`；
2. libc++abi 的 EHABI personality 对所有 `HANDLER_FOUND` 使用同一
   “non-catching handler”路径，并无条件调用 `__cxa_begin_cleanup`；
3. 因异常为 foreign，该调用把异常登记到
   `globals->propagatingExceptions`，实测从 `nil` 变为该异常 header；
4. handler 内 `throw;` 调用 `_Unwind_Resume_or_Rethrow`。该调用没有
   返回，而是启动新的 forced phase 2 遍历；
5. 重抛后的第一次 personality 调用返回 `CONTINUE_UNWIND(8)`；第二次
   命中 `ttype=0` 的真正 cleanup，而非再次命中 forced type；
6. 进入第二次 `__cxa_begin_cleanup` 时，实测
   `propagatingExceptions` 已是同一 foreign exception。libc++abi
   的既有保护条件因此调用 `std::terminate`，进程退出 134。

所以第一处语义偏离发生在首次 handler 安装前：S1 将“forced typed
handler”加入 scan 结果后，EHABI caller 没有保存“handler / cleanup”
区分，把 handler 当作 cleanup 登记。终止只是这一状态污染在后续真正
cleanup 上的可观测结果。

## 1. R40 原始返回行为

输入及 SHA256：

- armv7l：`docs/progress/R40/commands/056_run_diag_arm_cancel_personality.log`，
  `553cfb82ab4cf5d1069f7bccc5cf72d2c5eb0398cab6204ae4e371c9d074eae6`；
- x86_64：`docs/progress/R40/commands/049_run_diag_x86_cancel_return.log`，
  `1259e067b0a8a81db23ddf9447ad8ce164c13fc751358f09857faf7a0d3243f5`。

armv7l 的完整 stderr 已逐字收录于
`commands/004_r40_original_observation.log`。其中没有
`R40_RESUME_OR_RETHROW_RETURN`，计数为 0；x86_64 同样为 0。
因此任务书询问的 return `reason` 必须记为 `NOT_OBSERVED`，不能把
personality 的 `reason=6/8` 误写成 `_Unwind_Resume_or_Rethrow` 的
返回值。

armv7l 中 `_Unwind_Resume_or_Rethrow` 未返回；失败发生在它内部启动
的 forced phase 2。x86_64 中它也未返回，但 unwind 正常转移控制并
最终得到 `join=PTHREAD_CANCELED`、退出 0。

## 2. libgcc ARM forced-resume 静态路径

完整原文和行号见 `commands/006_libgcc_arm_path_full.log`。

- `unwind-arm-common.inc:77-81`：forced stop function 存于
  `unwinder_cache.reserved1`，argument 存于 `reserved4`；
- `:470-565`：`unwind_phase2_forced` 逐帧调用 personality。参数
  `resuming=0` 时首个 state 为
  `_US_UNWIND_FRAME_STARTING | _US_FORCE_UNWIND`；只有
  `resuming=1` 才使用 `_US_UNWIND_FRAME_RESUME | _US_FORCE_UNWIND`；
- `:520-541`：personality 返回后调用 stop function；返回
  `CONTINUE_UNWIND` 才继续，返回 `INSTALL_CONTEXT` 则恢复寄存器并
  转移控制；
- `:645-655`：`_Unwind_ForcedUnwind` 写入 reserved1/reserved4 后以
  `resuming=0` 启动；
- `:714-723`：`_Unwind_Resume_or_Rethrow` 在 reserved1 非零时把 PC
  置为当前 call site，并再次以 `resuming=0` 调用 forced phase 2；
- `_URC_HANDLER_FOUND=6`、`_URC_INSTALL_CONTEXT=7`、
  `_URC_CONTINUE_UNWIND=8` 的枚举原文见同一证据文件。

故预期调用序列是：reserved1 非零 → forced phase 2 → personality
收到 `state=0x9` → personality 选择 CONTINUE 或 INSTALL → libgcc
调用 stop function或转移到 landing pad。R41 板上序列与 libgcc 这段
实现一致。

## 3. 实际调用序列与第一处偏离

板上载体为 `192.168.108.26:26101`，原生 armv7l、无 qemu。运行时
`ldd` 与 `LD_DEBUG=libs` 证明加载：

- `/var/tmp/r41_s1_20260815/libc++abi.so.1`，SHA256
  `884b96e69c77c0a4d966b913500ded8fd7a90dd78e2d21d06f4534e8010720cb`；
- `/lib/libgcc_s.so.1`。

armv7l 测试件逐字复用 R40 diagnostic probe；其原始链接命令含
`-Wl,--export-dynamic`，用于保持 R39/R40 已证实的 typeinfo 匹配
前提，从而只观测重抛环节。完整命令见
`code/evidence/prior_r40/036_build_diag_arm_probe_retry2.log`。该选项
未被当作 S1 方案的一部分；本任务也未改变测试件。

完整输出见 `commands/038_run_arm_cancel_v2.log`。决定性行是：

- 首次匹配：`kind=forced-type ... ttype=2 reason=6`；
- 首次 cleanup 登记：`native=0 propagating=(nil)`，随后
  `propagating=<exception_header>`；
- 重抛：`reserved1` 非零；
- 重抛后第一个 frame：`scan reason=8`；
- 重抛后第二个 frame：`landing=... ttype=0 action_record=(nil)`；
- 第二次登记：`native=0 propagating=<same header>`；
- 终止：`reason=foreign-stack-nonempty`，退出 134。

`ttype=0` 和 `action_record=(nil)` 证明第二个 `reason=6` 是 cleanup，
不是同一 forced handler 的再次类型匹配。R40 仅打印 `reason=6`，不足以
作此区分；R41 补齐了这一缺口。

x86_64 宿主原生对照见 `commands/046_host_runtime_identity.log`。它使用
同一 S1 逻辑，首次 typed handler 后的调用依次为 reason 8、ttype=0
cleanup 的 reason 6、reason 8，最终
`join=PTHREAD_CANCELED`、退出 0。x86_64 不走 EHABI 的
`__cxa_begin_cleanup` propagating stack 路径。

逐步三方表见 `tables/call_sequence_comparison.tsv`。

## 4. 为什么归属 libc++abi 侧

libc++abi 实验源的原文见 `commands/008_libcxxabi_experiment_static.log`
和 `commands/045_static_mechanism_focus.log`：

- S1 在 `scan_eh_tab` 中把 forced typed catch 标为
  `_URC_HANDLER_FOUND`；
- EHABI personality 随后的单一 `HANDLER_FOUND` 分支注释为
  “Found a non-catching handler”，无条件调用 `__cxa_begin_cleanup`；
- `__cxa_begin_cleanup` 明确禁止第二个 foreign propagating exception。

GCC/libstdc++ 的 ARM 实现则保存 `found_handler` 与 `found_cleanup` 的
区别，只在 `found_type == found_cleanup` 时调用
`__cxa_begin_cleanup`。它还通过 `GNUC FOR` exception class 把 forced
状态交给 `__cxa_type_match`。原文见
`commands/007_libstdcxx_arm_reference.log`、
`commands/045_static_mechanism_focus.log`。

同板、同一系统 libgcc 的既有 libstdc++ 对照命中 forced handler 后
得到 `PTHREAD_CANCELED`、退出 0；证据复用及 SHA256 见
`commands/044_libstdcxx_board_control_reuse.log`。这排除了“libgcc ARM
无法支持该场景”的归因。

因此选择 `LIBCXXABI_SIDE`：问题是 S1 实验映射新增了 EHABI 过去没有
的结果形态，却没有同步保持 handler/cleanup 分类。这里没有对平台
源码作改动，也没有实现修复。

## 5. 上游线索

GitHub CLI 本机不可得，原始失败为 `gh: command not found`，见
`commands/039_github_upstream_issue_search.log`；随后使用 GitHub
官方 Search API，命令、退出码和结果见
`commands/042_github_api_issue_search_corrected.log`。

- LLVM issue #53849 仍为 OPEN，标签含 backend:ARM、libc++abi；但其
  报告是 forced_unwind 测试的 typedef 编译错误，并非本次重抛机制；
- 未检索到 LLVM issue 精确描述本次
  `pthread_cancel + ARM EHABI + foreign typed handler + rethrow` 组合；
- GCC 2008 讨论精确说明 ARM 必须把 forced 状态传给
  `__cxa_type_match`，对应代码已于 GCC r137683（本地 git
  `e32717fc8929c80c03c13fbd584437d93605acff`）合入，当前源码仍存在；
- GCC 2004 ARM EABI 集成说明 ARM frame unwind 由 language
  personality 参与；
- GCC 2012 线程取消讨论有“handler 内 forced unwind 重抛后 abort”
  的相关案例，但其架构是 x86_64，仅列为相关而非同根因证据。

检索清单见 `tables/upstream_clues.tsv`，一手材料原文见
`commands/043_upstream_primary_sources.log` 与
`commands/050_gcc_history_forced_mapping.log`。

## 6. 纪律、资源与清理

- 所有配置和构建均为 runtimes-only、并行度 2、
  `LLVM_PARALLEL_LINK_JOBS=1`、`nice -n 15`、`ionice -c 3`；
- 独立资源门禁通过 `bash -n` 与干跑，每次构建前显式调用并检查；
- 初始 `commands/022_build_arm.log` 在工具会话交接时缺少末尾退出码，
  随即在相同 build dir 重新执行目标，Ninja 报 `no work to do` 且退出
  0，见 `commands/023_*` 至 `025_*`；没有掩盖该记录缺口；
- 板上仅部署本任务两文件；收尾删除目录后独立核验
  `DEPLOY_PATH_PRESENT=NO`、`PROJECT_PROCESS_COUNT=0`，见
  `commands/048_board_cleanup.log`、`049_board_cleanup_independent_verify.log`；
- 未安装 RPM、未改系统库/配置、未重启或启停服务。

## 覆盖边界

结论覆盖本次 S1 实验实现、Tizen 11.0 armv7l、平台 libgcc_s 和
x86_64 对照。观测打印只执行 `fprintf`，没有改分支条件、返回值、
展开寄存器或控制转移。未调试或修改 libgcc；静态路径由平台对应 GCC
源码与运行序列互证。未实现修法，不做方案推荐。

## 交付记录

- 证据主提交：`baf0afa84bb1eb5850d0c21c65358cb71355ef9e`；
- 已推送至 `origin/codex/r5-ehabi-diagnosis`；
- 远端 `refs/heads/codex/r5-ehabi-diagnosis` 经 `git ls-remote`
  核验与本地主提交一致；原始推送与核验记录见
  `commands/057_push_primary.log`、`059_remote_verify_primary_corrected.log`；
- 首次远端核验命令因 `awk` 转义错误退出 1，原样保留于
  `commands/058_remote_verify_primary.log`，随后仅修正取列方式并复核通过。
