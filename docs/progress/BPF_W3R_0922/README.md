# 阅读顺序

**先读INCIDENT.md：本轮存在只读纪律违反，四份tmp主包副本被objcopy改写，未恢复。build_audit.py是出错的历史脚本快照，不要重跑。**

1. FINAL_RESULT.md：三部分结论表与未闭合项。
2. INSTALL_CAUSE.md：实际挂载、Size、固定RPM源码原文与版本边界。
3. BUILD_TIMING.md：真实时长、缓存检查、主包ELF/.text比较和归因限制。
4. BOARD_RESULT.md：全部成对结果、前50行、取消断言、清理。
5. DECISIONS.md：方法选择、技术性非零记录、尚存疑问。
6. W4_INPUTS.md：原配方/RPM身份、板测数据、镜像阶段安装挂账。

原始命令、退出码和输出在raw/、board-*/、build-audit/、history/、compiler-read/；执行脚本位于本目录。新抓源码固定提交及SHA在INSTALL_CAUSE.md。不重新构建；原RPM保持未变，四份tmp解包副本误改另行申报，不宣称全部输入未改。
