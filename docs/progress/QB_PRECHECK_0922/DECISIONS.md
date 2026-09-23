# 决策与执行边界

- 人工已明确澄清：允许项目仓 codex/r5-ehabi-diagnosis 提交并推送本轮材料；“不推任何远端”针对包仓/sandbox。本轮只向项目仓推送，不向 Gerrit 或包仓推送。
- 资源门禁 `tools/resource_gate.sh --level light` 实际退出0（raw/001）；无构建、无安装、无开发板。只读根查询复用W1隔离绑定并改为只读，命令逐项记录于 roots/commands.json。
- `osc -A https://build.tizen.org meta prjconf Tizen:Base:Toolchain` 是尝试方法，不是已确认的服务端身份。缺 .oscrc 导致本地提示 EOF/退出1；未创建配置、未提供或猜测凭据。OBS真正项目标识/接口与宏值均未取得。公开reference索引仅作 fallback 输入身份，不提升为当前OBS配置。
- 查询宏文件中 grep 无命中的退出1按未命中记录，不冒充系统错误或宏不存在于所有来源。个别历史路径检索未命中保留原记录；最终结论使用实际找到的09-17 STATUS与raw/202 argv，不凭脚本意图猜。
- 预期失败表保留原有条件。六个GCC包的driver风险不改写成逐包实测失败；tensorflow2 GCC失败不改写成默认Clang必败。未找到通用失败清单不是“全量一定成功”。
- PACKAGE_STATUS仅改bpftrace一行，包括该行旧static说明，避免新SHA搭配过时状态；其他10行和表外历史说明不变。表头残留旧时态明确在FINAL_RESULT提示，没有越权更新历史LLVM行。
- bcc-tools-static源码来源可由RPM头确定，但Distribution未填，当前在线二进制来源/可用性不猜。四个静态BR源码在范围内不等价于OBS构建输入齐备。
- 五份libclang身份差异不归因；-lstdc++fs不动；RPM根修复不实施；gmp既有判断不改；ARM静态展开器的实测与x86结论不混同。
- 本轮推送材料只含文本/脚本/查询记录，不包含RPM、库或凭据；提交前限定暂存范围并安全自检。脚本快照与SHA随材料交付。

## 2026-09-23 人工确认后的补充

- 人工明确允许作者/签字邮箱、Gerrit用户名、SSH仓库URL入库，这些公开标识不是凭据。raw/022的暂停保留，不改成成功；raw/023仅记录命中文件路径。扫描现按该裁决区分公开标识与密码，仍检查密码及编码、私钥/token/cookie/认证头、认证配置文件内容；不打印秘密、不复制认证文件。
- 源码确认w5T的无数字分支不越界；只有末尾T<数字>进入问题数字扫描分支。先前把服务端宏未观测列为必须先选择payload对策的阻断不准确，FINAL_RESULT/LINE_STATUS/PAYLOAD已修正；QuickBuild无需payload对策。本地失败来自为单线程限制而追加T1，与libc++无关、不在任何spec/sandbox中。本地继续w5且禁止T<数字>。
- 原RPM抽样选择固定Base-Toolchain下载清单中的libllvm、libicu、boost-devel（约9–13MB），不拿本地w5重包替代发布包；三份重算SHA均与原清单相同，包头均xz/5T。历史快照与当前OBS宏仍分开。
- raw/025、026、027的部分路径检索未命中保留；raw/030因本机Python没有hashlib.file_digest退出1，未执行RPM查询，改用等价流式SHA计算后raw/031全部成功，未改RPM或配置。
