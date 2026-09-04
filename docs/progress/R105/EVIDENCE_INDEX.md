# 证据索引

- 资源与初始状态：`raw/001_*`、`raw/002_*`、`raw/080_*`、`raw/095_*`、`raw/103_*`。
- libsigc++ 判据复核：`raw/018_*`–`raw/020_*`；提交、推送与核对：`raw/050_*`–`raw/053_*`。
- Boost 根因与最终改法：`raw/023_*`–`raw/032_*`；六格构建/核验：
  `raw/033_*`–`raw/075_*`；提交、推送与核对：`raw/076_*`–`raw/079_*`。
- TensorFlow 原错误、声明对照与最小探针：`raw/004_*`、`raw/005_*`、
  `raw/013_*`–`raw/017_*`。
- TensorFlow libc++ 三格：armv7l `raw/081_*`、`raw/085_*`–`raw/088_*`；
  aarch64 `raw/089_*`、`raw/092_*`–`raw/094_*`；x86_64 `raw/098_*`、
  `raw/101_*`–`raw/102_*`。
- TensorFlow 真实 GCC 夹具与结果：夹具 diff `raw/100_*`；armv7l 失败
  `raw/104_*`；aarch64 `raw/113_*`–`raw/115_*`；x86_64
  `raw/116_*`–`raw/118_*`。
- TensorFlow 未推送状态核对：`raw/119_*`；最终本地状态与完整改动 diff：
  `raw/120_*`、`raw/121_*`。
- R103 精确归档成员输出：`artifacts/tensorflow2_*_r103_matches/`。

所有 `*.command.txt` 均有对应 stdout、stderr 与 exitcode。`023`、`028` 的
exitcode 是紧随其后的恢复记录，旁边的 `*.exitcode.provenance.txt` 明确标注，
不得误认为日志包装器自动产生。
