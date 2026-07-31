# T0 21.1.1 制品与输入本地打捞报告

状态：`DONE`（允许的部分缺失已逐项登记，未把未确定覆盖伪装成穷尽）。

## 结果

任务开始时磁盘可用 `1187452612608` 字节，80 GiB 守卫退出 0（`progress/T0/00_disk_guard.log:4`）。归档最终包含 186 个文件、`1077593465` 字节；`SALVAGE_MANIFEST.sha256` 全量验证退出 0（`progress/T0/16_salvage_summary.log:1`；`progress/T0/16b_salvage_manifest_verify.log:1`）。权威对象位于 `artifacts/archive_2111_salvage/`。

打捞明细：

- 三架构 candidate repo：24 个 RPM 及 21 个 `repodata/` 文件，另含每架构 `repomd.xml.sha256`；三架构源/副本 SHA 清单逐字 `cmp` 均退出 0（`progress/T0/05_candidate_repo_copy.log:1`；`progress/T0/06_candidate_copy_verify.log:1`）。
- 21.1.1 工具链缓存：在 25 个已枚举旧根中检查 24 个既存缓存目录，共选取并复制 74 个 clang/clang-devel、llvm/llvm-devel 与同根 binutils 子包；每个副本均重新计 SHA 并与源匹配（`progress/T0/07_toolchain_cache_search.raw.log:1`；`progress/T0/08_toolchain_selection.log:1`；`progress/T0/09_toolchain_copy.raw.log:1`）。
- 旧根 buildconf：复制 25 个 scratch 根的 `tizen*.conf`，全部 SHA 匹配（`progress/T0/10_config_search.raw.log:1`；`progress/T0/11_config_copy.raw.log:1`）。
- 冻结构建证据：复制三架构 `build/` 全部 30 个文件及 7 个顶层身份/报告文件；复制件中保有逐架构 `init_buildsystem ... --repository` 原文（`progress/T0/12_frozen_build_evidence_copy.log:1`）。
- 冻结配置输入：复制旧 gbs conf（SHA `a32c1aba…`）与正式 D5 buildconf（SHA `7181723c…`），源/副本 SHA 均一致（`progress/T0/13_frozen_config_inputs_copy.log:1`）。

全量对象—来源—SHA—取得时间—可复现性说明见 `progress/T0/INVENTORY.tsv:1`；旧根路径与大小（权限受限根仅申报可读下界）见 `progress/T0/old_root_sizes.tsv:1`。

## 缺失与搜索边界

精确包名 `lld-21.1.1`、`lld-devel-21.1.1` 未命中；缓存中的 `lldb` 未冒充 `lld`。旧根内未发现 gbs conf 副本，冻结构建命令引用的只读 `gbs_llvm.conf` 已从 `plan_evaluation` 单独打捞。三项均登记为 `NOT_FOUND`（`progress/T0/14_not_found_assertions.log:1`；`progress/T0/NOT_FOUND.tsv:1`）。

两次 `/home/toolchain` 全深度 `find` 因巨型目录遍历不能及时完成，失败原文与终止命令均保留；随后以同文件系统、`maxdepth=5` 成功枚举 25 个根（`progress/T0/01_old_gbs_roots_find.log:1`；`progress/T0/01a_inefficient_find_terminated.log:1`；`progress/T0/02a_full_depth_find_terminated.log:1`；`progress/T0/03_old_gbs_roots_maxdepth5.log:1`）。因此“深度超过 5 的其他根”明确登记为 `NOT_DETERMINED`，不得把 25 根称作全深度穷尽。搜索口径集中见 `progress/T0/search_scope.tsv:1`。

所有源位置仅被读取；复制目标全部位于工作根的 `artifacts/archive_2111_salvage/`，未删除或修改任何旧根/只读参考源对象。
