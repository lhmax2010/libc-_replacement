# 低资源续跑结论：PARTIAL

## 结果与尚待裁决之处

LLVM armv7l libc++输入已完成；bcc-tools六格闭合并已推送。
bpftrace四格完整构建、源码程序及实际LLVM/BCC提供方检查均通过，
但包内另有未经重编的预编译static资产。它的内嵌C++标准库未观测，
推送门禁范围尚待人工确认。因此本轮不提交、不推送bpftrace，
总状态为10/11已完成并推送，而非11/11。

## 一、恢复前实际检查与资源

原构建树内实际执行CMake配置检查，内外层退出码均0（raw/045）。
实际输出如下，完整编译命令保存在build-w2c-confirm-config.build.log：

```text
FIXED_PATH=/bin:/usr/bin:/sbin:/usr/sbin
CMAKE_CXX_COMPILER:UNINITIALIZED=/bin/armv7l-tizen-linux-gnueabi-clang++
clang version 22.1.8
set(CMAKE_CXX_COMPILER_ID "Clang")
set(CMAKE_CXX_COMPILER_VERSION "22.1.8")
LLVM_ENABLE_LIBCXX:BOOL=ON
COMPILER_AND_LIBCXX_CONFIG_CHECK=PASS
```

生成的实际编译命令含`-stdlib=libc++`。配置、续建脚本均写死
`PATH=/bin:/usr/bin:/sbin:/usr/sbin`，不依赖交互shell。

全部新增构建保持jobs=1、nice=19、ionice class=3，未放宽门禁。
systemd用户scope：总内存32297756 KiB，请求MemoryMax为
16536451072字节，实际memory.max按页取整为16536449024字节。
已在运行中核查scope后代的实际优先级及限额，见raw/050、097、105、
131、137、147、152，不仅是脚本声明。

每500目标探测一次；LLVM共执行14次（500至7000），全部通过。
早期追加三次读写检查约20.45、20.58、18.90秒，均低于30秒，未冻结。
本轮没有触发10分钟暂停：连续三次暂停后的行为为NOT_OBSERVED；
独立freezer校准与早期修正记录保留。bcc/bpftrace各次目标数未达到500，
不虚构其“每500目标”额外探测。

计时从2026-09-15 15:51:49 +08重新开始，最后构建于20:20:24完成，
构建阶段4小时28分35秒，早于十二小时截止。报告、归档收尾时间另留日志。

## 二、LLVM输入及失误造成的重编

仅重建armv7l；未重建aarch64/x86_64。LLVM构建15:53:38至19:10:03，
11785秒（3小时16分25秒），Ninja7146/7146，内外层退出0，写出22个RPM。
12个非调试RPM用于本地输入，SHA256见llvm_armv7l_rpms.tsv。
最终RPM中的libLLVM.so.22.1依赖libc++.so.1、libc++abi.so.1、libgcc_s.so.1，
不依赖libstdc++.so.6；动态符号反修饰输出中std::__1为5646行、
std::__cxx11为0行。这是该ELF动态符号行计数，不是整个资产符号数。
证据：raw/089–090、llvm_input_verification.json及readelf原文。
本轮没有重跑已通过的MLGO运行时验证，也没有新推送LLVM。

此前错误由我启动续建时未固定PATH引起：/bin编译器路径被替换为/usr/bin，
CMake缓存重置后libc++开关关闭、构建配置偏离。发现后停止，恢复并取得
人工确认后才重跑；错误不能改写为机器故障。历史REPORT.md原样保留。

实测重编统计（raw/085–087，Ninja前后快照与统计脚本均归档）：

- 原801个完成目标中，800个在本次再次执行；唯一未同标签再次执行的是
  `Generating VCSRevision.h`。
- 其中745个编译对象任务耗时合计1262.828秒（约21.047分钟）。
- 此前错误配置尝试耗时254.391秒；配置恢复两次主要命令分别143秒、90秒；
  本次启动前配置检查20秒。各阶段原始起止与退出码见timing_summary.json。
- 21.047分钟只是已识别重复编译对象的记录耗时，不包含所有生成、链接和
  恢复工作，不能当作“若无失误本可节省”的精确墙钟差值；净增时间未能实测。

## 三、bcc-tools六格与推送

候选仅修改packaging/bcc-tools.spec，新增15行，未改上游C++。
spec SHA256为42ea7cc9ca6133c3f71594334718636b5cd6aaf68328c18dcd9016fcb1eed7bc，
与此前四格验证对象一致。aarch64/x86_64四格沿用
../BUILD_W2_0912/checks/下记录，不重复构建。

本轮armv7l libc++实际Clang22.1.8、GCC实际GNU14.2.0，均完整写包。
每格对同一批RPM重复核查5次：libc++格两个运行时ELF依赖libc++、
GCC格两个ELF依赖libstdc++；均无另一套标准库直接依赖。
GCC完整构建日志中`-stdlib=libc++`和`-lc++abi`均0。
实际构建根libLLVM两侧分别匹配对应标准库。
五次核查不是五次独立构建，不证明内核功能测试已完成。
完整矩阵及证据路径见MATRIX.tsv、bcc_gate.json、checks/。

