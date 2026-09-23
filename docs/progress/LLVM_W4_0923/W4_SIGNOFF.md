# 签字单：BLOCKED，当前不要推送

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
