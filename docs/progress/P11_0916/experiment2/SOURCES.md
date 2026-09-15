# 来源索引

## 本地原始材料

- 冻结 ELF 清单与二进制包清单：`INPUT_IDENTITY.json` 记录原路径与 SHA256；`inputs/*.gz` 为逐字节无损快照；`SNAPSHOT_IDENTITY.json` 记录解压后摘要与往返核对。
- 仓地址与原冻结记录位置：`raw/025_snapshot_provenance.stdout`。不是本轮在线仓库的新快照。
- 真实六份 RPM 与十八条 ELF 记录：`SAMPLES.tsv` / `SAMPLES.md`。全部 RPM SHA256 实际重算，ELF 与 RPM 的 SHA256 载荷摘要相符。
- 39,166 行逐项结果：`inputs/SCAN.jsonl.gz`。不提交重复的未压缩大文件；可按普通 gzip 无损解压复核。
- 动态符号版本常量：主机 `/usr/include/elf.h`，摘录在 `raw/013_elf_version_constants.stdout`。实际判定同时使用 readelf 原始表。
- 全部检测程序保留源码，记录器与资源闸门保留快照；SHA256 清单见 `SHA256SUMS`。

## 外部机制依据

| 来源 | URL | 获取与用途 | 边界 |
| --- | --- | --- | --- |
| GNU ld VERSION | https://sourceware.org/binutils/docs/ld/VERSION.html | 网页工具读取成功；shell curl 为 HTTP 403，原退出 22 保留在 `raw/007*`。匿名版本脚本、符号版本定义 | 不能由无命名版本反推没有使用过脚本；不证明 Tizen 历史配置 |
| glibc 2.40 `dl-lookup.c` | https://raw.githubusercontent.com/bminor/glibc/glibc-2.40/elf/dl-lookup.c | `raw/018_glibc_240_lookup.*` 下载成功；旧无版本引用与 dlsym 查找的区别 | 固定上游版本；不等于平台补丁源码或运行验证 |
| glibc master `dl-lookup.c` | https://raw.githubusercontent.com/bminor/glibc/master/elf/dl-lookup.c | `raw/008*`，前期定位用 | 正文选固定 2.40 源码，不把 master 当发布版 |
| Linux man-pages ld.so | https://man7.org/linux/man-pages/man8/ld.so.8.html | `raw/014*` 下载成功；加载路径、预加载与安全模式 | 机制约束，不是本平台拦截实验 |
| Linux man-pages dlsym | https://man7.org/linux/man-pages/man3/dlsym.3.html | `raw/015*` 下载成功；显式句柄搜索与 dlvsym | 不证明本例的句柄在运行时解析到哪个实现 |

正文外部机制均为静态源码/文档核查。没有用上游机制替代真实调用方缺口。
