# 当前会话上下文恢复核对

2026-09-22 01:01 +08:00 恢复核对：原 ARM GCC 构建继续运行，没有重启。以下只读命令均退出 0：`pwd`；`date -Is`；`sed -n '1,180p' progress/BPF_W1_0921/CONTINUE.md`；`sed -n '1,120p' progress/BPF_W1_0921/run.py`；`sed -n '1,100p' progress/BPF_W1_0921/HOURLY_STATUS.md`。随后执行命令由 raw/319 起记录。截止仍为 05:01，未因界面提示或会话恢复延长。

2026-09-21 22:34 +08:00 恢复核对，未重启或替换正在运行的 aarch64 未定义工具链构建。执行的只读命令：`pwd`（退出 0）、`tail -n 25 progress/BPF_W1_0921/cells/bpf-aarch64-undefined/build.log`（退出 0）、`sed -n '1,220p' progress/BPF_W1_0921/CONTINUE.md`（退出 0）、`sed -n '1,260p' progress/BPF_W1_0921/report.py`（退出 0）、`sed -n '1,240p' progress/BPF_W1_0921/test_original_arm.py`（退出 0）、`date --iso-8601=seconds`（退出 0）。随后命令原文与输出继续由 raw/151 起逐项记录。

raw/154 的 `sed` 返回 2，原因是误用了不存在的脚本名 `prepare_inputs.py`；经文件列表核对，真实文件为 `prepare.py`，raw/157 已读取。该技术性路径错误未执行任何安装或构建，不改变此前结果。

原始 ARM 根的已安装 LLVM Release 是 115.1；发生文件冲突的旧私有根则为 1。这是静态核查所得身份差异，尚不证明换一份独立副本可成功安装。准备对原始 RPM 数据库的独立副本执行正常 `rpm --test`，不用 `--replacefiles`、不安装、不修改原根；结果出来前不据此解除 ARM libc++ 门禁。

raw/181 的监控命令返回 1，是 `test -f .../exitcode` 当时未找到尚未生成的退出码文件；不是 rpmbuild 退出 1。随后 raw/130、raw/183 已记录整轮退出 0，三份 RPM 全部写出。

公开状态说明中，Source1002 的 tar 摘要与 tar 内二进制摘要严格分开。原 aarch64 二进制的 `_ZNSt7__cxx11` 精确前缀计数为零，但扩展检索 `St7__cxx11` / `std::__cxx11` 有命中；不能把精确前缀零命中写成标准库身份结论。
