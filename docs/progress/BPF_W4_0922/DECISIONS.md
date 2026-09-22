# 自行判断与待确认事项

**续轮状态更新：** 人工已授权完整已验证配方，下面“待确认范围”是上一轮历史，现已解除。最新操作与限制见文末续轮补记；包仓未推送。

## 必须由人工裁决的配方范围冲突

远端 `sandbox/lhmax2025/libcxx-migration` fetch 成功，FETCH_HEAD 与本地 HEAD 均为 `f895f8c0373d224847fc7d3ecbeaac3bf926a1a1`。来源 `ssh://lhmax2025@review.tizen.org:29418/platform/upstream/bpftrace`，实际可写候选克隆位于 `tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push`，不是 codes。证据 raw/010、012、018。

任务第 7 条要求同时满足：①完整等于 BPF_STATIC_0922/recipe 已验证配方；②只增 Patch0、%patch0 两行和补丁，不动其他行。但基线 spec **缺少**已验证配方中的四项 static BuildRequires、共享 libgcc 切换、原 LDFLAGS 保存、独立 static 构建段、按工具链选择新 static/原 Source1002 的安装段。完整差异已存 `BASELINE_TO_VERIFIED_RECIPE.diff`。

`BPF_STATIC_0922/SPEC_DIFF.patch` 的比较起点是当时的 **tmp 候选 spec**，不是远端 f895f8c 基线。这两个起点不能混用。只加两行会保留基线“安装预编译 static”的行为，不能如实称作已验证新配方；采用完整 recipe 又会超过“其他任何行不动”的明确限制。

**已经向人工询问：是否允许完整已验证 recipe 的前置差异一并提交。未收到确认前不修改包仓文件、不创建包提交、不跑 prep。** 本轮不是“提交已备好”；状态为 `BLOCKED_SCOPE_CONFIRMATION`。没有 `SANDBOX_COMMIT.diff`、format-patch 或可推的候选提交。`BASELINE_TO_VERIFIED_RECIPE.diff` 仅为核对差异，不能冒称提交差异。

## 已执行的范围内判断

- 按批准恢复四份 tmp 解包副本，保留四份事故文件；不回写任何 RPM 或历史报告。
- 修正脚本另存本轮，旧脚本保留事故历史；采用一次性副本验证新 objcopy 调用，未跑完整旧审计或重编。
- GBS 保留根及当前 binfmt 是当前配置证据，不回填为历史 W1/OBS 执行证据；未唯一归因提速。
- “不推任何远端”按结尾具体指令区分：包仓一律不推；本项目的材料提交推送获明确授权。只提交本轮 docs 和 LINE_STATUS，不夹带现有无关工作树改动。
- Patch 文件身份核对通过，两份均为 `abe6d2fa379195c83c2317d368df306ade03c9cc7a1c8dc3f58ad25e65ae5b9e`，见 PATCH_IDENTITY.json。仅核对未应用。

## 仍挂账

- `-lstdc++fs` 来自既有 static 链接流程、解析到 GCC 14.2 静态归档，沿用前轮原始证据，本轮不改。
- Source1002 未修改；签字时需明确 libc++ 新候选与 GCC/未定义路径原资产的边界。
- rpm 安装和 %post 覆盖验证留待镜像阶段；可写区成对通过不是安装通过。
- ARM 静态 libgcc 对照 20/20 通过，与 x86_64 既有崩溃不能混作同架构结论；本轮未重测。
- ARM 主包字节差异、历史编译分派及提速唯一原因未归因。

## 技术性非零与记录限制

- 初始查 `codes/bpftrace` 失败（该路径不存在），随后定位上述 tmp 克隆；仅为只读查找，没有误改源码树。
- raw/005 恢复脚本语法错误发生于执行前，修正后 raw/006 成功；没有覆盖或删除事故证据。
- raw/004 搜索零命中退出 1；raw/011 diff 退出 1 表示确有差异，不是验证失败。
- 个别初始只读查看通过终端执行而非日志包装器；关键内容随后由日志命令再次保存。所有实际恢复、提取、复制、fetch 操作均有原始命令和退出码。
- raw/024 全量 `git diff --cached --check` 退出 2：原始输出保留的 TSV CRLF、空 flags 的尾空格、带行号空行，以及 diff 格式的空上下文行被报尾空白。为保留原始证据没有清洗这些文件；另对说明文档单独检查。此结果不是包配方语法/prep 验收，后者未执行。

## 续轮补记：人工裁决后

- 按新授权，从 recipe 直接复制完整 spec 和 patch，不重新手写。仅这两文件进入本地包提交 `72fda9941031fc35d8825e73446ca43153c4b69b`；不改 Source1002、-lstdc++fs，不新增开关，不改 codes。
- 批准的 BASELINE_TO_VERIFIED_RECIPE.diff 只含 spec 差异。核对分为：spec 全部 hunk 坐标和内容一致；新增 patch 与 recipe 字节一致；整个提交只有上述两文件。这不把新增 patch 错称为旧 diff 已包含的部分。
- raw/041 首次比较器把 Git 在 `@@ ... @@` 后附加的上下文提示当作内容，误报不同；当时文件 SHA 已完全一致，尚未提交。raw/042 核对确认只为 hunk 元数据格式，修正比较器仅去掉该提示（不忽略增删行、坐标、空白），raw/043 通过后才提交。配方没有调整。
- raw/046 首次 prep 包装器的资源断言未考虑 cgroup 页对齐，比精确 50% 少 2048 字节即失败，未进入 rpm/prep；raw/048 采用向下页对齐的严格上限检查，实际 memory.max 16536457216，未增加限额，使用新输出目录成功执行 -bp。
- 原 GBS/input 根均只读绑定；唯一可写区为本轮全新 prep 树。实际 sed 只改新的解包副本，属于获准 %prep 范围；既有构建树不动。
- 提交信息使用该克隆既有 git 身份，未更改身份配置；英文全文描述所有变更，不含 Signed-off-by。
- 条件表分清 CLI 显式覆盖、项目宏、spec 内派生宏，以及解析前/构建时状态。常规格有效值是静态核查且由旧日志支持，不冒充本轮重跑或历史直接宏数值转储。两个不一致的构建阶段组合均未测。
- 已核配置默认 clang 路径中两条件同步；实际 QuickBuild 的最终宏集未取得，外部强制 build_with_libcxx=1 加 GCC 的情况未覆盖。只在签字材料申报，不自行改配方或增加门禁开关。
- 原任务不推 sandbox/包仓仍有效；仅项目材料提交推送。安装/%post 验证仍留镜像阶段。
