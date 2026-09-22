# W4输入

- 配方及diff：../BPF_STATIC_0922/recipe/、SPEC_DIFF.patch；原spec未动。
- 两架构六份RPM的实际路径/SHA/大小：../BPF_STATIC_0922/RPM_INVENTORY.tsv；本轮未重新写包。
- ARM static RPM：`tmp/BPF_STATIC_0922/rpm-bpf-armv7l-libcxx/RPMS/armv7l/bpftrace-static-0.24.2-1.armv7l.rpm`，SHA `672279ea1e93f896c6b82cd1d1b5d557ef6c8a8dcc9079793b2a2efe11f9e756`。
- ARM候选ELF：该RPM解出，SHA `6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba`。
- ELF依赖和命名空间：前轮verify-armv7l/result.json、verify-aarch64/result.json；本轮验证载荷身份未变。
- 新板测：BOARD_RESULT.md、board-test/pairs.json与cancel_summary.json；12组成对调用退出0、两组取消各20/20通过。
- **rpm安装验证留待镜像阶段**；不能把工作目录执行通过写成RPM安装或%post已验证。
- `-lstdc++fs`既有选择仍由W4裁决，详见前轮STDCXXFS_RESOLUTION.json。
- 新增挂账：ARM主包ELF与.text相比W1均不同；构建时长完整归因尚未取得；静态libgcc对照ARM通过，不外推x86_64崩溃。
- 板上工作目录已按全通过规则清理；需后续验证时从上述本地RPM重新部署，不能引用已删除板端路径作为现存输入。
- 下一步由人工W4审阅签字后决定配方及QuickBuild，不自动推sandbox、不起QuickBuild。
- **先处理只读违规挂账**：四份主包解包副本被本轮objcopy改写，路径与前后SHA在INCIDENT.md/INPUT_ELF_FINAL.json；不得当作原始ELF继续使用。原RPM仍可校验，恢复这些副本需人工批准，本轮未恢复；板测static不在这四份之内。
