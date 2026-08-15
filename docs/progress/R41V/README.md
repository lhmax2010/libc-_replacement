# R41-V 人工审阅材料

本目录仅整理 R41 已存在的代码、构建记录、原始输出和只读参考源码。
未重新构建、运行或连接开发板，未修改任何既有代码。复制的原件保持
字节不变；`INDEX.tsv` 记录来源、原路径、上传路径与 SHA256。

目录说明：

- `observation/`：R41 相对 R40 的完整 diff、R41 完整当前文件及 R40
  输入文件；
- `tests/`：R41 使用的完整测试件源码；
- `build/`：测试件编译链接记录、libc++abi 配置记录、CMakeCache 和
  当时实际使用的脚本；
- `outputs/`：未经整理的完整运行记录，含命令、stdout、stderr 与
  退出码；
- `static/`：R41 引用的 GCC/libstdc++/libgcc 完整源码；
- `EVIDENCE_LOCATOR.tsv`：三项关键判断的证据类型与精确行号；
- `OBSERVATION_POINTS.tsv`：全部 R41 新增观测点的位置和字段；
- `BUILD_OPTIONS.tsv`：测试件编译、链接选项逐项展开；
- `SOURCE_LOCATIONS.tsv`：静态材料的相关源码位置；
- `MISSING.tsv`：当时未采集或已不可得的材料。

本目录不重新分析 R41，不生成新结论。
