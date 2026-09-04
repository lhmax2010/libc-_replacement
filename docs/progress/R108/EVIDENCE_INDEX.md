# R108 证据索引

- 资源、工作区与分支切换：`raw/001_*`、`raw/002_*`、`raw/004_*`。
- R105 原结果提取：`raw/003_*`、`raw/008_*`。
- clean baseline worktree、源码/夹具身份：`raw/005_*`--`raw/007_*`、
  `raw/009_*`、`raw/012_*`、`raw/013_*`。
- 完整基线构建：`raw/015_build_tensorflow2_armv7l_gcc_baseline.*`。
- 失败机械比对：`raw/016_*`、`artifacts/failure_compare/`。
- 编译器和修改源文件未到达：`raw/018_*`。
- spec 的 Clang/GCC/XNNPACK 分支原文：
  `artifacts/spec_toolchain_xnnpack_excerpt.txt`、`raw/021_*`。
- 调整后五个既有成功格与 armv7l 非回归门禁：`raw/022_*`、
  `ADJUSTED_MATRIX.tsv`。
- 候选完整 diff、提交、普通推送和远端分支核验：
  `tensorflow2_candidate.diff`、`raw/023_*`--`raw/027_*`、
  `tensorflow_remote_*.tsv`、`verify_tensorflow_remote.sh`。
- 可比性机器表：`COMPARABILITY.tsv`。

`raw/020_*` 因只读检索的输出管道被 `head` 提前关闭，没有包装器 exitcode；
恢复说明和替代的有界取证位于 `raw/021_*`。所有裁决所依赖的命令均有独立
exitcode。
