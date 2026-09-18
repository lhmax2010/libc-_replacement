# 社区核查：同类机制已知，未确认 bpftrace 官方资产同样崩溃

W1 已实测 GNU 静态组同机制失败，因此本阶段前置满足。仅查资料，未向任何社区
发帖或提交补丁。

## 上游的两套构建形态不能混为一谈

1. bpftrace **v0.24.2** 的 [src/CMakeLists.txt](https://github.com/bpftrace/bpftrace/blob/v0.24.2/src/CMakeLists.txt)
   在 STATIC_LINKING 下确实加 `-static-libgcc` 和 `-static-libstdc++`。
   因此“这个选项是我们独有的”不成立。当前 master 的独立取样也已下载，不能把
   滚动 master 当成本次版本的构建身份。
2. 对应 [静态 CI 脚本](https://github.com/bpftrace/bpftrace/blob/v0.24.2/.github/include/static.sh)
   开 STATIC_LINKING，检查剩余动态依赖为 musl loader 和 musl libc；
   [Dockerfile](https://github.com/bpftrace/bpftrace/blob/v0.24.2/docker/Dockerfile.static)
   基于 Alpine 3.22。这不是 `-static` 的全静态程序，也不是动态 glibc 取消路径。
3. [Release 工作流](https://github.com/bpftrace/bpftrace/blob/v0.24.2/.github/workflows/release.yml)
   走 Nix/create-assets。复用已下载的官方 v0.24.2 AppImage **内部实际 bpftrace**，
   重新算 SHA256、readelf：其直接依赖包括 `libstdc++.so.6`、`libgcc_s.so.1`、
   glibc 及 LLVM/BCC 等共 18 项。AppImage 外壳不是内部程序的链接形态。
   内部 SHA256 `46f917df57bd9f04525ecb991e49ae3fa61ddf16ee46105b89ee59abfddd9f7a`。

API Release/tree 已重新访问，HTTP 200；CI 原文、HTTP 状态、URL、SHA 在
`community_sources.json`、`upstream_more_sources.json`；内部 ELF 当前证据为
`official-appimage-inner-needed.json` / `official-appimage-inner-sha256.json`。
下载与解包身份的既有链条见 `../BUILD_W2C_0915/upstream_release/REPORT.md`。

## bpftrace issues/PR 检索

匿名 GitHub issues search API，同一仓库、同时覆盖 issues 与 PR，关键词如下：

| 查询词 | API 总数 | 本次取回 |
| --- | ---: | ---: |
| pthread_cancel | 0 | 0 |
| _Unwind | 0 | 0 |
| static-libgcc | 13 | 13 |
| libgcc | 21 | 21 |
| SIGABRT | 27 | 27 |
| unwind | 44 | 44 |
| static | 461 | 第一页 100 |

具体 query URL、incomplete_results、标题/正文均留档。static 是宽查询，**未声称
审阅全部 461 条**；其余精确词未检出描述本次寄存器尺寸表机制的已确认 bpftrace
取消故障。正向查询返回已知静态构建条目，不能把返回空页默认为检索器正常。
[issue 1710](https://github.com/bpftrace/bpftrace/issues/1710) 讨论静态构建形态，
不是本次取消回溯；另取得 342、4233、4243 的评论供核对，未据标题推断已修复本问题。
结论限于以上检索，**不是宣称上游从未遇到过**。

## GCC / glibc 已有记录

- [GCC 开发邮件，2012-03-08](https://gcc.gnu.org/legacy-ml/gcc/2012-03/msg00106.html)：
  讨论 libgcc_eh 隐藏符号、进程内展开器统一、glibc 内部动态装载 libgcc_s；指出
  非全静态场景应使用共享 libgcc_s。这是机制/使用方式说明，不是 bpftrace 专项修复。
- [glibc 开发者邮件，2016-10-14](https://sourceware.org/pipermail/libc-help/2016-October/003944.html)：
  直接讨论 pthread_cancel 与静态 libgcc_eh，指出多份展开器和隐藏符号导致的问题；
  文中探索性补丁未成功，不能作为本次可采用的上游已合入解法。
- [GCC bug 42850 的官方邮件记录](https://gcc.gnu.org/pipermail/gcc-bugs/2010-January/327517.html)：
  记录 dwarf_reg_size_table 未初始化及 glibc 隐含依赖共享展开器的历史故障。
  当时触发背景是共享库版本变更，与本次环境不完全相同；不能把它写成本次故障
  已被该 bug 精确覆盖或当前已修复。

本机 curl 对 GCC/Sourceware 页面为 403；浏览工具可读上述邮件文本。
Bugzilla 当前页面受访问限制，状态/精确修复提交 **NOT_AVAILABLE**；没有绕过挑战。
额外访问的 glibc bug 13378 未取得内容，也未确认关联，**不作为支持性证据**。
网页检索还见 libunwind 支持的提案，但它不是当前 Tizen 实测环境，未当作本任务解法。

## 是否向上游提

当前证据足以说明**动态 glibc 混用静态 libgcc 的风险已有社区解释**，不足以说明
bpftrace 官方 musl 静态 CI 或官方 AppImage 有同一未解决故障。因此未生成一份
宣称“上游 bpftrace 已复现”的 issue 草稿，也未提交 issue。

供人工选择的事实：

- “向上游提”：可以讨论 glibc 下 STATIC_LINKING 的适用边界；但本次直接复现对象是
  独立探针，上游产品内触发路径未确认，不能按已证实 bpftrace 产品 bug 对外表述。
- “照上游做法”：官方 AppImage 使用共享 libgcc_s；musl 静态 CI 则是另一种 libc，
  不能直接移植其取消语义结论到本平台。
- “我们自己改”：本轮仅删除候选的 `-static-libgcc`，共享 glibc 与共享 libgcc_s
  统一展开流程；实测结果见 W4。没有改 libc++abi 或 glibc 源码，没有自行选定发布方案。
