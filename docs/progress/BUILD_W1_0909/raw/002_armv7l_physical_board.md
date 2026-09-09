# armv7l 物理板 MLGO 运行

载体：`192.168.108.26:26101`，设备报告架构 `armv7l`；本格明确为**物理板**。

只创建本任务目录 `/opt/usr/build_w1_0909_mlgo`。运行时线的既有目录与诊断包未修改。

执行的核心命令：

```bash
LD_LIBRARY_PATH=/opt/usr/build_w1_0909_mlgo:/usr/lib:/lib \
  /opt/usr/build_w1_0909_mlgo/verify_mlgo_release_mode.sh \
  /opt/usr/build_w1_0909_mlgo/opt armv7l-tizen-linux-gnueabi \
  /opt/usr/build_w1_0909_mlgo/mlgo_inliner_input.ll \
  /opt/usr/build_w1_0909_mlgo/result
```

核心输出：

```text
default_rc=0
release_rc=0
Pass: inline-ml count=6
ShouldInline count=6
AOT_MODEL_EXECUTION_PASS
armv7l_physical_board_mlgo_exit=0
```

release 路径在脚本内执行 `opt -passes=verify -disable-output` 并返回 0，故输出 IR 通过 verifier。YAML 中同时出现 `Pass: inline-ml` 与 `ShouldInline`，证明 release-mode AOT advisor 实际产生决策，而非只运行默认启发式。

板镜像没有 `cmp`，脚本第 41 行辅助比较产生 `cmp: command not found`，但该命令位于 `if` 条件内，没有改变上述 AOT/IR 判据及脚本最终退出码。结果拉回主机后用正确路径执行：

```bash
cmp -s tmp/BUILD_W1_0909/armv7l_physical_board/default.ll \
  tmp/BUILD_W1_0909/armv7l_physical_board/release.ll
```

退出码：`0`，两份 IR 相同。此结果只描述最终 IR；YAML 的 6 条决策记录仍直接证明 AOT advisor 已运行。

证据 SHA256：

```text
fa958a89084f0859d64195168c1e78fd698ccecb70ad8417cb70ce47a1a3d577  default.ll
fa958a89084f0859d64195168c1e78fd698ccecb70ad8417cb70ce47a1a3d577  release.ll
3c1ff81406c67da240777b722ef88e06543ca70ca11b09da5c508f135b851ded  release.remarks.yaml
```

使用的脚本快照：`docs/progress/BUILD_W1_0908/code/verify_mlgo_release_mode.sh`，SHA256 `839604338a798455f227623219e9e1d2227453e16b4aea049c8a9bb0ff8eac48`。

收尾命令只删除本任务自建目录：

```bash
rm -rf /opt/usr/build_w1_0909_mlgo
test ! -e /opt/usr/build_w1_0909_mlgo
```

退出码：`0`。清理后的 `/opt/usr` 顶层清单与开工前一致；**板子已释放**。
