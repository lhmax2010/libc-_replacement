# W4 输入（未授权推包仓）

- 完整候选spec：recipe/bpftrace.spec
- 完整spec差异：SPEC_DIFF.patch
- 新Patch：recipe/static-link-libbpf.patch
- 配方/补丁身份：recipe/IDENTITY.json
- 六份RPM路径、SHA、大小：RPM_INVENTORY.tsv
- RPM依赖：RPM_REQUIRES.tsv
- RPM内新ELF验收：verify-armv7l/result.json、verify-aarch64/result.json
- 物理板证据/恢复：board/；不得引用旧ELF当成本轮输入
- -lstdc++fs解析：STDCXXFS_RESOLUTION.json、link-audit-*/；本轮未改

当前：W2R_COMPLETE，W3=BLOCKED_AT_NORMAL_INSTALL_PREFLIGHT。安装预检阻断、未执行项与尚存门禁见FINAL_RESULT.md与BOARD_ANALYSIS.md。未获得W4人工签字，不推sandbox或包仓，不启动QuickBuild。
当前阻断是正常static安装预检退出1（MSM策略写入报错及文件系统92MB提示），不是已证明的候选执行失败。功能对照和两组取消均未执行，须先补齐W3，不能直接以W2R通过签发替换。板上五个上传文件按要求保留，root模式已恢复。
