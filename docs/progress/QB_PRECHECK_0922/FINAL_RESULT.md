# W4 推送登记与 QuickBuild 前置核查

W4 已由人工推送，远端与已验证本地提交树内容完全相同；11 个包远端 SHA 全部匹配。四个新增静态 BuildRequires 的源码提供方均在 Base-Toolchain 范围内。**QuickBuild 无需 payload 对策；三份现有 armv7l RPM 包头均为 xz / 5T，源码确认无数字的 T 不触发该缺陷。OBS 当前实际宏值仍记 NOT_OBSERVED，不冒充已取得。** 本轮无构建、无板操作、无 spec/config/平台源码修改、无包仓推送，也未启动QuickBuild。

根因口径更正：`ARM_RPM_DIAG/upstream-rpmio.c:765–777` 中 T 后无数字走 else（threads=-1），分支内指针不前移，不越界；仅 T<数字> 进入 if，while 扫描停在终止符，再由外层 for 的 mode++ 越过终止符。因此本地项目配置与两个保留根的 `w5T.xzdio` 不触发。09-17 失败是本地为限制 xz 线程数覆盖 `w5T1.xzdio` 引入（`BUILD_STATIC_0917B/resume_0917/STATUS.md:67–70`），与libc++无关，不在任何spec或sandbox提交里。后续本地构建继续用单线程 `w5.xzdio`，禁止 T<数字>。不再要求人工在QuickBuild前选择payload对策。

## 一、W4 推送登记

| 核对项 | 实测结果 | 证据 |
| --- | --- | --- |
| bpftrace 远端分支 | `sandbox/lhmax2025/libcxx-migration` | raw/002 fetch、003 ls-remote |
| 远端 HEAD | `7831fb34c6881b5cff0b8bdd0dac27a1829290fa` | raw/003、REMOTE_BRANCHES.tsv |
| Author | Hao Lin `<hao.lin@samsung.com>` | raw/004 |
| Committer | He Fangyu `<fangyu.he@samsung.com>` | raw/004 |
| Signed-off-by | He Fangyu `<fangyu.he@samsung.com>`，与 Author 不同、与 Committer 相同；只登记，不改写 | raw/004 完整提交正文 |
| 与本地候选比较 | `git diff 72fda9941031fc35d8825e73446ca43153c4b69b FETCH_HEAD --stat` 无输出、退出0 | raw/005 |
| 两提交 tree ID | 均为 `0cd67fd05efc0a16d7e2463695f1f2b040bbf2fe` | raw/019 |
| 状态表变更 | 仅 bpftrace 行改新 SHA、新 static 配方状态与材料索引；其余10行及其他文本保持 | PACKAGE_STATUS.before.md、交付自检 |

旧周末表的开头及 LLVM 行仍是当轮历史状态，本任务授权仅改 bpftrace 行，故未扩改；**不得将该表剩余历史文字当成当前 static 未写包/LLVM 只有18份的结论。** 当前状态以本报告和 LINE_STATUS 为准。

## 二、QuickBuild 前置核查

