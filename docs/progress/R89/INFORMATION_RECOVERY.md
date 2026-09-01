# SOURCE_PROVENANCE 信息粒度变化与恢复路径

## 1. 总体原则

旧文件试图同时承担两类职责：

1. 稳定内容身份；
2. 某一次 GBS export 的提交/补丁组织清单。

第二类信息会随 squash、rebase、cherry-pick 改变，而且 metadata commit
无法可靠记录自己的 SHA。新格式只承担第一类职责；第二类改由当次构建的
生成 spec、SRPM、构建日志和 git 历史承担。

## 2. 逐项变化

| 旧信息 | 新文件是否直接提供 | 从哪里恢复、怎么取 | 是否真正丢失及影响 |
|---|---|---|---|
| 当前 package/repository commit | 否 | 生成 spec 的 `VCS:`；SRPM 中 spec；RPM/QuickBuild provenance；`git rev-parse` | 不丢失。职责转交给 GBS 自动生成字段，避免自指 |
| 构建所在分支名 | 否 | QuickBuild/OBS project 配置、build record、git ref/reflog | 单靠安装后的文件会缺失；分支不是内容身份，影响低 |
| 导出 patch 总数 | 否 | SRPM 中 `*.patch` 数量；生成 spec 的 `PatchN:`；build log | 不丢失，只从构建产物取得；避免 10/7 等形态漂移 |
| PatchN 编号和文件名 | 否 | 生成 spec 的 `PatchN:` 和 `%patchN`；SRPM 文件列表 | 不丢失；它只对特定 export 有意义 |
| 每个 patch SHA256 | 否 | 从 SRPM 提取 patch 后执行 `sha256sum` | SRPM 保留时不丢失；只剩二进制 RPM 时会丢失 |
| 每个 stable patch-id | 否 | 对 SRPM patch 执行 `git patch-id --stable` | SRPM 保留时不丢失；用于跨 cherry-pick 对照，不是最终内容的唯一身份 |
| 每个 patch 对应 commit | 否 | patch header、生成时 git log、GBS build record | 若历史被重写且 SRPM/build log 也未保存，原 commit 归因可能真正丢失；不影响最终二进制复现，但影响审计归因 |
| `ehabi_backport` 对 Patch5/文件 0006 的映射 | 否 | 当次生成 spec、patch subject、build log | 不丢失于当次 SRPM；跨拓扑保留该映射反而会误导 |
| GBS non-native export、禁止 `--no-patch-export` | 是 | `source_export_mode`、`source_export_no_patch_mode` | 无损失 |
| GBS Source0 文件与 SHA256 | 是 | `source_tarball*` | 无损失；仍是基础源码复现锚点 |
| GBS export tag 与 peeled commit | 是 | `gbs_export_tag*` | 无损失 |
| LLVM 官方 release tag/archive/SHA256 | 是 | `upstream_release_*`、`official_release_*` | 无损失 |
| Source0 与官方 release 的差异、原因和构建影响 | 是 | 三个 `source_tarball_vs_official_release_*` 字段 | 语义保留；仅删除内部 R9C 命名和本地证据路径 |
| 最终 libc++/libc++abi 内容 | 旧文件没有稳定表达；新文件新增 | `validated_libcxx_tree`、`validated_libcxxabi_tree`，用 `git rev-parse REV:libcxx{,abi}` 核对 | 信息增强；不随提交组织变化 |
| 旧 platform frozen tarball SHA/status | 否 | git 历史、R9C archive、旧 RPM/SRPM | 从新安装文件中真正消失；该 tarball 已被当前 Source0 取代，对当前构建复现无影响 |
| superseded platform spec VCS/status | 否 | git 历史、旧 provenance、项目归档 | 从新安装文件中真正消失；仅属历史裁决，不是当前输入 |
| `progress/R9C/build_impact_verdict.tsv` 路径 | 否 | 本项目 R9C 归档 | 安装包消费者原本也无法访问该本机路径；删除不会减少可携带的有效证据 |
| R9-C adjudication 日期/内部任务名 | 否 | 本项目历史 | 从安装文件中消失；对发布包复现无影响 |
| ABI/unwinder/compiler-rt/symbol node | 是 | 文件尾部配置字段 | 无损失 |

## 3. 真正可能丢失的部分

如果发布系统只保留最终二进制 RPM，不保留 SRPM、生成 spec、patch 文件、
构建日志和 git refs，那么以下信息无法从新安装文件重建：

- 当次导出的 PatchN 编号、文件名和数量；
- 每个 patch 的完整字节、SHA256 与 patch-id；
- 被 squash/rebase 后已不可达的原始 commit 身份和作者归因；
- 已废弃 platform tarball/spec 的历史裁决细节。

这不会妨碍确认或复现最终 libc++/libc++abi 内容：Source0 SHA256、最终组件
tree 和 package `VCS` 仍提供三层锚点。但会降低“这一轮构建如何把提交组织
成 patch”的过程审计能力。

没有发现现有代码或打包脚本会解析旧 `export_patch_*` 字段；R89 对仓内
已解包源码与既有材料的检索未发现此类消费者。因此当前观测范围内不存在
依赖这些键的程序。平台外部私有工具是否解析它们为 `NOT_OBSERVED`。

## 4. 处理建议

重要但不稳定的信息应按“每次构建的产物”保存，而不是静态写入
`SOURCE_PROVENANCE`：

1. 发布归档长期保存对应 SRPM；
2. 保存 GBS 自动生成的 spec 和全部 patch 文件；
3. 保存完整 build log，其中含 `%patchN` 应用顺序；
4. 保存 QuickBuild/OBS build ID、project/repository 与 source revision；
5. 如需供应链自动化，生成一份随构建产物保存的 manifest，逐 patch 记录
   SHA256/patch-id，但由 GBS export 后生成，不提交回源码树；
6. 对最终内容继续使用新文件中的 Source0 SHA256 与两个 component tree。

这样既保留完整构建审计，也不会让一个源码内静态文件永久追赶分支拓扑。
