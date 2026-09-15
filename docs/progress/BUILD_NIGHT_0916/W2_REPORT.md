# W2：自行构建 bpftrace-static 的可行性评估

## 结论与 W3 门禁

**评估完成，但当前输入不满足“无明显阻断”；按任务第 11.b 条，W3 记为
`NOT_STARTED`，未启动静态构建、未替换资产。** 不是“静态构建已失败”，
也不是“源码无法静态构建”，更不是“LLVM 完全不提供静态库”。

已核实的核心缺口是：三架构现有匹配工具链输入均没有独立的 `libclang.a`，
也没有 `libc++.a`、`libc++abi.a`。后两者在当前 libcxx-runtimes 打包配置中
明确关闭静态构建/安装。另一个静态构建分支虽提供 libclang 预编译归档，
但标为 21.1.1，且实际引用 `std::__cxx11`，不能当作本轮 Clang/LLVM 22.1.8 +
libc++ 的现成输入。

相反，LLVM/Clang 组件归档、三个架构的 libc++ 版 BCC 归档已经存在；libbpf、
zlib、LZMA 等静态 RPM 也确实可以下载。这些不列为平台缺失。
完整静态链接闭包与功能等价尚未形成，不能将“多数依赖已在”写成可以闭合 W3。

这是一项**当前构建输入缺口**，不是永久技术限制。需要另行准备匹配的核心
静态库、完成静态依赖闭包及功能基准，再确定可执行的构建计划。本轮没有
把“估时不明”说成已实测超过 8 小时，也没有据不完整输入启动整套重建。

## 1. 数据身份和已做的检查

- bpftrace 版本 0.24.2；本轮已推源码适配为 `f895f8c0373d224847fc7d3ecbeaac3bf926a1a1`，
  其上游 C++/CMake 与父提交 `30e51cd665360f85b665308dc13ba27df0c5f739` 相同。
- 三个已存在构建根：armv7l/aarch64 使用上一轮 bpftrace-libcxx 根；x86_64
  使用此前 bcc-libcxx 根。完整绝对路径在 `static_archive_inventory.json`。
  **x86_64 根不是 bpftrace 已构建根**，其包的 ExclusiveArch 仍只有 armv7l/aarch64。
- 用 RPM 数据库查询各根已装包的版本、架构、VCS 和 payload 清单。不是只看
  spec 里有没有 BuildRequires，也不因“未安装 static 子包”就判平台不存在归档。
- armv7l LLVM RPM 的 VCS 是 `f203923a…`；aarch64/x86_64 LLVM RPM 是此前经
  同候选内容验证的 `2d23367d…-dirty` 构建记录，不能改写为提交后重新构建。
  libc++/libc++abi 开发包 VCS 为 `08a64b6e…`；这批开发包不提供静态标准库。
- 在线查询三个架构的 **Tizen-Base-Toolchain reference 滚动仓**，不是 Tizen-Base。
  URL 为 `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/<arch>/`。
  索引及其 SHA256、实际选中的 RPM URL/版本/VCS/哈希见
  `repository_static_packages.json`，不得与先前固定快照或本地候选 RPM 混为同批。
- 逐架构下载 5 个相关开发/静态 RPM，共 15 个；检查 RPM 内部 digest、实际
  文件清单。不验证签名身份，不安装包，不修改构建根。原 RPM 在临时目录，
  归档只保存索引、元数据、哈希与查询输出。

本轮所有命令受 nice=19、ionice class=3 和 50% 虚拟内存限额约束；无并行构建。

## 2. 上游静态构建机制

