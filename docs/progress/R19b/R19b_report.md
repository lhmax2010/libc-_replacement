# R19b：chromium-efl 分析对象来源追溯

## 分析对象

R19 使用的是 R11 下载并校验的 15 个 Chromium 相关二进制 RPM 实例：`chromium-efl`、`chromium-efl-browser`、`chromium-efl-devel`、`chromium-efl-squashfs`、`chromium-efl-tpk` 各含 aarch64、armv7l、x86_64 一份，版本均为 `0:1.1.144-1`。完整 NEVRA、两次 SHA256、绝对本地路径、下载 URL和每包运行时 ELF 数见 `tables/analyzed_rpm_origin.tsv`。15/15 个文件的现场 SHA256 与 R11 台账及 repodata checksum 三方 MATCH。

R19 的 42 个运行时 ELF 实际来自其中 6 个 RPM：三架构 `chromium-efl`（12/15/12 个 ELF）与三架构 `chromium-efl-browser`（各 1 个 ELF）。另外 9 个 RPM 的 `R19_runtime_ELF_count=0`，仍列出是为了完整申报同一源包的相关子包。

## 仓与快照身份

15 个 RPM 全部来自 `unified_binary`，即：

`https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/reference/repos/standard/packages/`

R11 记录的仓组件快照身份为：

- 类型：`COMPOSITE_REPOSITORY_COMPONENT`；R11 明确说明不存在跨 Base/Unified 的单一平台快照 ID；
- Unified binary repomd revision：`1785904307`；
- repomd SHA256：`e425b6a91a3cfb2f67cbd73160ac48e2bb776e92866de81d5b97bb25e5ab39c3`；
- primary metadata SHA256：`17c5337bb36f6df4449b6a4f02b7521aaa9250f388ca6207ea44012dc894c2e3`。

原始快照与 repodata 行见 `r11_unified_snapshot.tsv`、`r11_unified_repodata.tsv`。

## 下载时间口径

R11 的 `binary_download_status.tsv` 没有逐包 wall-clock 字段，故不能补写一个不存在的“日志下载时间”。本报告逐包申报本地文件 birth time 与 mtime：birth time 是本地对象创建时刻，mtime 是 curl 写入完成后的本地时间代理。15 个文件的 mtime 位于 2026-08-06 13:09:27–13:12:46（UTC+08:00）。这些是文件系统实测时间，不冒充服务器时间或独立日志字段。

## 开发板对照

SDB `192.168.108.25:26101` 可达。只读命令 `rpm -qa --qf ... | grep '^chromium-efl'` 退出 0，仅返回：

`chromium-efl-0:1.1.144-1.armv7l`

该 NEVRA 与仓侧 armv7l 主包一致。`chromium-efl-browser/devel/squashfs/tpk` 没有在板端查询结果中出现，记 `NOT_INSTALLED_ON_BOARD`；aarch64/x86_64 与 armv7l 板架构不适用。板端没有读取已安装 payload 的 SHA256，因此字节级同一性为 `NOT_OBSERVED`。

## 适用范围

R19 的 `42/42 ELF DT_NEEDED libstdc++.so.6`、`0/42 DT_NEEDED libc++/libc++abi`、`std::__1=0` 结论，严格适用于 Unified Toolchain standard binary 仓组件 revision `1785904307`、上述 repomd SHA 下的 15 个 RPM 身份及实际分析的 42 个 ELF。

板端仅能申报：已安装的 armv7l `chromium-efl` 主包 NEVRA 与分析对象一致；其余子包未安装，payload 字节身份未验证。因此不把结论无条件扩写为笼统的“平台上所有 Chromium”。

本任务未下载新数据、未构建、未安装或部署任何内容。