推送提交：`5bd0654e24bb231035ea684d278f674aa95a2cd2`。
父提交：`dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2`。
目标：`ssh://lhmax2025@review.tizen.org:29418/platform/upstream/bcc`，
`sandbox/lhmax2025/libcxx-migration`。
作者及提交者为hao.lin <hao.lin@samsung.com>；英文正文加中文说明，
不含内部任务号。普通推送退出0，无force；推送前后45个远端分支仅目标
改变，其他44个逐项相同，无增删，原HEAD是新提交直接父提交。
raw/110–117保存全过程。服务端标题长度提醒不影响推送成功，未改写提交。

## 四、bpftrace四格及预编译子包未闭合项

候选基于`30e51cd665360f85b665308dc13ba27df0c5f739`，仅spec新增15行；
分支为既有libcxx-migration，未改源码或预编译资产。
候选、diff、SHA256见bpftrace_candidate.spec及raw/128。

armv7l/aarch64 × libc++/GCC四格均完整构建并写出3个非调试RPM。
Clang均22.1.8，GCC均14.2.0。对源码构建的`bpftrace`、`bpftrace-aotrt`
分别读取最终RPM内ELF：libc++格依赖libc++且不依赖libstdc++，GCC格反之。
实际构建根libLLVM与libbcc的标准库亦逐个匹配；GCC日志禁止参数为0。
见raw/129、134、145、150及验证raw/132、143、148、154。
x86_64不在ExclusiveArch，不列为未通过格。

输入BCC为本轮已推送候选；aarch64复用此前同一候选内容的RPM，armv7l
使用本轮新RPM，逐个哈希见bcc_inputs_for_bpftrace.json、raw/123–127。
不能把“推送后使用同候选RPM”写成“每架构在提交后重新构建”。

必须单列的另一路径：原spec的Source1002引用预编译tar；`%install`
复制其架构二进制为bpftrace-static，`%post static`又复制覆盖
`/usr/bin/bpftrace`。源码程序正确并不保证安装static子包后的程序
就是本轮编出的那个。原spec第9、84、86–88行及原文见raw/119；
候选新增15行后的行号不同，完整前后spec均已保留。

资产SHA256：3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123。
两架构二进制已strip，.comment不可得；动态依赖只见glibc/libm等C库。
这不能证明内嵌的是哪套C++标准库，也不能证明不存在C++代码。
四格RPM中的预编译文件均与对应原文件逐字节一致，未执行这些二进制。
其内嵌标准库记NOT_OBSERVED，不把无DT_NEEDED误写成“没有标准库”。

已向人工提出两个门禁口径，尚未获得裁决：

1. 是否允许源码四格通过后推送，预编译static子包明确单列未验证；
2. 还是必须补齐预编译资产的标准库/ABI证据后，整个包才允许推送。

本轮不自行选定。bpftrace未提交、未推送，候选和四格RPM留在本地。
建前与收尾的17个远端分支逐项相同（raw/121、156及final_selfcheck.json）。
需要资产生成记录、未strip版本/链接信息，或由人工明确本次推送范围。

## 五、兼容性交叉核对与输入范围

11个spec逐项条件栈与原文见guard_context_audit.json，方法与限制见
COMPATIBILITY_REVIEW.md。新增字面libc++参数/依赖均在共同Clang条件内，
工具用4个正反样本校准；不将文本检查当作任意RPM宏的求值证明。
本轮实际GCC构建证据与沿用历史记录分开列出。
tensorflow2的armv7l/GCC是经人工批准的“与基线同点同因失败”判据，
不是六格全部构建成功。libsigc++也不强求保留libc++动态依赖。

本轮GBS公共输入来自Tizen-Base-Toolchain的reference滚动仓：
https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/ 。
配置、实际安装包和构建日志归档；不可冒充固定日期快照的完全重现。
本地LLVM、BCC输入独立列出RPM及SHA256；未混入Tizen-Base的数据身份。
GCC配置中的历史libc++开发包支持仓用于处理依赖预解析；不据开发包
是否安装判定编译器选库，依据实际命令和最终ELF。

构建可选依赖检测提示（如libbfd、libopcodes、libdw、libpcap、libblazesym、
asciidoctor）保留在日志，未自行补装或改源码；本轮未证明所有可选功能
与其他环境一致。没有内核跟踪功能、物理板或板上性能实测。
未触碰运行时线，未占用开发板。没有清理用户/abuild遗留文件或提权。
七个本轮构建scope收尾均inactive；磁盘剩余278958640 KiB（约266 GiB）。
保留构建根和RPM供后续核查，不将“进程已退出”写成“文件已清空”。

## 六、交付与自行判断

当前矩阵、11包状态、兼容性核对及全部命令/退出码分别见MATRIX.*、
PACKAGE_STATUS.*、COMPATIBILITY_REVIEW.md、raw/及各commands.jsonl。
脚本源文件与SHA256清单随材料归档；旧错误尝试原始输出不改写。
REPORT.md与DELIVERY.md为首次停下时的历史记录；当前结论以本文件为准。

自行判断包括：将重复核查与独立构建分开；将未知预编译子包范围
单列而非默认为已迁移；保留候选等待门禁裁决；复用已验证且哈希一致
的输入而非重新构建已通过架构。未修改判据、未推测未知资产的标准库。
待审问题就是上述static子包推送范围；未据四格源码PASS自动推送。
