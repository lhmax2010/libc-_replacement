# 原资产条件性补测：PARTIAL

## 本轮状态

人工已确认：完整 RPM 仅 armv7l/aarch64；x86_64 只做独立候选和取消探针，不扩 ExclusiveArch、不要求 x86_64 原资产回退。原资产实际链接参数不能恢复，探针仅作假设形态的条件性验证。

| 工作 | 结果 | 尚缺内容 |
| --- | --- | --- |
| W1 原资产补测 | 条件性对照完成 | 原资产本身的取消行为仍 NOT_OBSERVED |
| W2 两架构三路径完整 RPM | BLOCKED，未启动构建 | 工作区根目录 password.md 实际不存在，授权复制步骤未执行 |
| W3 armv7l 物理板 | 只读连通/预检完成，正式验证 NOT_STARTED | 等 W2 新 RPM 和相应 armv7l 静态输入/探针 |

不能批准候选替换发布资产。未把以前的独立目标构建、手工安装或已有 aarch64 运行时 RPM 等同于本轮完整六格 RPM 和安装验证。

## W1：实测和结论的界限

Source1002 只有 arm 与 aarch64 两个文件，tar 和二进制 SHA256 见 STATUS.md 与 raw/002、003。两份原资产均不直接依赖 libgcc_s.so.1，普通符号表已去除；动态符号中未匹配到 `_Unwind_ForcedUnwind`、`_Unwind_SetGR`、`_Unwind_GetGR` 的定义。strings 有 `_Unwind_GetGR`，不能由此认定具体实现或静态归档来源。**原资产实际是否内嵌 libgcc_eh、是否触发取消崩溃，仍未观测。**

按人工允许的“最接近的推测形态”，用既有 condition_cancel_probe.cpp 在 x86_64 Tizen 根内实际重新编译并运行。使用 Clang 驱动选择 libstdc++，动态 glibc，静态 libstdc++；两组只改变 `-static-libgcc` / `-shared-libgcc`。精确命令和输出在 w1-*-compile.json、w1-*-needed.json、逐次运行 JSON。

| 形态 | wait | system_clock | steady_clock | custom_clock | 总计 |
| --- | --- | --- | --- | --- | --- |
| 静态 libgcc | 5/5 退出 134 | 5/5 退出 134 | 5/5 退出 134 | 5/5 退出 134 | 20/20 退出 134 |
| 动态 libgcc_s | 5/5 断言通过 | 5/5 断言通过 | 5/5 断言通过 | 5/5 断言通过 | 20/20 通过 |

通过断言：确认已等待；清理执行；cancel_rc=0、join_rc=0、canceled=1、cleanup_count=1。静态组的 134 与前一轮已用 GDB 确认的 SIGABRT 结果相符；本轮没有重新获取回溯，不声称重做了机制归因。

**条件性结论：若原资产采用本次假设的静态 libgcc / 动态 glibc 配置，并经过相同的取消和符号绑定路径，则本次 x86_64 实验显示会崩溃。** 不写“原资产必崩”，不外推 armv7l 结果。探针还依赖 libatomic、libpthread 等，与原资产依赖清单并非逐项相同；用途是隔离展开器链接方式，不是重建原资产配方。

输入源的 SHA256 见 w1_probe_input.json；执行前脚本快照 SHA256 见 raw/007；四十次结果见 w1_matrix.json。

## bpftrace 是否使用取消

本轮重新核实 bpftrace f895f8c0373d224847fc7d3ecbeaac3bf926a1a1 和 BCC 5bd0654e24bb231035ea684d278f674aa95a2cd2：在排除 .git 和补丁文本的源树中，`pthread_cancel|pthread_setcanceltype` 明文检索均为零匹配（rg 退出 1）。这仅说明所扫源码未观测到这些名称的直接使用，不穷尽函数指针、动态加载或第三方间接调用。

LLVM 全树确有调用：本轮展示 `openmp/runtime/src/z_Linux_util.cpp:449–451` 的 `KMP_CANCEL_THREADS` 条件内调用作为阳性内容。此前 link map、导入符号的限定性检查及源码上下文见 `../BUILD_STATIC_0917/W2_REPORT.md` 与该目录原始 JSON；本轮未重做完整链接组件扫描。不把“bpftrace 没有直接文本命中”转述成产品绝对不受影响。

## W2：授权步骤尚无法执行

raw/005 实际 `stat password.md` 报 No such file or directory，退出 1；raw/014 再次 `test -e password.md` 为 1。检查的是工作区 `/home/toolchain/development/libc++_replacement` 根目录，未到其他目录搜寻凭据。已发出补充输入请求。

全部 sudo 调用清单：**空（0 次）**。没有读取、打印、复制密码；没有用其他权限路径绕过复制步骤。由于密码文件从未存在于可观测路径、复制步骤未执行，**没有调用 shred，不能宣称已销毁用户提供的密码**。

确认可续用的准备记录为 `../BUILD_STATIC_0916B/prepare_llvm_root.py`：源根 `tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0`；旧 `tmp/NIGHT_0917/finish_validation/llvm-root/` 是缺文件的副本，不可当作完成根。后续拟在本任务 tmp 下建立新副本，复制/chown 完成后立即按授权删除密码文件。未经执行，不预填复制或校验成功。

本轮记录的现有 builds 目录只含 x86_64/aarch64 的 runtime/libclang/static 目标目录，没有 armv7l 同类目录；不由此断言整个工作区都没有 arm 输入。尚未进行全工作区产物归属核验或工期估算。

## W3：物理板只读预检

SDB 列出 `192.168.108.26:26101 device rpi4`；实际 uname 为 armv7l，内核 `6.12.80-arm-rpi4-v7l`。连接使用 owner(uid 5001)，不是 root。进程列表已保存；目标 `/var/tmp/build_static_0917b` 不存在。包名筛选只返回 libllvm-22.1.8-2.1.armv7l，没有观测到 bpftrace 安装记录。指定临时路径的通配检查无输出不等于穷尽所有遗留文件。

板上 root 操作清单：**空（0 次）**。本轮未创建板上文件、未传输/安装/卸载软件，未启动取消探针或 BPF 脚本，因此没有需要撤销的测试变更。内核 BPF 配置、root 功能对照、20 次共享展开器探针与静态对照均为 NOT_OBSERVED。未占用板子供后续独占使用。

## 资源、技术性修正与自行判断

- medium 门禁实测 PASS（raw/004），并行探针 1；nice 19、ionice 3；ulimit -v=16148878 KiB，为机器总内存 50%，限制虚拟地址空间，不冒称 cgroup RSS 限制。
- 未启动大构建，因此未触发每 500 目标 I/O 暂停规则。只读 du 因 usr/share/upgrade 权限不足非零，不能拿它的部分结果当完整磁盘估算。
- raw/006 检索包含不存在的 ADAPTOR_STATUS 文件，退出 2；raw/009 去掉不存在路径后重查成功。后者误包含历史大体积 strings 记录，未作新证据使用，归档压缩保留，不据其噪声作判断。
- 自行判断仅限探针条件性解释、保持未授权架构与原资产不变、凭据缺失时不调用 sudo。没有改变原有判定、没有声明缺失验证已通过。
- 本轮不是因耗尽十小时时限停止，而是 W2 必需输入未到位。等待人工将密码文件放到指定工作区；不要通过聊天提供密码。
