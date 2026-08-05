# R9 搜索范围

- 仓：`codes/llvm` 当前 `tizen_base`、全部本地/远端 refs、全部 tags，以及平台 VCS 锚 `c12cbe887ff08f04628e1b1165f8d02ce69a9146` 的树和从 `upstream/22.1.8` 到该锚的历史。
- 仓内配置：`.gbs.conf`、`gbs.conf`、`debian/gbp.conf`、`.gbp.conf`；同时检查 `/etc/gbs.conf`、`/home/toolchain/.gbs.conf`、项目根 `gbs_llvm.conf`、`progress/T3R/config/gbs_llvm.conf` 的相关键。
- packaging：`codes/llvm/packaging/` 顶层全部文件；二进制 MLGO tarball 仅列名和 SHA，不展开其内容。
- 平台成品：`tmp/corpus/srpms/llvm-22.1.8-18.1.src.rpm` 的完整条目列表、流式 Source0 SHA、导出的 `llvm.spec` 与 Patch0。
- GBS/GBP：本机 `gbs 2.0.8`、`gbs-export 2.0.8`、`git-buildpackage-rpm 0.9.36+tizen20240605` 的 CLI help 与已安装 Python 源码。仅执行 `gbs export --help`，未执行 export。
- R6：`progress/R6/build_armv7l_git_r3b.full.log`、`build_input_delta.tsv`、`raw/exported_source_sha256.stdout.txt`、`repair_validation_report.md`；并回看 `progress/T3R/build_commands.tsv`。
- 多 spec 先例：T0 语料中 `gcc-14.2.0-1.14.src.rpm` 解包出的 `packaging/pre_checkin.sh`、`gcc.spec` 与四个 `gcc-<arch>.spec`。
- 未找到：平台构建服务器当次完整 export 命令行/外部 profile 覆盖记录；按任务纪律记 `PLATFORM_EXPORT_COMMAND_NOT_FOUND`，未作补全。
