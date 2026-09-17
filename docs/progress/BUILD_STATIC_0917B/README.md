# 原资产条件性补测：PARTIAL

**2026-09-17 续跑：** [LLVM aarch64 完整 RPM 与载荷核验已完成](resume_0917/AARCH64_RPM_RESULT.md)：最终完整 rpmbuild 于 14:17 返回 0，12 包摘要检查通过；RPM 内 libclang.a 有 libc++ 符号痕迹，libLLVM 依赖 libc++、不依赖 libstdc++。记录器重复执行事故、隔离与重写包过程完整保留在 [当前状态](resume_0917/STATUS.md) 和 `aarch64_complete_1417/`。bpftrace 两架构三路径 RPM 与板上验证仍未完成；没有包仓推送。

以下正文及 [FINAL_RESULT.md](FINAL_RESULT.md) 是上轮结束记录，保留历史，不代表 09-17 的最新进度。当前固定截止为 09-17 22:48:19 +08，不因技术性重跑顺延。

昨日未上传的增量证据现已补齐 269 个文件，见 `NATIVE_FINAL_MANIFEST.json`。日志文件经过上一轮运行脚本轮转；原检查点提交中的同名构建日志另存为 `llvm-aarch64-native-rpm.archived-at-8e5b4fc1.build.log`，保留其原文，不混成同一次执行。

## 本轮状态

人工已确认：完整 RPM 仅 armv7l/aarch64；x86_64 只做独立候选和取消探针，不扩 ExclusiveArch、不要求 x86_64 原资产回退。原资产实际链接参数不能恢复，探针仅作假设形态的条件性验证。

| 工作 | 结果 | 尚缺内容 |
| --- | --- | --- |
| W1 原资产补测 | 条件性对照完成 | 原资产本身的取消行为仍 NOT_OBSERVED |
| W2 两架构三路径完整 RPM | PARTIAL；五对象等价抽查通过；三轮 I/O 等待后仍超时，21:57:02 已停止并保留断点 | libclang 完整开发 RPM、bpftrace 六格 RPM 与安装验收未完成 |
| W3 armv7l 物理板 | 只读连通/预检完成，正式验证 NOT_STARTED | 等 W2 新 RPM 和相应 armv7l 静态输入/探针 |

不能批准候选替换发布资产。未把以前的独立目标构建、手工安装或已有 aarch64 运行时 RPM 等同于本轮完整六格 RPM 和安装验证。

最新续跑结果见 [原生工具集核查与结束记录](NATIVE_TOOLCHAIN_REPORT.md)。原生抽查五份对象的符号表、反汇编和整体 SHA256 均一致；**主构建没有新增完成目标**，不得将抽查通过写成完整 aarch64 LLVM 已由原生工具集构建完成。构建停止早于原截止；此后只整理材料，没有继续构建、安装或使用板子。

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

## W2：续跑后的最新状态

**本次新增确认：** 人工已批准原生交叉工具集。两套 Clang 身份、五个对象 nm/反汇编与完整 SHA256 相同的证据见 [原生工具集核查](NATIVE_TOOLCHAIN_REPORT.md)。这只支持抽查范围的等价性，不是每个 LLVM 目标的证明。完整续建资源改为实际 systemd user scope，MemoryMax=16536449024 字节（物理内存 50% 向下页对齐），实际 Ninja 的证明记录在事件日志。下文 19:58 停止是历史断点，不再表示等待授权。

人工补齐并修正凭据后，第一次完整复制误带入历史 GCC 树，且 sudo 重置了地址空间限制；经人工批准停止并缩小范围重做。最新复制约 1 GiB，小于 3 GiB；复制与 chown 退出 0，立即 shred 密码文件并确认不存在。全部四次 sudo 调用和退出码、逐目录体积、实际限制见 [复制续跑记录](COPY_RESUME_REPORT.md)。不再追加 sudo。

当前私有根为 `tmp/STATIC_0917B/llvm-root-minimal`。依赖安装预检和安装成功，实测 Clang 22.1.8。aarch64 的完整 `rpmbuild -bb` 首次因 namespace 的归档所有权恢复失败，改为普通用户构建后通过 `%prep`，进入 CMake；C/C++ 编译器身份与 libc++ 编译/链接探测通过。完整输出在 `llvm-aarch64-full-rpm-retry.build.log`，截止时间由 raw/037 起固定四小时，即 2026-09-16 22:09:25 +08。

