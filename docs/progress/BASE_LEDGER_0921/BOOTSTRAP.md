# 开场命令与技术性非零说明

资源门禁 light 退出 0，原文在 raw/001_resource.*。时间上限两小时，初次时钟记录为 2026-09-21 20:42:09 +08:00，本轮未进行构建。

以下开场只读命令发生在日志器建立前，按实际工具返回补记命令与退出码：

```text
pwd
exit=0
/home/toolchain/development/libc++_replacement

rg --files -g AGENTS.md -g '*base_all_source_packages*' -g 'PACKAGE_STATUS.md' -g '*source*repo*' docs progress tools
exit=143（枚举过宽，随后以记录在 raw/008 的 SIGTERM 终止；没有输出用于结论）

ls -l AGENTS.md docs/AGENTS.md progress/AGENTS.md /home/toolchain/AGENTS.md /home/toolchain/development/AGENTS.md
exit=2（列出的 AGENTS.md 不存在）

ls docs/progress/R100 docs/progress/R101/tables docs/progress/BUILD_WEEKEND_0918/w4
exit=0
```

其余命令与退出码由日志器或取证脚本直接记录。

- raw/011：VCS 正则最初仅接受 platform/，遗漏实际 product/upstream/gmp；改为读实际字段后 raw/012 成功，全部远端查询基于修正后的 73 行。
- raw/022：试读的既有文件名不存在；raw/023 枚举真实名称后 raw/029 读取成功。
- raw/025：Python -c 换行转义 SyntaxError；改用落盘脚本，没有执行任何 Git 命令。
- raw/026：R101 VCS 字段包含 VCS= 前缀，初版 baseline 正则未匹配；后续按实际仓路径精确匹配，最终 history_results 使用修正版。
- raw/049：codes/llvm 无指定运行时提交对象，作为历史不可得检查保留；raw/052 祖先检查退出 1 是“不为祖先”，不是网络错误。
- remote/ 中的退出 2 均表示指定引用不存在，已与网络访问失败区别。

没有修改被读取的仓库、表、源码或配置。