| 项目 | 结果 | 范围/依据 |
| --- | --- | --- |
| OBS 实际 `_binary_payload` | **NOT_OBSERVED** | osc 缺配置，在本地提示阶段退出；API/项目身份未获服务端确认；PAYLOAD.md |
| 本地项目与 W1 两根宏 | `w5T.xzdio`；通用宏文件为 `w19.zstdio`，被用户宏覆盖 | 两架构各实际 `rpm --eval`；roots/ |
| w5T→w5T1 的来源 | 本地 CLI override；不是项目同步 | 09-17 STATUS:65–72、raw/202 实际 argv |
| 公开 Base-Toolchain reference | 本次 build.xml 为 `tizen-base-toolchain_20260914.073422` | raw/017；不是服务端 prjconf |
| 已下载armv7l包头 | libllvm、libicu、boost-devel均为 xz / 5T，3/3查询退出0且原下载SHA匹配 | RPM_PAYLOAD_HEADERS.json；固定快照20260828.101647，非当前OBS宏 |
| 4 个新增 BuildRequires | 4/4 源码在 R101 254包范围；bcc-tools 已适配，其余3无需改 | 下表、STATIC_DEPENDENCIES.tsv/json、roots/*-static-packages.out |
| 11 个 sandbox 远端 | **11/11 MATCH**；llvm 用 libcxx-ehabi-backport，其余用 libcxx-migration | REMOTE_BRANCHES.tsv 含完整URL、分支和SHA，REMOTE_RESULTS.json含命令/退出码 |
| 预期失败对账 | 已确认的 tensorflow2/armv7l/GCC 失败是有条件条目；没有证据把它或6个driver风险包直接记为默认Clang必败 | EXPECTED_FAILURES.md |
| 五项挂账影响 | 均逐项登记；不作修复、不改既有判定 | OPEN_ITEMS.md |

| BuildRequires | 源码包 | armv7l 版本-发行号 | aarch64 版本-发行号 | R101 表行 / 判定 |
| --- | --- | --- | --- | --- |
| bcc-tools-static | bcc-tools | 0.35.0-115.2 | 0.35.0-112.1 | 14 / 需要改，已推送 `5bd0654e24bb231035ea684d278f674aa95a2cd2` |
| libbpf-static | libbpf | 1.6.2-1.1 | 1.6.2-1.1 | 91 / 不需要改 |
| liblzma-static | xz | 5.8.1-1.9 | 5.8.1-1.10 | 251 / 不需要改 |
| zlib-devel-static | zlib | 1.3.1-1.10 | 1.3.1-1.11 | 254 / 不需要改 |

查询对象是 W1 实際使用的 `tmp/BPF_W1_0921/input-armv7l-origin/rpmdb`、`input-aarch64-v3/rpmdb`，不是当前在线仓。SOURCERPM 完整值与元数据在 TSV。bcc-tools-static 的 Distribution 为 `(none)`、Vendor 为 `None (open source)`，所以只凭源码RPM名与历史输入记录确认提供方，**不冒充官方仓已发布该二进制包的证据**。其余三项 Distribution 为 Tizen-Base-Toolchain。没有源码包超范围或提供方未知的条目；在线 OBS 当前可用性仍 NOT_OBSERVED。

## 三、备案、判断与未闭合项

| 项目 | 状态 |
| --- | --- |
| W4 | 远端内容与候选一致，推送登记关闭；不等于正常安装/全量QB已经通过 |
| LINE_STATUS | 更新 W4、payload 未观测、挂账与人工下一步 |
| 项目材料推送 | 人工已明确允许，仅本目录、LINE_STATUS、PACKAGE_STATUS 的 bpftrace 行；回执见交付记录与最终回复 |
| 不触碰边界 | codes只读；未起构建/QuickBuild、未上板、未推任何包仓/正式分支、无force |
| 后续 | 人工起 Tizen-Base-Toolchain QuickBuild（指向11个sandbox分支，SHA见REMOTE_BRANCHES.tsv）→结果与预期失败清单对账 |

自行判断：将本地/公开快照/OBS服务端三个证据层分开；将有条件的历史失败与默认Clang预期分开；将Author/Committer/签字身份不一致按事实登记，不推断原因或修改提交。普通fetch会更新本地远端跟踪信息，未修改工作树或向包仓写入。完整判断及技术性非零见 DECISIONS.md。

尚存观测边界：服务端真实项目/API/最终宏集；服务端静态开发包与 libclang.a 输入可用性；正常RPM安装及%post留待镜像阶段。这些不由本轮“远端SHA匹配”自动关闭，但本次人工已裁决无需payload对策、下一步人工起QuickBuild。公开作者/签字邮箱、Gerrit用户名和SSH仓库URL获明确允许保留，不作为凭据；密码/token/私钥/cookie及认证配置内容仍禁止提交。完成后停，交人工审阅。