**最新停止原因不是超时，也不是编译失败。** 19:58 主动保存断点等待人工确认是否复用既有的宿主原生加速工具集。CMake 完成约 416 秒，主构建到 181/7546；现路径的编译器经 QEMU aarch64 执行。只读实测另一个已验证根的原生 Clang 也是 22.1.8、目标 aarch64，且使用当前根的 libc++ 头文件路径；该根 `emul` 为 515 MiB。这个探测只证明版本与 driver 选择，不证明切换后的完整构建已通过。

停止仅针对核对过的本任务普通用户进程组，以 SIGINT 保存 Ninja 已完成产物。监控记录 -2，外层记录器退出 254；不是新发现的编译错误。没有新增 sudo。实际命令、进程身份和结束核验见 raw/130–132。该次等待已由人工批准解除；资源上限、原截止均未自行放宽。

宏展开核查：armv7l/aarch64 各 Clang、GCC、未定义三路径均检查；GCC/未定义路径与原 spec 展开文本（忽略空行）一致，新增 shared-libgcc 仅在 libc++ static 分支。`spec_path_checks.json` 为结果。**这不是六格完整构建通过。**

交付核查索引：`SELF_CHECK.json` / `MANIFEST.json` 是首次、凭据尚未补齐时的历史记录；本次续跑以 `RESUME_SELF_CHECK.json` / `RESUME_MANIFEST.json` 为准。密码未打印或复制，已立即销毁；敏感模式与路径检查不冒充对已删除、且未保留的密码做精确字符串比对。

### 历史阻断（首次交付时，现已解除）

raw/005 实际 `stat password.md` 报 No such file or directory，退出 1；raw/014 再次 `test -e password.md` 为 1。检查的是工作区 `/home/toolchain/development/libc++_replacement` 根目录，未到其他目录搜寻凭据。已发出补充输入请求。

首次交付时 sudo 为 0 次；本次续跑已执行四次授权调用并完成密码删除，以上最新记录取代旧状态。原始失败记录保留，不能把早期“凭据不存在”继续当作当前阻断。

确认可续用的准备记录为 `../BUILD_STATIC_0916B/prepare_llvm_root.py`：源根 `tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0`；旧 `tmp/NIGHT_0917/finish_validation/llvm-root/` 是缺文件的副本，不可当作完成根。后续拟在本任务 tmp 下建立新副本，复制/chown 完成后立即按授权删除密码文件。未经执行，不预填复制或校验成功。

本轮记录的现有 builds 目录只含 x86_64/aarch64 的 runtime/libclang/static 目标目录，没有 armv7l 同类目录；不由此断言整个工作区都没有 arm 输入。尚未进行全工作区产物归属核验或工期估算。

## W3：物理板只读预检

SDB 列出 `192.168.108.26:26101 device rpi4`；实际 uname 为 armv7l，内核 `6.12.80-arm-rpi4-v7l`。连接使用 owner(uid 5001)，不是 root。进程列表已保存；目标 `/var/tmp/build_static_0917b` 不存在。包名筛选只返回 libllvm-22.1.8-2.1.armv7l，没有观测到 bpftrace 安装记录。指定临时路径的通配检查无输出不等于穷尽所有遗留文件。

板上 root 操作清单：**空（0 次）**。本轮未创建板上文件、未传输/安装/卸载软件，未启动取消探针或 BPF 脚本，因此没有需要撤销的测试变更。内核 BPF 配置、root 功能对照、20 次共享展开器探针与静态对照均为 NOT_OBSERVED。未占用板子供后续独占使用。

## 资源、技术性修正与自行判断

- medium 门禁实测 PASS（raw/004），并行探针 1；nice 19、ionice 3；ulimit -v=16148878 KiB，为机器总内存 50%，限制虚拟地址空间，不冒称 cgroup RSS 限制。
- 首次交付时未启动大构建；本次续跑已启用每 500 目标 I/O 监控与固定截止。复制前最终 du 在授权辅助进程内完整成功，不以最早的权限不足部分结果代替。
- raw/006 检索包含不存在的 ADAPTOR_STATUS 文件，退出 2；raw/009 去掉不存在路径后重查成功。后者误包含历史大体积 strings 记录，未作新证据使用，归档压缩保留，不据其噪声作判断。
- 自行判断仅限探针条件性解释、保持未授权架构与原资产不变、凭据缺失时不调用 sudo。没有改变原有判定、没有声明缺失验证已通过。
- 凭据与复制阻断已经解除，不再需要人工提供密码。后续仅用当前用户拥有的候选根继续；是否完成以最终 RPM 和板上证据为准。
