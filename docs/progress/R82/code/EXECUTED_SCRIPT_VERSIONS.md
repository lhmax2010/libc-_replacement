# R82 执行脚本版本说明

`raw/034_run_arm_local_matrix.command.txt` 执行时，
`progress/R82/code/run_arm_local_matrix.sh` 只循环 `filesystem` 与
`iostreams` 两个用例。随后为了加入第三个 `program_options` 用例，同一路径
的脚本被修改；归档时只保存了修改后的三用例版本。因此：

- 当前 `run_arm_local_matrix.sh` 是最终三用例脚本；
- `run_arm_local_matrix.executed-034.reconstructed.sh` 是依据当时输出、路径配置
  和后续增量修改重建的执行等价两用例版本；
- 当时没有随命令保存脚本 SHA256 或逐字节快照，所以不能声称重建文件与当时
  文件逐字节相同；
- `raw/034_run_arm_local_matrix.stdout` 只出现两类 `CASE=`，与重建脚本的执行
  集合相符；
- 直接用当前三用例脚本重跑，不会得到与 raw/034 相同的执行集合。

以后每次执行脚本型命令时，应在启动前把脚本复制为不可变的
`raw/<命令编号>.<脚本名>.snapshot`，同时记录其 SHA256；命令应引用该快照，
而不是引用随后仍会修改的工作副本。若脚本调用其他本地辅助文件，也应保存
依赖清单及各文件 SHA256。
