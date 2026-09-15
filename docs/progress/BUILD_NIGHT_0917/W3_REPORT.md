# W3：bpftrace-static 候选构建与核查

## 结论：PARTIAL，不可称已可替换发布资产

两架构源码静态构建成功，ABI 和系统依赖检查通过；两条回退路径实际构建及
原资产安装字节对照通过。**功能等价、完整 RPM 写包和 armv7l 尚未闭合**。
没有推送任何包仓，也没有覆盖 Source1002。

| 架构 | 候选字节数 | SHA256 | 精确 std::__1 / std::__cxx11 字符串行数 |
| --- | ---: | --- | ---: |
| x86_64 | 160787128 | `242855842b9b07ed21d3bbb12e590601e7996b88dc29f335aa701fd3c72a5cc7` | 23218 / 0 |
| aarch64 | 151746120 | `0981193d70d2a768ecef078a1980b58185f9d5904469a6de21d6532521561836` | 22910 / 0 |

候选路径为 tmp/NIGHT_0917/builds/bpftrace-<arch>-static/src/bpftrace。
这些文件未做最终 RPM 的 strip 处理；大小不能直接作为正式 RPM 大小预测。

## 输入身份与实际做法

bpftrace 干净 f895f8c0373d224847fc7d3ecbeaac3bf926a1a1，版本 0.24.2。
本轮使用前两阶段新生成的 libc++.a、libc++abi.a、libclang.a；其来源/哈希见各阶段表。
其它 LLVM/Clang 组件与 BCC 归档复用前一夜的 static_packages_v2.json、
static_archive_inventory.json、available_archive_abi.json；并非全部组件在本夜重编。
其 LLVM RPM 为此前测试候选的 2d23367d…-dirty 记录，不冒充提交后重建。
完整旧材料位于 ../BUILD_NIGHT_0916/；新 bpftrace-*-inputs.json 列出本次实际
使用 RPM 的路径与 SHA256。libbpf/zlib/LZMA/ffi 来自当时已下载的
Tizen-Base-Toolchain reference 滚动仓；cereal/vim 使用本次按已观察索引 URL
下载的 RPM。均检查内容 digest，不声称验证签名身份或固定快照全套一致。

所有依赖只解到任务私有前缀；原根及源码只读。先 x86_64 后 aarch64，
Clang 22.1.8、Release、STATIC_LINKING=ON、Ninja -j1。
沿用项目 CMake 的静态组件组合；补充临时 -L 路径、静态 libc++/libc++abi
链接组。没有修改任何上游 C++，也没有通过删除源码功能来规避错误。

链接过程中的三处配置问题均保留失败记录和执行脚本快照：

1. CMake CACHE PATH 先转换相对路径，导致 libclang 路径错位；显式 STRING
   保留源码相对路径后实际解析正确。不是复制旧 21.1.1 归档替代。
2. 首次最终链接找不到裸 -lz；静态库已在私有目录，补相应 -L 后继续。
3. libc++.a 未合并 ABI 归档，链接缺 new/delete/异常类型等定义；nm 确认定义
   在本轮 libc++abi.a 后，显式把两归档放入链接组，最终链接成功。

## 产物核查，而非只看构建退出码

两架构 DT_NEEDED 均为 librt、libdl、libm、libpthread、libc 与对应 ld-linux。
本报告把 librt/libdl/libpthread/动态加载器归入 glibc 家族；原始名字未隐藏。
没有 libc++、libc++abi、libstdc++、LLVM、Clang、BCC、libbpf 或 libgcc_s
共享库依赖。此处“静态”指这些组件静态链入，**不是完全没有动态加载器**。

strings 精确 std::__1 阳性、std::__cxx11 为零，并以 nm -C 的宽 __cxx11
匹配复核为零。bpftrace-*-artifact.json 保留计数、大小、哈希、原始依赖。
完整字符串/符号输出无损 gzip 归档，COMPRESSED_OUTPUTS.tsv 保留压缩前哈希。

