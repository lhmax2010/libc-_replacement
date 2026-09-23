# 签字单 R3：两笔本地提交已备好，额外历史 spec 差异待人工裁决

| 顺序 | 本地提交 | 内容 |
| --- | --- | --- |
| 1 | 617a210064c4559fe1152728f7c9cf5e1ab99a9f | 条件产出libclang.a，正文已纠正 |
| 2 | db5b49afa7836db0414140208b2d3ab4f4aa7293 | 条件产出/安装/打包libc++.a与libc++abi.a |

当前干净，ahead2/behind0；作者均Hao Lin；无Signed-off-by；包仓未推。

人工审阅：

1. `SANDBOX_COMMITS_R3.diff`、两份 `0001/0002-local-recipe_R3.patch` 与提交正文。
2. runtime四组忽略纯空行后的严格等价性PASS；原始diff及R2 FAIL保留；ARM `%prep`退出0，未build。
3. `spec-audit_R3/LOCAL_SPECS_R3.md`及`EXTRA_DIFFERENCES_R3.diff`：有26条本轮两项以外的历史配方差异，27项缺唯一原仓参照，不能自动视为当前QuickBuild需要推送的改动；按指令已停报，需人工裁决后再闭合清单。
4. runtime与指定c5358237…候选逐字节一致，共享库/ABI脚本/展开器设置不改。
5. Source1002、-lstdc++fs不动；正常RPM安装验证仍留待镜像阶段。

仅在上述审阅完成、人工决定推送后，在 `tmp/BUILD_W1BC_0911/target-fetch` 中执行：

```sh
git config user.name "Hao Lin"
git config user.email hao.lin@samsung.com
git rebase --signoff f203923a1508c9344f5fc6b17bd8822f011655c4
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git rev-parse HEAD
```

签字后两SHA会变化；远端须等于人工签字后的HEAD。普通推送拒绝即停，不force。该流程由人工执行，本轮不代签、不推包仓、不启动QuickBuild。

## R2历史签字单（已由R3取代，不作为当前操作指令）

第一笔已只改正文为 `617a210064c4559fe1152728f7c9cf5e1ab99a9f`，无Signed-off-by。第二笔未创建；当前ahead1/behind0。

R2人工审阅项：

1. `SANDBOX_COMMITS_R2.diff`及`0001-Build-static-libclang_R2.patch`当前只有第一笔；旧2cba97e5已被替代，tree不变。
2. LLVM新四组展开PASS；旧GCC override门禁已依裁决作废。
3. `prep-check/runtime_R2/default.diff`的12个开头空行能否单列为非功能差异；未豁免、未创建第二笔、未执行runtime prep。
4. 第二笔完成后审与指定c5358237…候选的功能一致性。
5. `inputs_R2/ALL_INPUTS_R2.tsv`仍有未归类项；两项获准C也须真正推送才转B。
6. -lstdc++fs、Source1002不动，RPM安装验证留待镜像阶段。

仅在两笔与输入门禁都齐备后，人工在`tmp/BUILD_W1BC_0911/target-fetch`执行（本轮不执行）：

```sh
git config user.name "Hao Lin"
git config user.email hao.lin@samsung.com
git rebase --signoff f203923a1508c9344f5fc6b17bd8822f011655c4
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git rev-parse HEAD
```

签字会改变SHA，核对签字后的HEAD；拒绝即停，不force。**当前不执行上述命令。**

## R1历史签字单（已被R2取代，不作为当前操作指令）

本地候选 `2cba97e503d506cc184c34b6a4301087b6fa4bb7` 已准备，未签字、未推送。不能标记“所有门禁通过”：

1. 审 `BASELINE_TO_VERIFIED_LLVM.diff`、`SANDBOX_COMMIT.diff`、`0001-Build-static-libclang.patch`，仅一行属于本轮授权。
2. ARM22/aarch6412所用spec同SHA，libclang.a均由llvm-static-devel提供；核LIBCLANG_PACKAGE.json。
3. ARM %prep通过；但 `_toolchain_override gcc` 仍进入Clang/libc++分支，静态开关出现1次。需人工决定如何处理既有强制Clang逻辑与本次门禁要求，**不自动换成_toolchain gcc算通过**。
4. 提交正文关于GCC保持原样只在最终实际选择GCC的口径下成立，不能外推到上述override组合。签字前应审正文、决定是否改写；本轮未代签/未自行改写已建提交。
5. 审11包 RECIPE_AUDIT.tsv及完整diff。另审UNPUSHED_LIBCXX_RUNTIME.diff：两个静态运行库的供给还不能仅凭本轮libclang.a补齐推断已解决；是否另开授权加入runtime配方或指定其它已验证供给，留人工。
6. 正常RPM安装/%post验证仍留待镜像阶段；不改变既有bpftrace板测结论、Source1002或-lstdc++fs。

## 仅在上述问题获人工裁决后，由人工执行

在包仓克隆 `tmp/BUILD_W1BC_0911/target-fetch`（确认当前分支为sandbox/lhmax2025/libcxx-ehabi-backport）中：

```sh
git config user.name "Hao Lin"
git config user.email hao.lin@samsung.com
git commit --amend --no-edit --signoff
git push origin HEAD:refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git ls-remote origin refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
git rev-parse HEAD
```

amend会改变SHA，推后应与人工amend后的HEAD核对，不能继续以本轮候选SHA为远端预期。普通push被拒即停，不用force。本轮不执行上述命令，也没有批准QuickBuild。
