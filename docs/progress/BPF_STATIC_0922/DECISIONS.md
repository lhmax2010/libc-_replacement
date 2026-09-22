# 判断、边界与待办

- 本轮是新配方整轮写包：临时 spec 只增加 Patch0 与 `%prep` 中无条件 `%patch0 -p1`；不复用上轮 ELF。CMake 改动确在 STATIC_LINKING 与 LIBCLANG_STATIC_PATH 嵌套分支，普通主包关闭 STATIC_LINKING，GCC/未定义路径不构建 static，故沿用人工批准的无条件应用，不加开关。
- 两架构都使用前轮原隔离 argv/输入根；仅将 /tmp/task 与 SOURCES 绑定指向本轮新目录，追加 Patch 文件。前轮两格实际 payload 都为 w5.xzdio，本轮保持，并分别在隔离环境预检。原 spec、来源 tar 与历史产物保留。
- 本轮用户明确允许跨工作窗口续跑，没有新总时限；不沿用已经结束的前轮截止时刻。串行、nice19、ionice idle、cgroup MemoryMax=16536457216 字节（50%）、I/O 门禁不变。每小时状态和 CONTINUE 记录断点；任何失败保留现场，不覆盖重试。
- `-lstdc++fs` 来自 bpftrace `src/CMakeLists.txt:188-191` 的 Clang/旧 GCC 文件系统支持分支，不是 spec 直接添加。实际解析文件待以 linker 或 driver 实测；不删除、不替换。W4 待办。
- raw/014 的 diff 退出1表示存在预期两处新增；raw/016 的退出1来自进程匹配无命中，不是 payload/spec 预检失败。raw/013 一个历史路径不存在，保留错误，已在现存周末脚本找到板端连接方式；未连接板子。
- 新RPM SHA与上轮ELF只登记身份，不归因；板上环境相同失败要保留并标为环境受限的成对等价，不冒充功能成功；候选独有失败单列。
- 包仓、sandbox、Source1002、QuickBuild 均不动；板上只允许本轮明确授权的安装/对照/清理，先记装前状态再操作。
- ARM首次核验器的 `Class:\\s+` 正则双重转义误判 ELF32，实际readelf明确ELF32/ARM，其余检查通过。raw/039退出1完整保留；首次目录移动为 verify-armv7l-initial-parser-error，旧脚本另存 verify-initial-parser-error.py，旧解包文件不删除。只修正验证器转义，在新的 `payload-...-verified` 目录重新解包核验，不重跑构建、不覆盖原始失败记录。该错误属于本轮工具错误，不是RPM/ELF失败。
- ARM核验时符号交集文本的换行被写为字面 `\\n`，不影响内存集合计算、770交集计数或门禁；原文件保留，输出可读副本供审阅。aarch64运行前修正显示分隔符；ARM实际执行脚本保留为 verify-corrected-executed.py。
- -lstdc++fs 的 ARM 实际解析已由独立输出的诊断重链接 `-Wl,--trace` 确认：`/usr/bin/../lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/libstdc++fs.a`。诊断链接不修改配方、原构建ELF、RPM或原dependency文件。不据此推断归档的每个成员都被拉入；W4审阅该项。aarch64同项待测。
- aarch64同项已测，解析 `/usr/bin/../lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/libstdc++fs.a`。两架构均为静态归档；实际trace、SHA与完整命令保留。
- 板上正常static安装预检退出1：MSM插件报告无法写 `/etc/device-sec-policy`，同时报告根文件系统需要92MB。df记录却有约1.3GiB可用；本轮不判原因、不修权限/策略/空间、不关闭插件、不用--nodeps、不重试预检。仅在失败后只读核对包/文件状态，恢复我们开启的SDB root模式；五个上传文件按“失败现场保留”要求留板，不清理。功能对照与取消探针不再运行，记NOT_OBSERVED。
