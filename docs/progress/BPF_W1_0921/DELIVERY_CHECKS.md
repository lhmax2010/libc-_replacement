# 交付自检

- `raw/450_identity_recheck`：74项输入身份复核PASS，退出0。
- `raw/449_rpm_requires`：18份RPM实际依赖声明查询，607行，退出0；未安装。
- `raw/456_publish`：3068份证据文件，38,857,012字节，复制逐文件SHA256一致，凭据扫描PASS，无跳过的Python缓存。后续发布会增加本页和提交回执，最终清单以PUBLICATION_MANIFEST.json为准。
- `raw/458_staged_check`：1806份本次暂存新增/变化文件，范围仅本轮docs与LINE_STATUS，凭据扫描PASS；未暂存其他工作区变更。
- `raw/459_diff_check`：`git diff --cached --check`退出2，**不记为通过**。`raw/461_whitespace_paths`列出了所有涉及路径。三个生成TSV使用Python csv默认CRLF行尾；其余提示来自原始build.log、展开spec、执行脚本快照及原始输出的行末空白/末尾空行。为了保留证据原文，不修改这些记录；该结果不是编译、写包或RPM摘要失败。
- RPM与大型ELF保留在`tmp/BPF_W1_0921/`，不作为项目Git附件上传；完整路径、大小、SHA256在RPM_INVENTORY.tsv/ELF_COMPARISON.tsv中。全量strings临时输出不入库，命令、退出码、计数、摘要与检索样本已保存。没有静默排除任务证据。
- 本轮未使用sudo，未推Gerrit/包仓，未上板，未覆盖Source1002，未编辑平台源码或spec。六格实际写包均退出0；两份libc++static额外libbpf依赖不合格保持挂账。
