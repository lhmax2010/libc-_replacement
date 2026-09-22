# 阅读顺序

1. FINAL_RESULT.md：两架构写包、RPM/ELF验收与板测门禁总表。
2. BOARD_ANALYSIS.md：实际安装预检失败、未执行项和保留现场；不把未测试写成等价。
3. SPEC_DIFF.patch、recipe/：W4候选配方与新Patch，原spec未动。
4. RPM_INVENTORY.tsv、RPM_REQUIRES.tsv、verify-*/：六份RPM身份、文件/依赖清单、从RPM提取ELF的核验。
5. STDCXXFS_RESOLUTION.json、link-audit-*/：实际链接解析，保留W4待办。
6. DECISIONS.md、CONTINUE.md、raw/、cells/、board/：方法、工具错误留痕、原命令/退出码、资源与板端记录。
7. W4_INPUTS.md：下一步输入与未闭合项；本轮不推包仓、不起QuickBuild。

RPM、ELF、源码tar和构建树留在tmp，不提交大二进制；路径与SHA均随表提供。
