# W4：最终状态与发布边界

## 替换判定

**现有候选不能作为已验收资产替换发布。整体 PARTIAL，未推包仓、未覆盖 Source1002。**

| 项目 | 实际结果 | 证据 |
| --- | --- | --- |
| armv7l / GCC 完整 bpftrace RPM | PASS；3 RPM，static 与原资产字节相同 | W1_MATRIX.tsv；verify-bpf-armv7l-gcc/result.json |
| armv7l / 未定义工具链 | PASS；3 RPM，static 与原资产字节相同 | verify-bpf-armv7l-undefined/result.json |
| aarch64 / GCC | PASS；3 RPM，static 与原资产字节相同 | verify-bpf-aarch64-gcc/result.json |
| aarch64 / 未定义工具链 | PASS；3 RPM，static 与原资产字节相同 | verify-bpf-aarch64-undefined/result.json |
| aarch64 / libc++ | rpmbuild 退出 0、3 RPM，但 static 多出 libbpf.so.1，不满足约定静态依赖门禁 | W1_REPORT.md；aarch64-first-rpm-set-result.json |
| armv7l / libc++ | W1 时间窗口内未完成，停在约88%；0 RPM | ARM_BUILD_CHECKPOINT.json；cells/bpf-armv7l-libcxx/ |
| armv7l LLVM 整轮补做 | 再次退出1，cpio Function not implemented；18 RPM 单独核验通过，整轮仍不完整 | W2_REPORT.md；llvm_retry_partial/result.json |
| ARM 物理板取消，共享展开器 | 四种等待各5次，20/20 PASS | BOARD_CANCEL_MATRIX.tsv；board_cancel/results.json |
| ARM 物理板取消，静态展开器对照 | 同样20/20 PASS，未复现x86崩溃 | 同上；不得写两架构同机制 |
| ARM 候选安装、功能等价 | NOT_OBSERVED；缺合格候选RPM | W3_REPORT.md |
| ARM 原资产RPM安装、功能基线 | 正常安装预检缺BCC/Clang依赖，NOT_AVAILABLE；未绕过检查 | board-install-original-only/016.stdout |

“六格”是2架构×3路径，不是6个文件。实际每个写包成功格有 main/common/static 三包；本轮 bpftrace 共15份 RPM：12份来自四个通过格，3份为 aarch64 libc++ 门禁失败候选。它们的逐文件 SHA 在 `W1_RPM_MANIFEST.json`，不能全部称通过。x86_64 不在此轮 RPM 范围。

四个回退格分别识别到120条实际编译/链接命令，匹配的 libc++ 选项为0，展开 spec 的 libc++ BuildRequires 为0；未定义路径使用明示的测试入口取消宏后包含原样候选，不把一次查询或 CLI --undefine 冒充完整构建。平台检查中 app-rootstrap 两份规则 skipped 的范围限制仍保留。

## 11 包状态与兼容性复核

`PACKAGE_STATUS.tsv` / `.md` 列全11包的分支、完整提交、原验证结果、本轮审计和候选备注。**11/11 已发布的源码适配状态不变**；tensorflow2 ARM/GCC 是已批准的“与基线同点同因失败”判据，不改写成六格均成功构建；LLVM沿用12格记录；bpftrace只有ARM/aarch64源码四格，static仍是原已批准单列资产。

本轮逐个从已发布提交读取 spec，字面审计以下事项：

1. 唯一 `build_with_libcxx=1` 定义受 `_toolchain` 已定义及 `toolchain_is clang` 双条件保护；
2. 扫描到的 `-stdlib=libc++`、`-lc++abi`、`LLVM_ENABLE_LIBCXX=ON` 和 libc++ 依赖行均在该启用条件内；
3. 1个阳性及3个反例校准（无条件泄漏、else分支泄漏、缺Clang条件）符合预期；
4. 11个远端 sandbox HEAD 均与所审提交一致，无漂移。

**11/11 PASS，未发现已扫描字面选项落在 GCC 分支。** `SPEC_GUARD_AUDIT.tsv` 给出每一行实际代码、行号、条件栈和 spec SHA256；`spec_audit_v2/` 保存原文；`package-head-results.json` / `package-head-commands.json` 保存远端核对。

边界：这是已发布 spec 的字面条件复核，不是本轮重建11包、不是完整RPM宏解释器，也不是证明任意外部 flags/宏注入都安全。既有实跑兼容性依据沿用状态表所列各轮构建；本轮新增完整回退构建仅为上述bpftrace四格。

技术性错误留痕：首次审计在 bpftrace 处因 `codes/R104/bpftrace` 不含已发布提交对象而退出1（raw/548、551、553），不是发现选项泄漏。按原推送报告第77行定位 `tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push`，确认同一完整提交后，仅修正读取仓库位置，v2审计退出0（559）；旧脚本、失败记录和部分输出保留。raw/555 额外列了不存在的 PUSH_RECORD.md，rg退出2；实际路径证据来自已存在的W1_REPORT.md，不从缺失文件推断。

## 尚未闭合与待裁决

- aarch64 static 额外动态libbpf：已定位裸 `bpf` 链接项；是否允许在 tmp 候选的静态 libc++ 分支中改为已找到的 `${LIBBPF_LIBRARIES}`，仍待人工确认。未修改该处、未放宽门禁。
- ARM libc++ bpftrace需从已保存断点继续；原生交叉工具许可/等价抽查尚待确认，当前没有切换，不能借用aarch64的许可。
- ARM LLVM整轮的cpio故障仍需归档层诊断；这次新18包不能证明整轮可复现，新libclang.a与旧输入也非字节相同，原因未归属。
- 物理板需准备已明确缺失的BCC/Clang依赖，并得到合格ARM候选RPM，才能完成安装、版本/info/probe与三脚本的成对功能验证。
- ARM静态取消通过而x86静态组失败，架构差异根因未作源码级验证；不能按原预期写结论。单独列入 QUESTIONS.md。

## 清理、范围与交付

板上本任务上传副本已全部删除并核验，未装包，root状态恢复；随后主动断开本次SDB连接，设备列表为空（570/571）。这是正常收尾，不是异常连接中断。本机列明名称的构建进程检索无匹配（572退出1）；磁盘约205GiB（573）。失败/未完成构建树、RPM、本地探针均保留，不删除断点。PC sudo调用0；不读取凭据。

W1 项目提交 `5576313e4ab957b1ff009308451c2b04b5fe6297`；W2 `fd1860537fbbff8bbee864eb85da82212fb6b4e1`；W3 `6d04975f0632c92001524114b8eb945c7d2ac6d2`。均普通快进到 `codex/r5-ehabi-diagnosis`，其他三个分支逐项不变，见各 `W*-project-delivery.json`。W4提交与推送回执在本阶段收尾产生后另存；不声称提前知道自身提交SHA。

本轮自行判断：四格回退构建和取消探针可独立保留；功能门禁不可相互代替；发现取样结果与预期不同即按实测报告。未自行决定额外CMake修正、部署系统依赖或发布static替换件。各阶段在原时限内完成/停止，不把后续阶段时限回借给W1。
