# W1：bpftrace 源码适配推送

## 结果

bpftrace 提交 `f895f8c0373d224847fc7d3ecbeaac3bf926a1a1` 已普通快进推送到既有
`sandbox/lhmax2025/libcxx-migration`。父提交为
`30e51cd665360f85b665308dc13ba27df0c5f739`。仅 spec 新增 15 行，内容与此前
四格所验证候选的 SHA256 `f471f98b0d48dd769de286fbeaa8d7292230fbf316c95b06e03e2b631a86cdd3`
完全一致，没有新增代码或重编 static 资产。

全部 17 条远端分支对照：只目标分支快进，其他 16 条逐项未变，无新增分支、
无正式分支推送、无 force。远端给出标题超过 50 字符的 warning，但推送退出 0，
随后 ls-remote 核对成功。未因格式 warning 改写已推提交。

按本轮人工明确批准的门禁，**源码四格通过，static 单列**。11 包状态因此为
11/11 源码适配已推送，不等于 static 替代工作或所有可选功能均已完成。
本轮没有重跑已完成构建，没有使用开发板。

## 四格证据沿用与重新核对

复用 `docs/progress/BUILD_W2C_0915/bpftrace_source_matrix.json` 和对应四份
完整构建日志/最终 RPM 检查记录。本轮重新校验当前 spec 与受测候选相同，
检查四格集合完整、程序与提供方标准库结果、两个 GCC 完整日志禁止选项为 0。
原始记录不修改；本轮复核在 `release_gate.json`、raw/006。

| 架构 | libc++ | GCC |
| --- | --- | --- |
| armv7l | Clang 22.1.8，完整构建；bpftrace/aotrt 及 LLVM/BCC 提供方均 libc++ | GNU 14.2.0，完整构建；源码程序 libstdc++，禁止参数 0 |
| aarch64 | 同上，已验证 | 同上，已验证 |

x86_64 不在当前包的 ExclusiveArch，不伪列六格。
static 子包四格内仍是原资产，内嵌标准库 `NOT_OBSERVED`；镜像范围、覆盖主程序
的副作用与资产身份详见 `KNOWN_ITEMS.md`，已写入英文正文 + 中文说明的提交消息。

## 11 个 spec 的兼容性交叉核对

逐个通过 git show 读取既有已验证提交的 spec；bpftrace 使用同一受测候选。
各文件全文、SHA256、所引 revision 与选项条件栈见 `spec_audit/`、
`release_gate.json`，执行命令及退出码见 `audit_commands.jsonl`。
复用旧纯扫描函数但不执行旧脚本的写文件主程序；以 4 个正反样本重新校准。

11 包均按以下共同条件定义开关：

```spec
%if %{defined _toolchain}
%if %{toolchain_is clang}
%define build_with_libcxx 1
%endif
%endif
```

新增的字面 libc++ 编译/链接选项和依赖都在 `%if 0%{?build_with_libcxx}`
正分支内。tensorflow2 另要求 USE_CLANG=ON；Boost 在条件外保存/读回参数，
但不会在 GCC 路径自行生成 libc++ 选项。没有发现本轮这些新增选项从条件中泄漏。

| 包 | 复核结果与沿用实测边界 |
| --- | --- |
| abseil-cpp | 条件内开发依赖与 flags；沿用三架构 GCC 实测 |
| boost | 条件内 flags，构建和安装共享同一组参数；沿用三架构及子包一致性实测 |
| icu | 条件内依赖与 flags；沿用三架构实测 |
| jsoncpp | 条件内依赖与 flags；沿用三架构实测 |
| libsigc++ | 条件内 flags，交给 Meson；libc++ 依据实际命令和 std::__1 痕迹，不强留动态依赖 |
| pcre | flags 及 devel Requires 同条件；沿用三架构实测 |
| taglib | 条件内依赖与 flags；沿用三架构实测 |
| tensorflow2 | 条件与 USE_CLANG 双重控制；armv7l GCC 按与基线同点同因失败判据，不称完整成功 |
| llvm | 条件内 LLVM_ENABLE_LIBCXX、AOT 资产选择与 libc++abi 选项；沿用十二格验证 |
| bcc-tools | 条件内新增依赖与 flags；沿用六格验证与提供方检查 |
| bpftrace | 条件内新增 15 行；四格源码程序与提供方验证通过，static 单列 |

这是固定提交中新增字面选项的条件复核，结合各包已有真实构建证据；
不是任意 RPM 宏求值器，也不保证能过滤外部人为注入的 CXXFLAGS/LDFLAGS。
本轮没有把“构建根安装了 libc++ 开发包”误当成“GCC 使用了 libc++”。

## 纪律与后续

开工 21:55 +08，推送约 22:01 +08，未超两小时。资源方式和数值见 STATUS.md。
`codes/` 仓库保持原样，提交操作在 `tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push`。
包候选的实际 diff 在 raw/012；原始推送/远端检查在 raw/016–017。
本任务只授权发布既有源码适配；W2 继续评估从源码构建 static，W3 即使成功也不推。

自行判断：将“未进产品镜像”严格限定为已核对的六份清单，将“不是上游发布”
限定为与当前 v0.24.2 Release 不匹配，避免把已有证据放大。未自行更改其余十包
结论、静态资产验收或任何运行时线工作。
