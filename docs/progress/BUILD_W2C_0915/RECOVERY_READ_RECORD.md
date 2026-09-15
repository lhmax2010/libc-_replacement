# 一项查看历史记录时的路径错误

以下为根据工具已返回结果补记的恢复记录，不是日志包装器自动生成。
此项只读取历史命令记录，没有启动构建或改变配置。

命令原文：

```sh
sed -n '1,100p' progress/BUILD_W2C_0915/raw/050_verified_scope_processes.command.txt
```

实际退出码：`2`。

实际错误输出：

```text
sed: can't read progress/BUILD_W2C_0915/raw/050_verified_scope_processes.command.txt: No such file or directory
```

修正：不再猜文件名，用 `find` 定位到
`050_live_resource_check.command.txt`（raw/095，退出0）。
本轮 bcc-tools 的实际进程检查另用 check_scope.py 执行，raw/097、105均退出0。

时间汇总脚本首版也将045的标签误写为 `045_confirmed_config_check`，
产生“精确标签未找到”的记录（raw/139），不是该次CMake证据不可得。
raw/140实查正确文件名为 `045_actual_cmake_confirmation.started.txt`；
旧版脚本另存 timing_summary_v1.py（raw/141），修正标签后重新生成时间表。
# 最终收口补记

raw/165的交付格式自检退出1：llvm_confirm_config.sh中从原RPM片段保留的
4行行尾空格触发git diff --check。执行快照不得为格式检查而改变，故保留
脚本字节及执行前哈希，把格式检查限定到说明文档/Python文件；实际执行
shell脚本仍做bash -n与哈希核对。自检v1另存，失败原文不改写。
这不改变任何构建或产物门禁，凭据模式扫描在格式检查之前已通过。

最终整理的一次apply_patch因同一patch同时删除并新增STATUS.md而被工具拒绝，
未改变文件；改为单一Update操作后成功。属于材料编辑的技术性失败，
与构建/验证判据无关，未修改任何原始输出。

以下是会话续接时的只读定位命令与实际退出码恢复记录，不冒充run_logged自动记录：

```text
python3 progress/BUILD_W2C_0915/poll_cell_brief.py build-w2c-bpftrace-aarch64-gcc => 0（多次，完整读取另入cell_monitor_reads.jsonl）
test -f progress/BUILD_W2C_0915/finalize_tables.py => 0
ls progress/BUILD_W2C_0915 => 0
sed -n '1,240p' progress/BUILD_W2C_0915/finalize_tables.py => 0
rg --files -g AGENTS.md -g '!codes/**' -g '!tmp/**' -g '!progress/**' -g '!docs/**' => 1（此范围未匹配）
sed -n '1,220p' progress/BUILD_W2C_0915/CONTINUATION_REPORT.md => 0
sed -n '1,200p' progress/BUILD_W2C_0915/STATUS.md => 0
sed -n '1,220p' progress/BUILD_W2C_0915/COMPATIBILITY_REVIEW.md => 0
cat progress/BUILD_W2C_0915/timing_summary.json => 0
sed -n '1,200p' progress/BUILD_W2C_0915/verify_bpftrace_cell.py => 0
ls progress/BUILD_W2C_0915/raw/11* progress/BUILD_W2C_0915/raw/12* progress/BUILD_W2C_0915/raw/15* => 0
cat progress/BUILD_W2C_0915/raw/157_final_audit_inputs.stdout.txt => 0
cat progress/BUILD_W2C_0915/final_selfcheck.json => 0
tail -n 8 progress/BUILD_W2C_0915/raw/159_report_source_checks.stdout.txt => 0
rg -n 'CMAKE_CXX_COMPILER:|clang version 22|COMPILER_AND_LIBCXX|Source1002|%post|cp .*bpftrace|FIXED_PATH|CMAKE_CXX_COMPILER_ID|CMAKE_CXX_COMPILER_VERSION|LLVM_ENABLE_LIBCXX:BOOL' progress/BUILD_W2C_0915/raw/159_report_source_checks.stdout.txt => 0
cat progress/BUILD_W2C_0915/raw/160_cmake_output_excerpt.stdout.txt => 0
```
