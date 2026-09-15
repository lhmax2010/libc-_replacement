# 单列的资产与基线事项

本表不改变已有结论，不将“源码包已推送”写成所有附带资产均已重新验证。

| 项目 | 当前事实与状态 | 材料 |
| --- | --- | --- |
| tensorflow2 / XNNPACK armv7l GCC | 原基线与候选在同点同因的 vsdot.s8 汇编能力检测失败；按人工批准的相对基线无回归判据交付。不是该格完整构建成功。 | docs/progress/R108/ |
| LLVM / MLGO XLA AOT | 曾因预编译资产标准库 ABI 不同阻断；TF 2.18 双标准库资产及消费适配已完成，LLVM f203923a1508c9344f5fc6b17bd8822f011655c4 十二格闭合。保留资产版本、标准库与生成机制的追踪，不将此项写成仍阻断当前 LLVM。 | docs/progress/BUILD_W1B_0909/；docs/progress/BUILD_W2C_0915/FINAL_REPORT.md |
| bpftrace-static | 来源未确认；当前上游 v0.24.2 Release 资产均不匹配。源码四格通过后，本轮人工批准 static 单列推送；自行构建替代仍待评估与验证。 | docs/progress/BUILD_W2C_0915/static_origin/；docs/progress/BUILD_W2C_0915/upstream_release/ |

## bpftrace-static 的精确身份

| 对象 | SHA256 |
| --- | --- |
| Source1002：prebuilt-static-bpftrace.tar.gz | 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123 |
| arm/usr/bin/bpftrace | ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b |
| aarch64/usr/bin/bpftrace | 4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1 |

两者实际 `--version` 均为 `bpftrace v0.24.2`，此前在 QEMU 用户态查询退出 0。
strings 均含 `Clang 21.1.1` 与 `LLVM version 21.1.1`；这是嵌入工具代码线索，
不是生产该资产的编译器身份证明。两者已经 strip，内嵌 C++ 标准库为
`NOT_OBSERVED`，不能以动态依赖只剩 C 库推断没有 C++ 运行时代码。

候选 `packaging/bpftrace.spec:99–108` 原有 static 安装/卸载逻辑包含：

```spec
install -m 0755 prebuilt-static-bpftrace/%{_arch}%{_bindir}/bpftrace %{buildroot}%{_bindir}/bpftrace-static

%post static
cp -rf %{_bindir}/%{name}-static %{_bindir}/%{name}
rm -rf %{_bindir}/%{name}-static

%postun static
rm -rf %{_bindir}/%{name}
```

因此安装 static 子包后 `/usr/bin/bpftrace` 会被该预编译程序覆盖，不能用
源码主包通过验证来证明安装 static 后仍运行本次编出的程序。以上为源码
核查，本轮没有执行这些安装/卸载命令。

镜像范围固定为 `Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247`
的六份清单（boot/headed/headless × armv7l/aarch64），4399 行合计中无
bpftrace-static 条目。**不排除测试加装、其他镜像或后续快照。**

上游核对范围为 v0.24.2 当前五个上传资产中的三个含程序下载项，均是同一个
x86_64 AppImage；不能将这项结果扩展为所有历史 Actions/private build 均已排除。
其余两个上传项为工具脚本与 manpage。来源未知不等于已经确定为 Tizen 自行编译。
