# 本轮断点与继续顺序

更新时间：2026-09-21 22:14 +08:00。硬截止：2026-09-22 05:01 +08:00。

1. 对账原样推送已完成：c7aa2c5e72f78516e40b7a5423a4b83673608bad。
2. aarch64 libc++：整轮退出 0，3 RPM 已核验；唯一未闭合载荷检查是额外 `libbpf.so.1`，不自行修复。aarch64 GCC：整轮及全部检查 PASS，3 RPM，static 与 Source1002 逐字节一致。
3. 当前运行：`cells/bpf-aarch64-undefined`，systemd 用户作用域 `bpf-w1-0921.scope`。原始启动记录 raw/130；不得同时再启动编译。完成后先检查 exitcode/build.log。写包失败立即停止并按任务书单断点取证；非写包失败保留现场、如实记录。
4. 完成后用 `cell_commands/verify-aarch64-undefined.json` 配合 guard.py 执行检查。检查脚本 check_cell.py 顺序执行 RPM 载荷核验、spec/实际编译命令核验、主包全部 ELF 依赖核验。检查失败不能改成 PASS。
5. aarch64 三路径完成后立即生成 report.py、更新 docs/LINE_STATUS.md 五节骨架、归档本轮材料并只 commit/push 项目分支。先检查 staged 范围与凭据，普通 push 被拒即停。不要推包仓。
6. ARM libc++ 当前不得启动：新 22 RPM 已复制校验，但安装 --test 因 211 个同名 llvm-static-devel 文件冲突返回 19。已询问是否允许仅在本轮私有副本 --replacefiles；**截至本记录未收到批准**。`installed-armv7l-inputs.json` 是拟安装名单，不是安装成功证明，见 INPUT_STATUS.md。guard 对 ARM libc++ 强制核对安装 PASS 和第五份 libclang.a 摘要。
7. ARM GCC 与未定义路径使用原有独立 GCC 根，不依赖受阻的 libc++ 输入；可以在 aarch64 结束后继续，依次构建、检查。所有已准备的命令在 cell_commands，各路径 topdir 独立，payload 预检均为 w5.xzdio。
8. 若收到 ARM 输入替换批准，再严格按批准范围处理、记录事务与最终 .a 摘要；不得绕过依赖检查，不改原构建根。新 archive 目标 SHA256：5deb3c6c662c73865e8a38f4f8df8d6e672af2ae8a6a8faae7700b5909c1524a。
9. 构建/检查入口：`systemd-run --user --scope --expand-environment=no --unit=bpf-w1-0921 -p MemoryMax=16536457216 nice -n 19 ionice -c 3 /usr/bin/python3 progress/BPF_W1_0921/guard.py W1 <cell>`；通过 run.py 落盘原命令与退出码。新 cell 开始前确认上一作用域已退出，磁盘 ≥20 GiB、I/O 正常。
10. codes/ 只读，不改候选 spec（本轮复制 SHA 已记录）、不覆盖 Source1002、不上板。最终报告区分 RPM 完整性与候选验收；W3 只交输入清单。项目仓材料用 publish.py 扫描/复制到 docs/progress/BPF_W1_0921，Git 使用普通优先级；保留其他既有工作树修改。
