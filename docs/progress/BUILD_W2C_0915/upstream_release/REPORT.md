# bpftrace-static 与上游 v0.24.2 发布资产的核对

## 结论

**Source1002 中的两个程序，均不等于本次可下载的上游 v0.24.2 Release 二进制。来源仍未确认，不能据此登记为“上游官方发布资产”。**

- 官方三个含二进制的下载项已全部下载、核对大小及 SHA256；两个 bundle 内的程序与独立下载项逐字节同一身份，只有一个 **x86_64 AppImage**。
- Source1002 内是 **armv7l 与 aarch64 的普通 ELF**，与官方 AppImage 外层、内部实际 bpftrace 的 SHA256 均不同；当前该 Release 没有可供同架构配对的 ARM/AArch64 上传资产。
- 三个程序分别实际执行 `--version`，均输出 `bpftrace v0.24.2`、退出 0。官方程序在 x86_64 本机执行；两个 Source1002 程序在 **QEMU 用户态**执行，未使用板子。版本相同不证明源码 revision、生成者或链接方式相同。
- 官方程序是 **AppImage 打包的 glibc + 动态 libstdc++**，不是“整个 bpftrace 静态链接 libstdc++”。这项已确认的上游事实不能移植到两个来源未知的 Source1002 程序。
- 未重建、未改源码/spec、未推送 bpftrace、未变更门禁。其余十包状态不变，查证到此交人工裁决。

结论范围是 **2026-09-15 读取到的指定 Release 资产集合**，不排除其他 GitHub Actions 产物、曾撤回的上传、私人构建或其他站点上的同版本程序。

## 1. 输入身份与下载范围

