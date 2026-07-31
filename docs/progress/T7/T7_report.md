# T7 配置层重建报告

状态：`DONE`。

## 新原件与提取来源

工作根 `gbs_llvm.conf` 已冻结为
`artifacts/config_2218/originals/gbs_llvm.conf`，SHA256 为
`8fab16c1ca9db28d45f6b269881af47a71ef3d111d93527b602f57e1e37d49cf`。

effective Unified/standard buildconf 不是从旧产物复制：使用 GBS 2.0.8、
全新 armv7l root 和当前两个 reference URL 重新下载，命令全文在
`progress/T7/extraction_command.txt:1`，GBS 明确报告下载路径在构建全程日志
`progress/T7/extraction_gbs.log:3`（`LOCAL_ONLY`，SHA256
`4af2d1dd8f66c19397e41573b6dfa9bc4593b519faf426bd2ff6c50d034bed23`）。探针完整构建退出 0；实际解析并安装
clang、libllvm、llvm `22.1.8-18.1.armv7l`
（`progress/T7/resolved_llvm22_evidence.log:1`）。buildroot 内 effective 文件
与冻结副本 `cmp=0`（`progress/T7/extraction_provenance.tsv:1`）。该探针根
已标 `RETIRED_EXTRACTION_ONLY`，产物不具晋级资格。

新下载 buildconf SHA256 仍是
`9eac0e23968b9bd7dabd463d400f0281d2464815fc2299431052f7b8cfb736a5`，
且与旧提取物逐字相同。这是独立重取后发现的事实，不是继承旧身份。旧
`a32c1aba…` gbs 原件与旧 `9eac0e23…` 提取身份均按规格保留并标
`SUPERSEDED_BY_PLATFORM_2218`；新、旧身份和“同字节”事实同时记录在
`progress/T7/config_identity_registry.tsv:1`。

## D5 来源与字节拼接

D5 来源选择规格允许的 21.1.1 正式 buildconf，源 SHA256 为
`7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`。
实际插入块已冻结为
`artifacts/config_2218/sources/d5_wave1_macro_block.conf`。源区块插入后从
最终副本反向抽取，逐字 `cmp=0`；`libc++-devel` Required 片段同样
`cmp=0`（`progress/T7/cmp_gates.tsv:1`）。因此没有取用 T-1 提交前的缺陷
宏形态。

正式副本仅用 `grep -aboF` 确定字节偏移，并以 `dd bs=1` 拼接 prefix、
冻结 fragment、suffix；没有用行式工具重写文件。偏移记录在
`progress/T7/byte_splice_offsets.tsv:1`，方法说明见
`progress/T7/generation_method.md:1`。

正式配置身份为：

- gbs：`c11c12b03fdf551e46072824ccdcac51b26a26a996103f9a8327ee7632d39a12`；
- buildconf：`7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`。

路径、大小、SHA 与状态见 `artifacts/config_2218/FREEZE_REGISTRY.tsv:1`（`LOCAL_ONLY`，`artifacts/` 禁止上传，SHA256 `16e2c2623ff881e68d4f57f2d6769b621e5326bdfb292851952d14417da40c66`）。

## 两级 diff 门与 OFF_PROVEN

gbs 正式副本剥离 candidate repo 段后与新 gbs 原件 `cmp=0`；buildconf
正式副本剥离 D5/Required 增量后与新 effective 原件 `cmp=0`
（`progress/T7/cmp_gates.tsv:1`）。两份文件末尾 16 字节 hexdump 均与原件
相同。实际变更行和旧验证增量分别排序成多重集后比较，两项均 `cmp=0`，
避免依赖 diff 呈现顺序（`progress/T7/additional_gates.tsv:1`）。

对正式 buildconf 执行模式
`SUPPORT_UID_SANDBOXING|uid_app_sandboxing|dev_wos`，grep 退出 1，结论
`OFF_PROVEN`（`progress/T7/off_proven.tsv:1`）。所有自检均 PASS
（`progress/T7/self_check.tsv:1`），配置冻结集 MANIFEST 9/9 通过。
