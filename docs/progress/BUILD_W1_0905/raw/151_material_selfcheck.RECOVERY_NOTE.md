# 命令自身的技术性错误

本次自检命令把 Markdown 反引号放在了 `bash -c` 的双引号参数中，导致 shell
把 `PARTIAL` 与 `NOT_AVAILABLE` 当作命令替换执行。虽然两个 `grep` 因而退化
成空字符串匹配并最终返回 0，但这一轮不能作为状态文本检查的有效证据。

未修改本轮原始输出；后续 `152_material_selfcheck_corrected` 去掉反引号后重新
执行相同判据，其结果才作为自检依据。