link map 的初始“为满足引用而链入的归档成员”段落独立证明新 libclang、
libc++、libc++abi 成员实际进入程序；pulled_members.tsv 与 map 原文压缩件均保留。
虽然构建规则仍列出 -lstdc++fs，两架构实际拉入该归档的成员为 0。
不把“链接清单有一个库名”误当“该库代码实际进入”。

实际静态展开器为 libgcc_eh.a，不是原已验证动态 libgcc_s 路径。
本轮没有复跑静态线程取消/跨库异常行为验证，不能沿用既有动态运行时的完整结论。

## 功能对照及未完成部分

x86_64 原生、aarch64 QEMU 用户态均 --version / --help 返回 0。
aarch64 与原资产同环境对照，这两项输出逐字相同（bpftrace v0.24.2）。
原资产 SHA256 为 4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1。
新旧 --info 都返回 1：要求 root。没有提权、伪装身份或连接离线开发板。
因此支持 probe 的实际集合与内核功能对照为 NOT_OBSERVED，**不是功能通过**。
x86_64 原 tar 无对应程序，不能给它编造同架构原资产基准。

配置报告当前根缺 BFD/opcodes、DWARF、pcap、blazesym 等可选依赖；没有手工
关闭这些开关，但自动检测缺失仍会影响构建功能。是否与原资产一致尚无法证明。
旧程序有权限环境下的 --info 与代表性 probe 结果，及新程序同环境对应结果，
是判断功能等价所缺的材料。不能据 --help 相同就断言可关闭上述功能。

FindLibBcc 的 bcc_usdt_foreach 检查使用 C driver 并未带齐静态传递依赖，
实际失败不是证明该符号不存在。原 CMake 因此选完整 BCC 回退链接；未伪造探测
缓存为成功，最终程序已实际链接。该探测局限单列，不能作为功能裁剪依据。

## 条件化 spec 候选与兼容性

candidate_specs/bpftrace.spec / raw/163 给出完整候选及 diff。
只在既有 _toolchain 已定义且 toolchain_is clang 的路径增加静态构建依赖、
第二个 static 构建目录、静态标准库链接组。保留普通共享构建；保存原 LDFLAGS，
静态构建不继承本包为共享路径加的强制动态 ABI 链接项。
仅 libc++ 路径安装源码生成的 static 程序；GCC 与未定义路径继续 Source1002。
原 tar 声明、解包和 %post static 覆盖逻辑未变。该候选从未推送包仓。

两条回退的展开 spec 与基线忽略空行完全一致，禁止的 libc++ 参数为零。
随后在 aarch64 的真实 GNU 14.2.0 根分别配置、构建全部 CMake 目标，不共用
构建目录；新动态程序依赖 libstdc++，不依赖 libc++/libc++abi。
实际执行候选展开后对应的 Source1002 安装操作，两个安装结果均与原 aarch64
资产逐字节一致，哈希见 fallback_result.json。

这两次是**实际 GCC 主程序构建 + static 资产安装对照**，不是只看宏或复制文件；
但未执行完整 rpmbuild/GBS 写包。源码静态候选的构建采用私有输入路径，候选
spec 使用安装后的 /usr/lib64 路径；完整候选 RPM 构建仍需新静态开发包输入。
脚本、输入清单与哈希允许在当前已备输入的工作区复跑；本轮尚未在空环境中
独立复建，不能称已验证跨机器或字节级可复现。大型根与二进制留在 tmp，不上传。

## 资源与自行判断

并行 1、nice19、ionice3、每进程虚拟内存 16148878 KiB。阶段硬时限未放宽。
初版监控按单次 Ninja 计数；前两次 402 目标短构建没有触发 500 周期探测，
不能声称这些区间已测 I/O 正常。raw/164 补测 0.106 秒；此后每次启动先探测，
再每 500 目标探测，旧脚本快照保留。未发生需暂停十分钟的实测退化。
raw/134 的 rg 模式以 - 开头被误作选项，raw/135 加 -- 重跑；未改变判据。

功能未知仍保留未知，不自行裁决替换；待人工取得功能基准并审阅候选。
armv7l 为 NOT_OBSERVED_BOARD_OFFLINE；新静态展开器行为与完整 RPM 仍待验证。