[上游 v0.24.2 顶层 CMake](https://github.com/bpftrace/bpftrace/blob/v0.24.2/CMakeLists.txt#L15)
定义 `STATIC_LINKING`；开启后优先查找 `.a`，并设置静态链接搜索起止策略。
这是构建选项，不是发布资产来源的证明。

[上游静态 CI 脚本](https://github.com/bpftrace/bpftrace/blob/v0.24.2/.github/include/static.sh#L20)
实际设置 `-DSTATIC_LINKING=ON`，并在构建后检查其仅剩 musl C 库动态依赖。
该 CI 的 Docker 输入是 Alpine/LLVM18，检查对象是 x86_64；本轮没有照抄其
`-j$(nproc)`，也没有执行 Docker、ldd 或改宿主环境。它不等于本任务要求的
glibc + libc++ 方案，也不是之前核对的 Release AppImage 工作流。

本平台当前代码的关键规则（完整带行号输出见 raw/024–025）：

```cmake
# CMakeLists.txt:81–85
if(STATIC_LINKING)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_LINK_SEARCH_START_STATIC TRUE)
  set(CMAKE_LINK_SEARCH_END_STATIC TRUE)
endif(STATIC_LINKING)
```

`src/ast/CMakeLists.txt:58–90` 在静态路径选择 LLVM 组件、libclang_static 或
LIBCLANG_STATIC_PATH，并处理其向共享 LLVM/clang-cpp 的传递依赖。
`src/CMakeLists.txt:141–179` 额外链接 BCC loader、libbpf、ELF、LZMA，以及
启用 DWARF 时的 bzip2/EBL 等库；199–207 行将系统 C 库与 zlib 的链接策略分开。

还有必须在真正构建中核对的细节：

- `src/CMakeLists.txt:92–94` 添加 `-static-libgcc`、`-static-libstdc++`。
  **不能单凭选项名字断言 Clang 最终选择哪套标准库**；需结合 `-stdlib=libc++`
  的实际 driver 链接命令及最终产物核验。本轮没有把该项测试写成已执行。
- 同文件 187–192 行在 Clang 路径仍添加 `stdc++fs`；是否拉入成员必须查看真实
  链接，不将该旧注释视为所有 Clang 版本都需要它的证明。
- blazesym 处有“未完整遵从 STATIC_LINKING”的 TODO，systemd/pcap/DWARF/BFD
  均有可选条件。不能为了少编依赖而自行关闭可能影响原功能的选项。

## 3. 静态依赖清单与实际可得性

`static_dependencies.tsv` 是**初次构建根库存**，`NOT_AVAILABLE_IN_ROOT` 不等于
平台没有。后续 RPM 核对补全于下表、`static_packages_v2.json` 与
`repository_static_packages.json`。LLVM/Clang 全部组件文件名及大小也保存在
`static_archive_inventory.json`，没有只列聚合计数而丢失明细。

| 库/依赖组 | armv7l | aarch64 | x86_64 | 依据与限制 |
| --- | --- | --- | --- | --- |
| LLVM 静态组件 | 114 个 .a | 115 个 .a | 128 个 .a | llvm-static-devel 已装且实际存在；架构目标集不同，数量不能互换 |
| Clang 静态组件 | 89 个 .a | 89 个 .a | 89 个 .a | 实际文件存在；不包含独立 libclang.a |
| 独立 libclang.a / libclang_static | 缺匹配输入 | 缺匹配输入 | 缺匹配输入 | 当前根、llvm-static-devel/clang-devel 清单与导出目标均无该独立目标 |
| libbcc.a / libbcc_bpf.a / libbcc-loader-static.a | 本地 RPM 有 | 本地 RPM 有 | 本地 RPM 有 | 不是 devel 子包，而是已构建的 bcc-tools-static；解出各 3 个归档核验 |
| libbpf.a | 官方 static RPM 有 | 有 | 有 | 下载 libbpf-static，payload 逐项核对 |
| libelf.a | 根中有 | 有 | 有 | 该归档的完整传递依赖仍需最终链接确认 |
| libz.a | 官方 static RPM 有 | 有 | 有 | zlib-devel-static，不在初始根并非平台缺失 |
| liblzma.a | 官方 static RPM 有 | 有 | 有 | liblzma-static，payload 核对 |
| libc++.a | 缺 | 缺 | 缺 | 当前运行时配置明确关闭 STATIC/INSTALL_STATIC；libc++experimental.a 不能替代 |
| libc++abi.a | 缺 | 缺 | 缺 | 同上，独立 libc++abi 静态构建/安装为 OFF |
| libgcc_eh.a | 根中有 | 有 | 有 | 若按仅余 glibc/libm 目标选择静态展开器，还需验证异常/取消兼容性；本轮未选定额外运行时方案 |
| libxml2.a | 未取得 | 未取得 | 未取得 | LLVM 导出目标存在 LibXml2 依赖；根中无，下载的 libxml2-devel 也无 .a；是否进入所选静态闭包尚待完整配置 |
| libffi.a | 官方 static RPM 有 | 有 | 有 | libffi-devel-static；是否实际需要取决于 LLVM 配置 |
| libzstd/libedit/terminfo | 根中未取得 | 同左 | 同左 | 条件/传递依赖；本轮未将可选全功能静态闭包求值完成，不宣称平台绝对不存在 |
| BFD/opcodes/iberty/sframe | 根中未取得 | 同左 | 同左 | 反汇编及其条件依赖；需依据功能基准决定所需集合，未擅自关闭 |
| DWARF/EBL/bzip2 | 根中未取得 | 同左 | 同左 | DWARF 及其条件依赖，同上 |
| pcap/blazesym/systemd | 根中未取得 | 同左 | 同左 | skb_output、符号化及可选 systemd 功能；是否必须保留尚缺原资产 --info 基准 |
| cereal | 头文件依赖 | 同左 | 同左 | CMake 中的头文件载体，不要求独立 .a |
| Bison/Flex/xxd/CMake 等 | 构建工具 | 同左 | 同左 | 不是需要静态链入最终 bpftrace 的库 |

这份清单覆盖已读构建逻辑中的直接库与已识别的条件/传递库组；**未产出成功
配置后的全量链接命令，因此不能声称已穷尽最终的所有归档成员和系统依赖**。
实际闭包还受功能开关、LLVM 构建选项、导出目标的传递关系影响。

### 已有归档的 ABI 抽核

对三个 BCC 归档及 LLVMSupport 各架构共 12 个归档执行 nm/objdump；全部
观察到 `St3__1` 形式的未定义符号，`__cxx11` 行数为 0。逐项哈希和计数见
`available_archive_abi.json`，完整原始符号在 `available_archive_commands.jsonl`。
这是这些归档的正面 libc++ 符号证据，**不是所有 LLVM/Clang 组件的完整 ABI
审计或最终静态链接验证**。

### 独立 libclang.a 为何没有

平台 LLVM 的 `%files static-devel` 只安装已生成的 `lib*.a`，不会自动生成
未开启的目标（spec:543–546）。LLVM 源码
`clang/tools/libclang/CMakeLists.txt:84–85` 明确：

```cmake
option(LIBCLANG_BUILD_STATIC
  "Build libclang as a static library (in addition to a shared one)" OFF)
```

同文件 115–116、135–157 行控制静态目标创建。准备本轮独立 libclang 静态
输入需启用该目标并以匹配的 LLVM/Clang 版本和 libc++ 编译；本轮只读了这些
规则，未在旧构建树上重新配置，也未声称只需复制别的 Clang 组件归档就能替代。

`libcxx-runtimes.spec:94–105` 的实际配置包括：

```text
-DLIBCXX_ENABLE_STATIC=OFF
-DLIBCXX_INSTALL_STATIC_LIBRARY=OFF
-DLIBCXXABI_ENABLE_STATIC=OFF
-DLIBCXXABI_INSTALL_STATIC_LIBRARY=OFF
```

不能从“共享运行时已验证”直接推导新静态运行时的展开器与取消行为已验证。

## 4. 新找到的静态构建分支（只读线索）

W1 的远端列表已有 `sandbox/hoegeun/static-build`，本轮只在临时仓 fetch 该
既有分支到本地，未改/推它，也未改变 codes。
revision：`624f68d5f67a673083e5ad08ea205742613d6e53`，摘要
`spec: enable FORCE_STATIC_LIBBPF for static package build`。

其 spec 已请求 `bcc-tools-static`、`libbpf-static`、`liblzma-static`、
`zlib-devel-static`，并配置 STATIC_LINKING 与 FORCE_STATIC_LIBBPF。
但它的 Source1002 是另一个 **prebuilt.tar.gz**，内含 libclang 归档，不是
当前 Source1002 的两个完整 bpftrace 程序。**这没有确认原二进制的来源**。

| 文件 | SHA256 | 未定义 __cxx11 符号行数 |
| --- | --- | ---: |
| prebuilt/usr/lib64/libclang.a.21.1.1 | cb5958fa3c575430a9e033d9d24dc49311836693f6f9f295c93b5a18963fa9b8 | 31 |
| prebuilt/usr/lib/libclang.a.21.1.1 | bceccaf5ef0d5fd44478b93888dd5630f9389408474651159d114a2fccb317d5 | 30 |

两者 `St3__1` 匹配为 0，普通 ar 成员有可读符号。这是明确的 libstdc++ ABI
耦合证据，不因文件版本同为 bpftrace 0.24.2 就认为兼容本轮 libc++。
目录有 `libclang.a` 指向带版本文件的软链接，没有 x86_64 对应目录。

与当前基线树对照，该分支还改变 `src/ast/passes/clang_build.cpp`（4 行删除）
及 CMake。本任务禁止改上游 C++，所以**未整体套用该分支**。
其 `%install` 注释提到通过工具“force static linking”，实际只是查依赖后
删除 RPATH 的命令；移除搜索路径不能把共享库代码补入 ELF，本轮未采用或执行。

fetch 使用 depth=1；`git show --stat` 因浅边界显示整树，不把那份整树统计
误当“这个提交新增了全部文件”。实际对照使用两棵 tree 的 git diff（raw/029）。

## 5. 原资产验收基准：已取得与不可得部分

原两架构程序的哈希、历史版本、Clang/LLVM21.1.1 字串见 KNOWN_ITEMS.md。
本轮只对 AArch64 用 QEMU 用户态版本查询环境，不使用物理板、不伪装 root：

| 架构/项目 | 结果 | 可证明的范围 |
| --- | --- | --- |
| aarch64 --version | `bpftrace v0.24.2`，退出 0（raw/036） | 版本字串，不是内嵌标准库证明 |
| aarch64 --help | 退出 0（raw/037），全文保留 | 命令界面，不等于各 probe 能成功工作 |
| aarch64 --info | 退出 1（raw/031）：`ERROR: bpftrace currently only supports running as the root user.` | NOT_AVAILABLE：缺足够权限；没有推导功能支持与否 |
| aarch64 probe 类型的实际可用集合 | NOT_OBSERVED | --info 受权限门禁，未挂接探针；QEMU 用户态也不能冒充设备内核能力 |
| armv7l 本轮板上版本/info/probe 核查 | NOT_OBSERVED_BOARD_OFFLINE | 不等待、不连板；历史 QEMU --version 成功只作为历史证据复用 |
| x86_64 原 Source1002 基准 | NOT_AVAILABLE | 原 tar 根本不含 x86_64；不能用上游 AppImage 冒充原资产同架构基准 |

本平台 `src/main.cpp:464–467` 在调用 info 前实际执行 check_is_root，
与上述失败一致。spec 的说明列举 kprobe/uprobe/tracepoint，是产品说明，
不是本轮实际验证的支持列表。没有把 help 示例或 strings 命中转成 probe PASS。

补全功能等价需要：原资产在有权限且内核条件明确的环境中的 `--info` 与
代表性 probe 结果，以及新产物在同等条件下的对照。不能在缺这组基准时
默认删掉 BFD/DWARF/pcap 等可选功能仍“同等功能”。

## 6. 耗时、体积和风险估算的边界

| 架构 | 可引用的既有尺度 | 新 static 全流程耗时 / 体积 |
| --- | --- | --- |
| armv7l | 上轮同低资源限制 LLVM 完整构建 11785 秒（3 小时16分25秒）；原 static ELF 98974296 字节 | NOT_AVAILABLE：还需准备核心静态库和最终闭包；板上验证本轮离线 |
| aarch64 | 现有 LLVM/BCC libc++ 组件可用；原 static ELF 129681256 字节 | NOT_AVAILABLE：没有匹配全流程的已测计时或新链接产物 |
| x86_64 | 现有 LLVM/BCC libc++ 组件可用；原 tar 无该架构 | NOT_AVAILABLE：无同架构原资产尺寸基准，且缺核心静态输入 |

已有完整 LLVM 的时间 **不是** 额外构建 libclang_static 单目标的时间，不能
据它给三架构各填相同小时数。LLVM 源码列出独立 libclang 的源文件与依赖目标，
但未实际生成新构建图，不能把文件数当准确构建目标数或时间。
已有源程序动态构建也不包含静态标准库、缺失传递库、静态链接及功能核查成本。
因此本轮不给伪精确的八小时保证或新产物 MB 数；原 ELF 字节数仅作参考，不是预测。

主要风险和仍需核对的内容：

1. 不匹配的预编译 libclang/LLVM/标准库会重现跨运行时 ABI 耦合，不能混用。
2. CMake 静态标志不自动消除全部共享依赖；Clang 导出目标、XML等传递依赖和
   可选库都要从真实链接闭包核实。
3. 只剩 glibc/libm 的要求还涉及展开器链接方式，不等于既有 libgcc_s 动态路径
   的行为已经继承；不能仅凭标准库命名空间判异常/取消行为通过。
4. `strings` 的指定前缀是辅助证据，strip 后无命中不能证明没有另一套标准库；
   正式验收还需未剥离符号/链接输入记录与 ELF 依赖核查，本轮未伪造这些结果。
5. 缺原功能基准与 root/内核条件，不能证明新旧工具功能等价。

## 7. 执行修正、自行判断与交接

- raw/024 曾按错误路径读取 `src/ast/passes/CMakeLists.txt`；该文件不存在，
  管道最后的 head 返回 0，不能把包装器 0 当作该文件读取成功。随后用正确的
  `src/ast/CMakeLists.txt` 重读并落盘（raw/025），未改判据或源码。
- 另分支输入第一次仅按 `.a` 结尾选择符号检查，漏掉 `.a.21.1.1`；首轮只
  输出元数据、**未据此写零符号结论**。保存 v1 后改按实际 ar 类型识别，
  v2 才执行两份归档 nm/objdump，正面检出 31/30 行（raw/033–035）。
- 没有做权限绕过，没有尝试 sudo、凭据猜测或对 --info 的身份伪装。
- 本轮读取新线索、下载只读依赖并抽查归档；没有在任何旧构建树上改配置，
  没有在 codes 写文件，没有执行新构建，更没有替换或推送 static 候选。

自行判断：以固定受测本地 RPM 为 C++ 输入，以在线 reference 的小型 C 静态
RPM 验证可得性，并明确分开数据身份；将根缺包、RPM可得、ABI不匹配和核心
目标未生成分为不同问题；将未形成的静态闭包认定为当前明确阻断，按人工的
第 11.b 条不启动 W3。没有据此认定静态版本永久不可行。

下一步需要的材料/准备：匹配版本与 libc++ ABI 的 libclang.a、libc++.a、
libc++abi.a；完成传递静态库清单（含 XML 条件依赖）；有权限的原资产功能
基准。当前适配分支的 Source1002、安装/覆盖逻辑均未改变。

复核材料：`static_dependencies.tsv`、`static_archive_inventory.json`、
`static_packages_v2.json`、`repository_static_packages.json`、
`available_archive_abi.json`，对应原文命令与输出在 raw/ 和各 commands.jsonl。
