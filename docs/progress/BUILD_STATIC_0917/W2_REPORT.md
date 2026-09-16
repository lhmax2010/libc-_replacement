# 原预编译资产与实际调用点

## 原资产：取消是否崩溃仍为 NOT_OBSERVED

先明确哈希对象，避免沿用历史转述混淆：

| 对象 | SHA256 |
| --- | --- |
| Source1002 **tar.gz 本身** | `3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123` |
| tar 内 aarch64/usr/bin/bpftrace | `4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1` |
| tar 内 arm/usr/bin/bpftrace | `ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b` |

`original-tar-sha256.json`、`original-tar-members.json` 与 `original_summary.json`
分别给出取证命令与结果。tar 的目录叫 `arm`，不是 `armv7l`；x86_64 原资产不存在。

| 检查 | aarch64 | armv7l |
| --- | --- | --- |
| DT_NEEDED | libm.so.6、libc.so.6、ld-linux-aarch64.so.1 | libm.so.6、libc.so.6、ld-linux.so.3 |
| 普通符号表 | nm 报 no symbols | nm 报 no symbols |
| `_Unwind_ForcedUnwind` / `_Unwind_SetGR` 名称 | 未观测 | 未观测 |
| personality 字符串 | `__gcc_personality_v0`、`__gxx_personality_v0` | 同左 |
| 上述 personality 为已定义 ELF 符号 | 未观测 | 未观测 |
| libgcc_s 字符串 | 未观测 | 未观测 |
| GNU/LLVM 标准库命名空间字符串 | 两种查询均未观测 | 两种查询均未观测 |

字符串可能是内嵌编译器用于生成其他程序的名称，不能据名称单独断定函数已链入。
因此本轮仍**不能确认原资产内嵌的是 libgcc_eh、LLVM libunwind 还是其他处理方式**。
没有把“无 libgcc_s 的直接动态依赖”当成“取消时不会 dlopen libgcc_s”。

两个程序均已 strip，缺 `_Unwind_SetGR` / 私有寄存器表的可定位符号；没有原始
链接命令、link map、匹配调试文件，也没有能在原 bpftrace 里触发取消的受控入口。
故未进行一个无法可靠定位断点的 GDB 实验，更没有伪造“与原资产实际参数相同”的
探针。W1 是**我们候选配置**的直接证据，不是原资产的直接证据。
要补齐原资产结论，需要原链接 map/未剥离产物和可触发取消的执行路径；仅再跑
--version 或从其他架构类推都不足够。未在旧资产里注入代码或修改二进制。

## bpftrace / BCC / LLVM 是否使用取消

同一明文匹配 `pthread_cancel|pthread_setcanceltype`，排除 `.git` 与补丁文本，
扫描已展开源码；LLVM 内真实调用作为阳性对照（`search-positive-control.json`）。

| 输入 | 实际结果 | 可得结论 |
| --- | --- | --- |
| bpftrace f895f8c0373d224847fc7d3ecbeaac3bf926a1a1 | 0 个文本命中 | 已扫源树未观测到这两个名称的直接使用 |
| BCC 5bd0654e24bb231035ea684d278f674aa95a2cd2 | 0 个文本命中 | 同上，不以包名推断 |
| LLVM f203923a1508c9344f5fc6b17bd8822f011655c4 全树 | 有命中 | 不能说整个 LLVM 项目不用取消 |

生产代码中的代表命中与条件：

```cpp
// openmp/runtime/src/z_Linux_util.cpp:449–451
#ifdef KMP_CANCEL_THREADS
  KA_TRACE(10, ("__kmp_terminate_thread: kill (%d)\n", gtid));
  status = pthread_cancel(th->th.th_info.ds.ds_thread);
```

该文件还在 562、640 行调用 pthread_setcanceltype。另有 LLDB：

```cpp
// lldb/source/Host/posix/HostThreadPosix.cpp:43–46
#ifndef __FreeBSD__
    llvm_unreachable("someone is calling HostThread::Cancel()");
#else
    int err = ::pthread_cancel(m_thread);
```

还存在 MacOSX debugserver、sanitizer 拦截器、测试、文档等命中，均保存在
`llvm-cancel-search.json`。没有把 FreeBSD/Mac 条件下的代码算成当前 Linux 实际调用。

本地 x86_64/aarch64 旧候选的 link map 对 libomp/liblldb 及两个取消名称检索为零，
未定义符号表也未观测到两函数。本轮新 x86_64 候选同样没有两函数导入。
**已覆盖的候选直接调用/链接记录中，没有证据显示其实际触发本次取消崩溃。**
这是范围受限的否定证据，不等于证明所有第三方依赖、动态加载或将来使用方式均安全；
函数指针、其他名称/包装、生成代码及运行时加载不由本次字符串检索穷尽。
也不能把本任务的取消探针失败描述为已观察到产品 bpftrace 功能崩溃。
