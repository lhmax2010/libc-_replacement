# gcc 多 spec 机制要点

## 取证身份

- 源码树：`codes/gcc`
- 分支：`tizen_base`
- HEAD：`49c5060508ce8a6ed6beca375233688a6bdd9140`
- 工作树：取证时 `git status --porcelain` 零输出
- 完整身份与退出码：`commands/00_project_and_source_identity.txt`

## packaging 文件组织

`codes/gcc/packaging/` 顶层共有 11 个普通文件：

- spec：`gcc.spec`、`gcc-armv7l.spec`、`gcc-armv7hl.spec`、`gcc-aarch64.spec`、`gcc-riscv64.spec`
- 生成脚本：`pre_checkin.sh`
- manifest：`gcc.manifest`
- 同目录源码输入：`gmp-6.2.1.tar.bz2`、`isl-0.24.tar.bz2`、`mpc-1.2.1.tar.gz`、`mpfr-4.1.0.tar.bz2`

完整文件清单、大小与 SHA256 见 `commands/01_gcc_packaging_inventory.txt`。

## spec 与源码包名

五份 spec 的共同 Name 原文均为：

```spec
Name:          gcc%{?cross:-%{cross}}
```

基准 `gcc.spec` 没有在文件首部定义 `cross`，故 Name 为 `gcc`。四份派生 spec 分别在完整基准正文前增加：

```spec
%define cross <arch>
%define <arch> 1
```

因此 Name 分别展开为 `gcc-armv7l`、`gcc-armv7hl`、`gcc-aarch64`、`gcc-riscv64`。Name、Version、Source 原文的逐文件行号见 `commands/02_gcc_spec_identity.txt`。

## 生成脚本与关系

`pre_checkin.sh` 的完整 14 行原文见 `commands/03_gcc_pre_checkin_full.txt`。它循环四个架构，写入上述两行宏和一个空行，再把 `gcc.spec` 全文拼接到派生文件。逐个删除派生文件前三行后，与 `gcc.spec` 的 `diff -u` 均退出 0，见 `commands/05_gcc_generated_relation.txt`。所以派生 spec 是可独立提交和消费的全量文件，不是运行时 include，也不是仅保存前缀的片段。

## QuickBuild/OBS 配套文件事实

在 `codes/gcc/packaging/` 顶层未见 `_multibuild`、`_service`、XML 或 include 配置；完整搜索原文见 `commands/04_gcc_multibuild_aux_search.txt`。因此本仓 packaging 内可观察到的机制由“多个完整 `.spec` 文件 + 可重复生成它们的 `pre_checkin.sh`”构成，未见额外 QuickBuild/OBS 配置。此结论仅覆盖仓内可见文件，不推断服务端隐藏配置。

## 对 R18 实现的约束

1. 多个构建目标以多个完整 spec 并存呈现。
2. 每个目标必须产生不同的 Name。
3. 生成脚本仅在派生文件能由同一正文加机械前缀确定时成立。
4. 包专属 manifest、tarball 等辅助输入与 spec 同置于 `packaging/`。
5. gcc 仓内没有可直接照搬的 `_multibuild` 或 `_service` 文件。
