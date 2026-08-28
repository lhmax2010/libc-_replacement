# R76：四补丁 Gerrit sandbox / QuickBuild 提交验证

日期：2026-08-28  
最终状态：`SUBMITTED_BUILD_PENDING`

## 1. 结论摘要

四个定稿补丁的 SHA256 与 R69 记录逐项一致，并在 Gerrit 当前 `tizen_base` 基线 `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c` 上按 0001、0002、0003、0004 顺序干净应用。补丁文件、`SHA256SUMS` 和平台源码差异均未改动。

四个独立提交已直推 `refs/heads/sandbox/lhmax2025/libcxx-forced-unwind`。远端 sandbox HEAD 为 `74900f3fbb2969f7f127449f9b1d20df72b9eefa`，与本地最终提交一致；正式 `tizen_base` 推送前后均为 `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`。未使用 `refs/for/`，未使用 force，未推正式分支或 LLVM 上游。

本次沿用 R7 的 QuickBuild 触发形态：直推 sandbox 分支后由平台侧人工观察。当前环境没有 QuickBuild 日志入口，因此构建结果、产物、其他包影响和构建告警均为 `NOT_OBSERVED`，需要人工在平台侧查看；状态据此为 `SUBMITTED_BUILD_PENDING`，没有推断为成功或失败。

## 2. 资源门禁与补丁身份

`tools/resource_gate.sh --level light` 退出 0。

四补丁身份见 `PATCH_IDENTITY.tsv`：

- 0001：`af41f2989efcc28405bd26e3db24fb8e26c1fcca89f7d6f22c9140ccdb5beebd`；
- 0002：`a121ce3bb7b95d3170a5ae0acd885557a5c7c0ab100b360a469d129c9c985b6e`；
- 0003：`0a32dc44e0e57ed524283bc416bdc5c4f6a00b131de5e421485b6e7bcb4b82e9`；
- 0004：`88228328db96b38ca139ec1fa3b04ddd82c25f17b97e8a90c674feb2a654d90e`。

首次自动身份校验因 R76 脚本错误地在 `progress/R69/patches/` 内解释工作区相对路径而退出 1。该失败属于命令自身路径错误，不是身份判据不成立。经用户确认后，仅把两条身份校验命令改为从工作区根目录读取原 `SHA256SUMS`；修正 diff 为 `raw/041_preflight_path_fix.diff`。补丁与 `SHA256SUMS` 修正前后 SHA 文件经 `cmp` 退出 0。

修正后身份门禁在应用前和应用后均为 4/4 `OK`。原始记录见 `raw/apply_preflight/004_patch_identity.log`、`raw/apply_preflight/023_patch_identity_after.log`。

## 3. 基线与干净应用

只读查询确认远端 `refs/heads/tizen_base` 为 `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`，计划 sandbox 当时不存在。抓取精确基线后，在独立 worktree 顺序执行每个补丁的 `git apply --check` 和 `git apply --index`，八步均退出 0；最终 `git diff --cached --check` 退出 0。

校验日志位于 `raw/apply_preflight/`，聚合见 `raw/046_preflight_retry_digest.txt`。没有观察到冲突。

## 4. 四提交 series

提交者全部为 `lhmax2025 <hao.lin@samsung.com>`；补丁 author 保持原样：

| 顺序 | 层 | 提交 SHA | author |
|---|---|---|---|
| 1 | A | `a4d590eb5a2165a65e82eec6ff9dcf73f778242c` | `R51 Evidence <r51@example.invalid>` |
| 2 | B | `c92ab20e624234394804bac7c39bb753bf0ed874` | `R51 Evidence <r51@example.invalid>` |
| 3 | C 头文件批 | `9765bca48c0498b755ba8f1460abf1ef5a949cea` | `NOT_AVAILABLE <not-available@invalid>` |
| 4 | C 库内批 | `74900f3fbb2969f7f127449f9b1d20df72b9eefa` | `He Fangyu <fangyu.he@samsung.com>` |

