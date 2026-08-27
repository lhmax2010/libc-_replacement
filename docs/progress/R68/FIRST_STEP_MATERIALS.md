# R68 第一步材料说明

本提交只归档能力探测阶段；未启动第二步全量运行。

## 主要材料

- `STEP1_ARMV7L_CAPABILITY.md`：armv7l 能力探测结论、计数、耗时与空间估算。
- `code/`：配置、探测、ARM 交叉编译、SDB 执行、抽样、汇总和板端清理脚本。
- `commands/`：第一步命令、退出码和构建记录。
- `raw/capability/`：x86_64 与 armv7l、打补丁与未打补丁配置的能力探测原始输出。
- `raw/capability/summary/`：armv7l 计数以及逐项 `UNSUPPORTED` 原因。
- `raw/capability/patched/armv7l/sample_actual/`：UTF-8 修正前的原始抽样结果。
- `raw/capability/patched/armv7l/sample_actual_after_decode_fix/`：修正后的同分片结果；`failures/` 下另存 4 个失败项的完整输出。
- `commands/075_board_step1_cleanup.log` 与 `raw/board_step1_post_cleanup.txt`：板端清理前清单、删除命令及清理后核验。
- `INDEX.tsv`：全部归档文件的来源、大小和 SHA256。

## 执行器版本身份

`code/SDB_EXECUTOR_VERSIONS.md` 明确记录修改前版本的身份：它由当前完整源码逆向应用唯一一行已记录补丁而确定性恢复，不冒充当时另存的原始文件。修正前的实际行为由原始抽样输出直接佐证。

## 尚未进行

- 未运行 x86_64/armv7l × 打补丁/未打补丁的四格全量测试。
- 抽样的 4 个失败尚未取得两组合对照，未作性质分类。
- 第二步将使用 `/opt/usr` 下的工作目录、按套件分批记录进度以支持断点续跑，并为这 4 个失败取得打补丁/未打补丁对照。
