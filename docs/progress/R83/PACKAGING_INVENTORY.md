# sandbox libc++ 打包盘点

## 分支与提交

- Gerrit ref：`refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport`
- 本轮获取 HEAD：`08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`
- 打包提交：`5ed6c77278dfa7a470667cf1a137723d3c96fe60`
- 四层提交依次为 `77e0f73b`、`82afebab`、`9039afa4`、`08a64b6e`。

`packaging: add libc++ runtimes spec` 新增且只新增四个文件：

| 文件 | 用途 |
|---|---|
| `packaging/libcxx-runtimes.spec` | 构建 libc++/libc++abi shared runtimes、头文件与开发链接文件 |
| `packaging/libcxx-llvm22.map` | 将导出符号置于默认 `LLVM_22` version node |
| `packaging/libcxx-runtimes.manifest` | 四个二进制子包共用的安全 manifest |
| `packaging/SOURCE_PROVENANCE` | 源码/导出补丁来源记录；当前内容存在过期问题 |

仓内还保留原 LLVM 打包文件 `llvm.spec`、`llvm.manifest` 和三个 MLGO 模型包。

## spec 形态

`libcxx-runtimes.spec`：版本 `22.1.8-1`，支持 `armv7l aarch64 x86_64`，
BuildRequires 为 `clang = 22.1.8`、cmake、ninja、python3、glibc-devel、
libgcc。配置选择 `libcxxabi;libcxx`，只构建 shared library，使用
libc++abi + libgcc_s，不使用 compiler-rt 或 LLVM libunwind，关闭测试和
benchmark。

四个主要二进制子包是：

| 子包 | 主要内容/依赖 |
|---|---|
| `libc++` | `libc++.so.1*`；精确依赖同 Release 的 `libc++abi` |
| `libc++-devel` | `/usr/include/c++/v1`、链接脚本、experimental archive、modules；依赖 runtime 与 abi-devel |
| `libc++abi` | `libc++abi.so.1*`，自动依赖 libgcc_s/glibc |
| `libc++abi-devel` | `libc++abi.so` linker name；公共 ABI 头由 `libc++-devel` 的共同头目录提供 |

R9C 已用同一 spec 的四补丁前版本在 x86_64、armv7l、aarch64 三架构各
成功生成上述 RPM；完整构建日志每格退出 0。本轮没有重用该事实冒充当前
四补丁产物检查。

## 四补丁是否进入构建

会。checked-in spec 没有 `Patch:` 行，是因为该仓使用 GBS standard
non-native export。R83 对 sandbox HEAD 实际执行 `gbs export`，得到从
`upstream/22.1.8` 到 HEAD 的 10 个顺序补丁。导出 spec 明确写入：

- `Patch6` / `%patch6`：层 A；
- `Patch7` / `%patch7`：层 B；
- `Patch8` / `%patch8`：层 C 头文件批；
- `Patch9` / `%patch9`：层 C 库内批。

因此 `%prep` 在编译前实际应用四层，不需要在 checked-in spec 手写四条
Patch 声明。导出原文与 SHA256 在 `raw/015_*`、`raw/017_*`。

## 层 C 头文件是否安装

会。层 C 头文件提交修改四个安装头：

- `libcxx/include/__ostream/basic_ostream.h`
- `libcxx/include/future`
- `libcxx/include/istream`
- `libcxx/include/string`

这四项均在 `libcxx/include/CMakeLists.txt` 的 `files` 清单。默认
`LIBCXX_INSTALL_HEADERS=ON`；循环把清单逐个安装到
`${CMAKE_INSTALL_INCLUDEDIR}/c++/v1`。spec 未关闭该选项，执行完整
`cmake --install .`，且 `libc++-devel` 的 `%files` 拥有整个
`%{_includedir}/c++/v1`。层 B 的 `cxxabi.h` 也通过共同 ABI header
安装链进入该目录。故随后每个消费者编译取得的头文件包含层 C 防护。

## 完整性结论与缺失

结论分两层：

1. **构建/安装路径完整：能直接构建出 libc++ 与 libc++abi 四个主子包；
   四补丁会实际应用，层 C 头文件会进入 `libc++-devel`。** sandbox 的
   QuickBuild 通过由任务背景给定，但 R83 本机没有平台日志副本。
2. **发布审计元数据不完整：** `SOURCE_PROVENANCE` 仍写
   `export_patch_count=6`、旧 HEAD `3d3e3da4...` 和 “plus six patches”，
   而实际导出为 10 个补丁、HEAD `08a64b6e...`。它不阻止编译，却会把
   错误 provenance 安装进 `/usr/share/doc/libcxx-runtimes/`。发布前必须由
   人工决定是否另提打包修正；本任务未修改。

另一个执行风险是 spec 仍为 `22.1.8-1`。若远端仓已有同 NEVRA 的旧包，
仅靠名称不能证明 buildroot 选中 sandbox 内容。运行手册用 GBS
`--release 83.1`、优先本地仓和 `--use-higher-deps` 消除验证时歧义；这不
改补丁内容。

## 本地试构建处置

`NOT_OBSERVED`。资源门禁本身通过，但准备试构建时主机已有另一项 armv7l
内核 GBS 构建运行，负载约 9–10，多个 QEMU/clang 进程各占满 CPU；近期又
发生过系统崩溃。R83 因此没有叠加第二个 GBS build。只执行了低优先级
`gbs export`，没有执行任何全量编译。资源与进程证据见 `raw/021_*`、
`raw/022_*`。
