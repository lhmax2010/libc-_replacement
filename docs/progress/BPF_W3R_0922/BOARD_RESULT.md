# 三、物理板可写区对照

工作目录/空间通过：`/var/tmp/bpf_static_0922/` → `/opt/var/tmp/bpf_static_0922/`，可写ext4；精确可用字节见board-test/WORK_DIRECTORY.json，超过500MiB门槛。板端没有cpio，使用已存在ARM构建根的cpio，仅传入本目录并记录源/目标SHA（board-test/transfers.json）；未装系统工具。以rpm2cpio管道解包到新extract/，开启pipefail，实际命令/返回值见commands.json。没有rpm安装、--nodeps、remount或修改MSM。

RPM SHA `672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756`；解出候选SHA `6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba`；原件SHA `ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b`，均实测相符。仅在工作目录中执行，TMPDIR/HOME/XDG_CACHE_HOME均指该目录；禁止core，单进程有20秒上限，未触发超时。

| 项 | 候选rc / 秒 | 原件rc / 秒 | 结果 | stdout/stderr（board-test/内） |
|---|---|---|---|---|
| version | 0 / 0.230 | 0 / 0.230 | 双方退出0 | version-candidate.out、version-candidate.err；version-original.out、version-original.err |
| info | 0 / 0.334 | 0 / 0.334 | 双方退出0 | info-candidate.out、info-candidate.err；info-original.out、info-original.err |
| list | 0 / 1.737 | 0 / 1.744 | 双方退出0 | list-candidate.out、list-candidate.err；list-original.out、list-original.err |
| begin-1 | 0 / 0.637 | 0 / 0.635 | 双方退出0 | begin-1-candidate.out、begin-1-candidate.err；begin-1-original.out、begin-1-original.err |
| begin-2 | 0 / 0.633 | 0 / 0.631 | 双方退出0 | begin-2-candidate.out、begin-2-candidate.err；begin-2-original.out、begin-2-original.err |
| begin-3 | 0 / 0.676 | 0 / 0.649 | 双方退出0 | begin-3-candidate.out、begin-3-candidate.err；begin-3-original.out、begin-3-original.err |
| kernel-1 | 0 / 0.932 | 0 / 0.831 | 双方退出0 | kernel-1-candidate.out、kernel-1-candidate.err；kernel-1-original.out、kernel-1-original.err |
| kernel-2 | 0 / 0.931 | 0 / 0.838 | 双方退出0 | kernel-2-candidate.out、kernel-2-candidate.err；kernel-2-original.out、kernel-2-original.err |
| kernel-3 | 0 / 0.839 | 0 / 0.859 | 双方退出0 | kernel-3-candidate.out、kernel-3-candidate.err；kernel-3-original.out、kernel-3-original.err |
| interval-1 | 0 / 1.632 | 0 / 1.634 | 双方退出0 | interval-1-candidate.out、interval-1-candidate.err；interval-1-original.out、interval-1-original.err |
| interval-2 | 0 / 1.635 | 0 / 1.634 | 双方退出0 | interval-2-candidate.out、interval-2-candidate.err；interval-2-original.out、interval-2-original.err |
| interval-3 | 0 / 1.670 | 0 / 1.647 | 双方退出0 | interval-3-candidate.out、interval-3-candidate.err；interval-3-original.out、interval-3-original.err |

耗时为宿主单调时钟包围一次远端调用测得，含SDB调用开销；不包含随后pull输出文件的时间。成对顺序均为候选→原件，参数完全相同，仅执行路径不同。

- --version：均为bpftrace v0.24.2。
- --info：逐行diff仅LLVM 21.1.1→22.1.8；build项与能力项其余行相同，详见info.diff。
- -l：完整输出均保留，选择实际候选列表中的 `tracepoint:sched:sched_switch`，脚本为 `tracepoint:sched:sched_switch { exit(); }`。前50行附后。
- BEGIN脚本各3次均输出ok；选中tracepoint脚本各3次、interval计时退出脚本各3次均退出0。只证明这三个脚本在这块板、此内核的覆盖，不声称所有探针和内置函数均测过。
- 没有候选独有失败，也没有双方失败而删项的情况。

## 取消探针：原样二进制、原断言

| 形态 | 执行数 | 原断言通过 | 退出码 | 结论 |
|---|---:|---:|---|---|
| shared-gcc | 20 | 20/20 | [0] | 本轮物理板实测 |
| static-gcc | 20 | 20/20 | [0] | 本轮物理板实测 |

四种wait模式各5次。逐次要求wait_confirmed、恰好一次cleanup、cancel_rc=0/join_rc=0/canceled=1/cleanup_count=1；stdout及判断结果见cancel.json。二进制身份沿用../BPF_W1_0921/W3_REFERENCE_INPUTS.json，推前推后SHA核对在transfers.json。