入口：[官方 v0.24.2 Release](https://github.com/bpftrace/bpftrace/releases/tag/v0.24.2)。
API：[Release 元数据](https://api.github.com/repos/bpftrace/bpftrace/releases/tags/v0.24.2)。
API 标记发布时间为 `2025-12-12T01:58:05Z`，tag 对应 commit：
`0e0c5cfcc7b1093b000cb516a121f21cd37a7143`。
原文分别归档于 `release_v0.24.2.json`、`tag_v0.24.2.json`。

API 列出 5 个上传资产：以下三个含程序的文件，以及 `man.tar.xz`、`tools.tar.xz`。
后两者分别由发布脚本的 manpage 与 `.bt` 脚本打包步骤产生，不是额外架构的
bpftrace ELF，本轮未下载。网页另列两个自动源码归档，不计为发布二进制。
没有因为用户要求“各架构”而猜造 ARM 下载地址。

| 官方下载项 | 字节数 | 独立计算的 SHA256 |
| --- | ---: | --- |
| bpftrace | 194020752 | 654fda127b0c7cd33342de2c10931fff4c9fb5a333b0486985166613dbd84c65 |
| binary_tools_man-bundle.tar.xz | 192687384 | ceeda577efed7c1c087dc93c5cd84ad69223f41bf135c86c4d26feab6f6fd2b7 |
| binary_tools_man-bundle.tar.zst | 193005550 | b4c428920fef9d2f878252607481983fd8456305d0557bb59cc7fc42bb37a590 |

三个文件的实际大小、哈希均与 API 的 size/digest 一致。它证明下载与本次服务器
声明一致，不等于独立签名认证。具体 URL 保存在 `comparison.json`，下载命令和
退出码在 raw/006、008、012。两个 bundle 各有 83 个 tar 条目，各仅有一个
`bin/bpftrace`；该成员大小和哈希均与上表第一行相同。完整条目另存两份
`*.members.json`，没有把压缩容器哈希误当内含程序哈希。

本地源输入为 `codes/R104/bpftrace/packaging/prebuilt-static-bpftrace.tar.gz`。
用户给出的 `3785365b…` 是 **tar 自身**的 SHA256，本轮重新校验如下。

| 对象 | SHA256 |
| --- | --- |
| Source1002 tar | 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123 |
| tar 内 arm/usr/bin/bpftrace | ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b |
| tar 内 aarch64/usr/bin/bpftrace | 4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1 |
| 官方 AppImage 内部 bin/bpftrace | 46f917df57bd9f04525ecb991e49ae3fa61ddf16ee46105b89ee59abfddd9f7a |

tar 成员与此前解出文件的一致性复用 `../static_origin/REPORT.md` 的逐字节核对；
本轮对解出文件重新计算哈希，对 tar 重新计算哈希，均一致。
下载及解包内容仅在 `tmp/BUILD_W2C_0915/upstream-release.yvCwWp/`，不提交大二进制。

## 2. 同版本、不同程序：版本与符号核查

| 对象 | 架构 / 格式 | 实际 --version | ELF 直接依赖摘要 | 符号表 |
| --- | --- | --- | --- | --- |
| 官方外层 | x86_64 AppImage 启动器 | v0.24.2，退出 0 | 外层没有 DT_NEEDED | 普通符号 1763 行；不是内部应用符号 |
| 官方内部 bpftrace | x86_64，8521776 字节 | 由上述 AppImage 启动并报告版本 | libstdc++.so.6、libgcc_s.so.1、libc.so.6 等共 18 项 | 普通定义符号 10396 行；动态符号 1000 行，其中 __cxx11 匹配 260 行 |
| Source1002 armv7l | ARM EABI5，98974296 字节，stripped | v0.24.2，退出 0；QEMU 用户态 | libm.so.6、libc.so.6、ld-linux.so.3 | 普通符号已剥离；动态符号 377 行 |
| Source1002 aarch64 | AArch64，129681256 字节，stripped | v0.24.2，退出 0；QEMU 用户态 | libm.so.6、libc.so.6、ld-linux-aarch64.so.1 | 普通符号已剥离；动态符号 372 行 |

行数口径是 `nm --defined-only` / `nm -D` 的输出行数，后者包含未定义符号，
不是“业务接口数”。完整输出在四组 `*.nm.txt`、`*.dynsym.txt`，ELF 信息在
`*.readelf.txt`。本轮对两个 Source1002 动态符号表检索 `_ZNSt3__1` 与
`__cxx11` 均为 0；此前全文件 strings 的同类零命中记录见 static_origin。
**剥离后的零命中不能证明没有内嵌某套 C++ 标准库。**

实际版本输出均为：

```text
bpftrace v0.24.2
```

成功记录：raw/015、016、027。只执行版本查询，不进行 BPF 加载、追踪或板上验证。
同版本、不同架构和 ELF 依赖/布局说明它们不是从这个官方 x86_64 文件单纯
换 tar 容器得到的同一个程序；但 **不能确定它们是谁用哪棵源码另行构建的**。
当前 Release 不提供同架构对照，因此无法用符号表恢复或确认完整生成来源。

## 3. 官方发布流程与标准库

以下源码全部固定在上述 tag commit，不是今天的 master。
本地原文在 `upstream_source/`，下载 URL 与哈希见 `ci_sources.json`；
`create-assets.sh` 单独下载于 raw/010。

### 3.1 Release 实际走 AppImage

[.github/workflows/release.yml:7–12、24–30](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/.github/workflows/release.yml#L7)
在 release published 时触发，runner 为 ubuntu-latest；构建步骤原文：

```yaml
run: nix develop --command bash -c "OUT=./assets ./scripts/create-assets.sh"
```

[scripts/create-assets.sh:49–57](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/scripts/create-assets.sh#L49)
实际把同一结果放进独立资产和 bundle：

```sh
nix build .#appimage
cp ./result "$OUT/bpftrace"
cp ./result "$TMP/bin/bpftrace"
```

[flake.nix:275–288](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/flake.nix#L275)
中 AppImage 入口原文：

```nix
appimage = nix-appimage.mkappimage.${system} {
  drv = default;
  entrypoint = pkgs.lib.getExe default;
```

同文件 39–40 行默认 LLVM 为 21；149–188 行用 `pkgs.stdenv.mkDerivation`，
依赖列表有 GCC、Clang 及 LLVM 库。**只凭列表不认定实际编译器**；本轮读取
内部程序 `.comment` 实际得到 `GCC: (GNU) 14.3.0`（raw/028）。这是产物中的
编译器标记，不冒充完整逐编译单元的构建日志。

### 3.2 标准库以内部 ELF 与随包库为准

用 `--appimage-offset` 得到 828816，再以 `unsquashfs -processors 1 -offset 828816`
解出载荷（raw/018–019），独立读取内部程序动态依赖，实际包含：

```text
Shared library: [libstdc++.so.6]
Shared library: [libgcc_s.so.1]
Shared library: [libc.so.6]
```

载荷同时有 Nix store 中的 `gcc-14.3.0-lib/lib/libstdc++.so.6.0.33`、
`glibc-2.40-66/lib/libc.so.6`。前者 SHA256 为
`35d1f3cc75f9397c5f537b876991b80a58cc1a207a2ef231628a05319cf64fc4`。
因此官方发布项是**自带动态依赖的 AppImage**，其内部主程序使用 libstdc++ 与
glibc。外层 `file` 显示 statically linked，以及 flake 的 self-contained static
注释，不能替代内部 ELF 核查；不能写成 musl 或静态 libstdc++ 的已证事实。

### 3.3 不混淆另外两条 CI

- [binary.yml:9–17、32–41](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/.github/workflows/binary.yml#L9)
  是 master push 后的 Actions artifact 工作流，有 x86 与 ARM runner，生成
  AppImage；它不是 release.yml 的 GitHub Release 上传。未下载历史 Actions
  artifact，是否与 Source1002 有关联为 NOT_OBSERVED。
- [.github/include/static.sh:14–29](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/.github/include/static.sh#L14)
  是另一个 Docker 静态测试入口，设置 STATIC_LINKING，并检查 musl-x86_64
  依赖，不是上述 Release 流程。
  [src/CMakeLists.txt:92–94](https://github.com/bpftrace/bpftrace/blob/0e0c5cfcc7b1093b000cb516a121f21cd37a7143/src/CMakeLists.txt#L92)
  该路径确有如下代码，但不能用它证明 Source1002 就由此生成：

```cmake
if(STATIC_LINKING)
  target_link_options(bpftrace BEFORE PRIVATE "-static-libgcc" "-static-libstdc++")
endif(STATIC_LINKING)
```

## 4. 回到自行构建评估：缺什么，能做到哪一步

本轮未确认官方 Release 来源；上一轮也未证实 qemu-accel 来源。因此仍不能
将 Source1002 按“已知上游 libstdc++ 资产，登记即可”放行。

可确认有源码与通用静态构建入口，但它采用的 musl/x86_64 路线和 Source1002
的 ARM/AArch64 + glibc 不同；其配套 LLVM18 Docker 输入也不等于资产内的
LLVM21.1.1 版本线索。不能称为原资产的可复现配方。

用 libc++ 自行生成同等功能静态资产在本轮**未实施、未验证**。
要可靠评估，仍缺 Source1002 的原始生成脚本/完整链接命令、源码 revision、
对应 LLVM/BCC 等静态依赖及其标准库、sysroot、strip/打包步骤。
仅更改最后一个 bpftrace 的编译选项不能证明所有静态输入的 C++ ABI 一致。
原构建时长 NOT_AVAILABLE；普通源码四格的耗时不包含未知静态依赖重建，
不能据此估算本资产全链重建耗时。未自行决定替换资产、重建或调整门禁。

此前六份固定镜像清单中 bpftrace-static 均未出现的结论仍仅适用于那六份
历史清单，不推导为所有产品/测试镜像永久不会安装它。详情继续引用
`../static_origin/REPORT.md`，本轮没有改变镜像归属判定。

## 5. 技术性失败、修正与纪律

- light 门禁 raw/001 退出 0。开始 2026-09-15 21:08:36 +08；本轮未进行构建。
  核心查证和自检于 21:24 +08 完成，未触及两小时时限；归档与项目仓交付随后进行。
- AArch64 第一次 QEMU -L 使用现有根，解释器绝对软链接解析失败，退出 255
  （raw/017）；显式调用 loader 的另一种方法退出 139（raw/021）。这些是
  版本查询环境失败，未归因为资产回归或标准库问题。
- 随后在独立临时目录复制现有根的 loader/libc/libm，逐文件校验未改变内容，
  不修改构建根。初次查询缺少库搜索路径，退出 127（raw/026）；加 QEMU
  `-E LD_LIBRARY_PATH=/lib` 后 `--version` 退出 0（raw/027）。此法只是
  查询版本的临时环境，不是产品部署或运行能力验证。
- raw/013 的工具探查曾因不存在的路径返回 2；不作能力判据。
- raw/022 误将 AppRun 当作脚本用 sed 读取，实际是 ELF，输出包含二进制字节；
  原记录保留，后续以 file/readelf 核对，不再显示该输出。没有改写原始记录。
- 自检脚本首跑（raw/029）检查到了尚在执行的自身包装器，因其退出码尚未写入
  而退出 1。保留 v1 快照后，仅将本次正在执行的记录排除于完整性循环，
  其余已结束记录仍逐项检查；这是自检时序修正，不涉及资产比较判据。
  第二次自检（raw/031）因当前 Python 没有 `hashlib.file_digest` 而退出 1；
  保留 v2，改用逐块 SHA256，未改变比较值或判据。
- 只向项目证据目录写入报告、元数据、工具快照；下载、解包与查询环境在 tmp。
  未使用凭据、未用板子、未改 codes。包 HEAD 仍为
  `30e51cd665360f85b665308dc13ba27df0c5f739`；候选 spec 哈希仍为
  `f471f98b0d48dd769de286fbeaa8d7292230fbf316c95b06e03e2b631a86cdd3`。

自行判断限于：按元数据与打包脚本识别含二进制下载项；分别检查 AppImage
两层；使用 QEMU 仅查询版本；把未确认来源与版本一致分开。没有自行判定
Source1002 的标准库、生成者、产品安全性或门禁可放行性。

尚待人工材料：Source1002 是否来自其他发布地址或 Actions 运行？能否提供
下载 URL、生成脚本或构建记录？本轮来源查证完成，包仍暂停推送。

## 6. 复核索引

- `comparison.json` / `sha256_comparison.tsv`：下载身份、容器成员、ELF 比较。
- `release_v0.24.2.json` / `tag_v0.24.2.json`：当次服务端元数据。
- `upstream_source/` / `create-assets.sh` / `ci_sources.json`：固定版本 CI 原文。
- `raw/`、`ci_commands.jsonl`、`comparison_commands.jsonl`：实际命令、输出、退出码。
- `aarch64_version_private_prefix.json`：临时 sysroot 文件哈希与失败的首次运行；
  随后的成功运行在 raw/027，不将二者混为同一次。
- `compare_assets.py`、`check_aarch64_version.py`、`fetch_ci.py`、`run_logged.sh`：
  实际使用的脚本快照，文件清单哈希随归档提供。
- raw/033 自检通过；raw/035 是脚本（含两份失败自检版本）的 SHA256。