四个提交线性相接。每个提交的 stable patch-id 与对应定稿 patch 相同；最终四提交源码 diff 与顺序 `git apply --index` 的结果逐字节相同；工作树干净。完整记录见 `raw/final_series_validation/commits.tsv`、`series.diff`、`patch_identity.log`。

原 message 已包含局部依赖，但没有完整写明四提交整体不可拆分：层 A 只写提交内“两处必须作为同一系列应用”；层 B 写 armv7l 依赖层 A；两个层 C 各写依赖 libc++abi marker/unwind fixes。按用户确认，只在层 A message 末尾增加中英文整体说明，原文保存在 `code/layer_a_series_dependency.txt`；其余三个 message 逐字不变。仓库 `commit-msg` hook 同时只在层 A 自动加入 `Change-Id: I7432efca9c400b75e054c53d890f3714c1f4733c`，这是已允许的 Gerrit 元数据。

新增依赖说明明确：四提交构成一套验证 series，固定顺序为 A、B、C 头文件批、C 库内批，不得只应用或验证子集；两个 C 批次依赖 B，并在 ARM 上依赖 A；只应用 A 而不带其余系列不是受支持部署，可能引入回归。

## 5. Gerrit sandbox 提交

实际命令：

```text
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-forced-unwind
```

退出 0，服务端创建新 sandbox 分支。完整输出见 `raw/086_gerrit_sandbox_push.combined`。服务端只报告 commit-message 风格 warning：两个 subject 超过 50 字符，三处提示 message 中超过 72 字符的行；这些不是编译 warning，也不是 QuickBuild 结果。按“不修改定稿 message”纪律未因此改写提交。

推送后精确 `ls-remote` 证明 sandbox HEAD 为 `74900f3f...`，正式 `tizen_base` 未变化，见 `raw/087_gerrit_post_push_verify.txt`。

## 6. QuickBuild

- 触发方式：与 R7 相同，向 `platform/upstream/llvm` 直推 sandbox 分支，由平台侧 QuickBuild/人工观察流程处理；
- 目标：仓库/包 `platform/upstream/llvm`，ref `sandbox/lhmax2025/libcxx-forced-unwind`；当前环境没有额外的构建 profile 或架构选择入口，具体平台 profile 为 `NOT_OBSERVED`；
- 当前结果：`SUBMITTED_BUILD_PENDING`；
- 完整构建日志：当前环境不可取得，需要人工在 QuickBuild 平台侧查看；
- 构建产物是否正常：`NOT_OBSERVED`；
- 与本地产物是否一致：`NOT_OBSERVED`，无法比对；
- 是否导致其他包构建失败：`NOT_OBSERVED`；
- 是否有指向改动的新增编译告警：`NOT_OBSERVED`；
- 重试次数：0。

人工回传平台日志前，不把 Gerrit push warning 误当作编译 warning，也不推断 QuickBuild 成败。

## 7. 已验证与未覆盖范围

提交前既有验证状态：x86_64 与 armv7l 全量验证已完成，三方独立评审提出的四项缺陷已修复并复验。R76 新增验证覆盖补丁身份、远端基线干净应用、四提交组织、author/committer、message 允许范围、patch-id、最终源码 diff 和 sandbox ref 身份。

尚未覆盖：aarch64、LLVM 官方测试套件全量运行、共存期/混合运行时场景，以及本次 QuickBuild 的实际平台构建结果。

## 8. 异常、决定与疑问

R76 共记录三类校验脚本技术错误：身份脚本路径解析错误；series message 校验最初漏计邮件 Subject；最终校验器最初未允许 hook 添加的 Change-Id，随后又遇到零匹配计数输出为空。每项均先停止、保留失败原文和修正 diff，再只修正校验逻辑；补丁及源码 diff 未改。

自行判断与尚存疑问见 `SELF_DECISIONS_AND_QUESTIONS.md`。