**静态libgcc对照也通过，不按此前x86_64结果预设ARM崩溃。** 因而不能把x86_64的20/20崩推广为所有架构必崩。本轮不追加机制归因，也不据此改回候选的共享libgcc选择。

## 收尾

所有实际测试通过后，已先记录工作目录每个普通文件SHA（preserved-shas.transport.out），再删除该目录文件和空目录。随后独立只读复查目录不存在（raw/020，TASK_DIRECTORY_ABSENT_RC=0）。这是用户允许的本任务目录清理，板上副本已删除；本地原RPM/ELF/探针仍保留，输出已拉回，可重新部署。

rpm查询仍无bpftrace安装；/usr/bin/bpftrace*仍ABSENT；本轮起止/proc/mounts全文逐字节一致；SDB root恢复装前关闭状态。见CLEANUP.json、MOUNTS_COMPARISON.json、ROOT_RESTORED.json。不宣称整个系统所有运行状态都未改变（BPF脚本本来会短暂挂接探针），确认的是未安装包、未改指定系统文件与挂载。

**rpm安装验证留待镜像阶段**：本轮绕开的是安装这个测试步骤，不是绕过RPM门禁执行安装；没有修改安全策略或只读属性。W4需据此前提审阅，不写“安装已通过”。

## -l前50行（完整清单在board-test/list-*.out）

### candidate

```text
hardware:backend-stalls:
hardware:branch-instructions:
hardware:branch-misses:
hardware:branches:
hardware:bus-cycles:
hardware:cache-misses:
hardware:cache-references:
hardware:cpu-cycles:
hardware:cycles:
hardware:frontend-stalls:
hardware:instructions:
hardware:ref-cycles:
iter:bpf_link
iter:bpf_map
iter:bpf_map_elem
iter:bpf_prog
iter:bpf_sk_storage_map
iter:cgroup
iter:ipv6_route
iter:ksym
iter:netlink
iter:sockmap
iter:task
iter:task_file
iter:task_vma
iter:tcp
iter:udp
iter:unix
kprobe:ERR_getErrorString
kprobe:FSE_NCountWriteBound
kprobe:FSE_buildCTable_raw
kprobe:FSE_buildCTable_rle
kprobe:FSE_buildCTable_wksp
kprobe:FSE_buildDTable_internal
kprobe:FSE_buildDTable_raw
kprobe:FSE_buildDTable_rle
kprobe:FSE_buildDTable_wksp
kprobe:FSE_compressBound
kprobe:FSE_compress_usingCTable
kprobe:FSE_compress_usingCTable_generic
kprobe:FSE_createCTable
kprobe:FSE_createDTable
kprobe:FSE_decompress_usingDTable
kprobe:FSE_decompress_wksp
kprobe:FSE_decompress_wksp_bmi2
kprobe:FSE_freeCTable
kprobe:FSE_freeDTable
kprobe:FSE_getErrorName
kprobe:FSE_isError
kprobe:FSE_normalizeCount
```

### original

```text
hardware:backend-stalls:
hardware:branch-instructions:
hardware:branch-misses:
hardware:branches:
hardware:bus-cycles:
hardware:cache-misses:
hardware:cache-references:
hardware:cpu-cycles:
hardware:cycles:
hardware:frontend-stalls:
hardware:instructions:
hardware:ref-cycles:
iter:bpf_link
iter:bpf_map
iter:bpf_map_elem
iter:bpf_prog
iter:bpf_sk_storage_map
iter:cgroup
iter:ipv6_route
iter:ksym
iter:netlink
iter:sockmap
iter:task
iter:task_file
iter:task_vma
iter:tcp
iter:udp
iter:unix
kprobe:ERR_getErrorString
kprobe:FSE_NCountWriteBound
kprobe:FSE_buildCTable_raw
kprobe:FSE_buildCTable_rle
kprobe:FSE_buildCTable_wksp
kprobe:FSE_buildDTable_internal
kprobe:FSE_buildDTable_raw
kprobe:FSE_buildDTable_rle
kprobe:FSE_buildDTable_wksp
kprobe:FSE_compressBound
kprobe:FSE_compress_usingCTable
kprobe:FSE_compress_usingCTable_generic
kprobe:FSE_createCTable
kprobe:FSE_createDTable
kprobe:FSE_decompress_usingDTable
kprobe:FSE_decompress_wksp
kprobe:FSE_decompress_wksp_bmi2
kprobe:FSE_freeCTable
kprobe:FSE_freeDTable
kprobe:FSE_getErrorName
kprobe:FSE_isError
kprobe:FSE_normalizeCount
```
