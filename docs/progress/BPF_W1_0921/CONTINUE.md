# 本轮断点与继续顺序

更新时间：2026-09-22 02:46 +08:00。硬截止：2026-09-22 05:01 +08:00。

**最终状态优先**：所有构建与核验已完成，六格构建0、18RPM；四个回退格PASS，两个libc++仅额外libbpf.so.1挂账。最后ARM未定义格raw/386、446退出0，API61373/60691已关闭；输入复核raw/450 PASS74项，API19703已关闭。rpm_requires.py与report.py已运行，FINAL_RESULT.md与LINE_STATUS已写。下一步只做材料publish、暂存范围/凭据检查、项目commit/push及远端核对，不再构建或上板。最新原始编号455，下一456。下文旧断点仅作过程记录。

**最新优先状态**：ARM GCC 整轮退出0、三RPM核验PASS（raw/315、381、385）；static与Source1002逐字节一致、主包/参数审计PASS。最后一格 ARM undefined 已启动：`bpf-armv7l-undefined`，raw/386，API61373，原作用域与限制不变。下一步仅监控该格，退出0后 `verify-armv7l-undefined`，再运行 rpm_requires.py、recheck_inputs.py、report.py、更新LINE_STATUS并提交发布。最新编号386，下一387。下文00:57时的GCC运行状态已过时，保留为过程记录。

1. 对账原样推送：c7aa2c5e72f78516e40b7a5423a4b83673608bad。aarch64 阶段材料已推送：e44ad19ff271af0b7761b186ec49a6c0bb4b98cd。
2. aarch64 三路径整轮退出 0，9 RPM 已核验；GCC/未定义两格 PASS；libc++ 只剩额外 libbpf.so.1 动态依赖，不能标成可替换。见 MATRIX.json。
3. ARM libc++ 已完整退出0并写出3RPM，核验只剩额外libbpf.so.1依赖；原API3755已关闭。当前运行 ARM GCC，cell=bpf-armv7l-gcc，启动 raw/315，systemd 用户作用域 bpf-w1-0921.scope，API session72524。实际GNU14.2.0、LLVM22.1.8已见 raw/317。不得并行启动另一构建。
4. ARM 输入阻断已通过普通安装解除：原私有副本同 NEVRA 文件冲突，保留未修；从原始 Release115.1 根复制新私有树 input-armv7l-origin，普通 --test 与正式安装均退出0，没有 --replacefiles、--nodeps 或 sudo。已核对第五份 libclang.a SHA256 5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a；证据 INPUT_STATUS.md、origin-install-commands.json、prepare-armv7l-result.json。此前授权询问不再是执行前提，实际未使用该选项。
5. 四个 bwrap 私有目录绑定已改向 origin，其他参数、spec 不变。guard 强制核对安装成功、实际绑定和归档摘要。新环境三路径 rpm --eval 均 w5.xzdio，raw/206。
6. ARM libc++ 核验 raw/309 返回1因额外libbpf，不是写包失败；其余产物/主包/编译命令检查PASS，详细见verify-bpf-armv7l-libcxx/result.json、audit目录、DECISIONS.md。新static 90,847,040字节，SHA70d01ef4c0be1e40e8b02065865fb28729497a3010d04569886a16cd5ce86fbc，std1字面22952行，窄/宽cxx11均0。不要重跑。
7. 当前GCC完成后检查退出码；成功则verify-armv7l-gcc，再 bpf-armv7l-undefined、verify-armv7l-undefined。各 cell_commands 已备好；GCC根独立，不混入libc++LLVM。每个新构建前检查空间≥20GiB和IO。同一scope串行。若是写包失败，立即停止并按mode16字节单断点取证，参考在 docs/progress/ARM_LZMA_0921B/mode-entry（progress下该子目录不存在）；非写包失败保留现场继续独立格。
8. 构建/核验入口：systemd-run --user --scope --expand-environment=no --unit=bpf-w1-0921 -p MemoryMax=16536457216 nice -n 19 ionice -c 3 /usr/bin/python3 progress/BPF_W1_0921/guard.py W1 <cell>。用 run.py 为外层命令与退出码落盘。monitor.py <cell> 只读查看进度，资源与每500目标IO由guard监控。硬截止05:01。
9. 不改候选spec、不改源码、不推包仓、不覆盖Source1002、不上板。新W3输入表还包括既有探针与原资产本地SHA，见 W3_REFERENCE_INPUTS.json；前轮板端缺BCC/Clang动态库，ARM静态取消组前轮20/20通过，不能冒充本轮观察或写成必崩。
10. ARM 架构完成或截止停下后 report.py 汇总，更新 docs/LINE_STATUS.md 五节，publish.py 扫描/复制，git add 仅 docs/progress/BPF_W1_0921 与 docs/LINE_STATUS.md，check_staged.py 校验凭据和范围，再普通 commit/push 项目分支。禁止force，被拒即停；其他dirty文件不动。最终报告区分每格3RPM（共最多18）与6格、RPM完整与候选可发布。
11. 最新原始命令编号317，下次318。原始读文件命令少数未run.py包装，RESUME_AUDIT.md中记原文和退出码。raw/181监控test返回1不是构建失败，raw/183证明实际退出0。
12. 最终汇总前运行 rpm_requires.py 导出实际RPM依赖供W3，再运行 recheck_inputs.py 确认34份LLVM RPM来源/副本、实际libclang.a和spec摘要未变（只读原输入）。也要更新 report.py、docs/LINE_STATUS.md。原板上缺BCC/Clang库为前轮记录，当前未连接板子。准备的脚本未运行不能冒充结果。
13. 链接日志边界：两架构static配置未开verbose。static-link-*.json/.txt证明CMake生成的链接参数含-shared-libgcc、不含-static-libgcc，但实际driver命令回显NOT_OBSERVED；link_audit.py返回1/NOT_CLOSED是补充取证，不是rpmbuild失败。ARM仅一次/proc只读瞬时检查也未赶上链接进程，不能宣称捕获到。无需为补日志重建。两架构真实ELF均有libgcc_s，无动态libc++/LLVM；未做本轮取消/功能验证。
