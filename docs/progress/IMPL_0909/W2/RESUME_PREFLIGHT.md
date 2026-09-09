# 续测前置确认（用户批准只编译测试后）

两个前置均通过后启动续测。没有重建运行库、重新配置 CMake、修改实施源码、
安装副本、官方测试或四个既有补丁。

## 运行库身份

上轮清理时保存的板端摘要与当前主机构建目录逐一相同，四个新旧文件的核对结果见
[身份表](RESUME_RUNTIME_IDENTITIES.tsv)。本轮只部署新库两个文件，旧基线不需要再运行，
没有部署旧基线库。板上此前已清空，所以不能在重新部署前声称板上仍有那两份库。
重新部署后、任何测试编译前，再核对板端 SHA256：

| 文件 | 本地、当前板端与上轮板端共同的 SHA256 |
|---|---|
| libc++.so.1 | d339c29ed11bc80e1a7ba82baf773014f7e97a89c205e2b39272b9723e9cf39f |
| libc++abi.so.1 | e874715056fe005edc2e6b69a6ecd00ee68a89b6eb539b008598bce5e176969c |

[上轮原始摘要](raw/previous_cleanup_04_runtime_hashes.stdout)、
[当前本地核验](raw/025_resume_preflight.stdout)、[当前部署核验](raw/026_deploy_resume.stdout)，
板端逐文件原文为 raw/resume_deploy_0_install.stdout 和 raw/resume_deploy_1_install.stdout。

### 安装副本差异的查明与保留的检查失败

首次预检在附加检查中停止：libcxx 的 test-suite-install/lib/libc++.so.1
并非与 build-armv7l/lib/libc++.so.1 字节相同；前者 SHA 为
eda1726505174705f27ba76d44bd2f072b3c3c356f1586d117ab6eaaf4098d95。
这不是部署运行库较上轮发生变化。只读读取 ELF（可执行与共享库文件格式），
在内存中模拟删除 RUNPATH（依赖库搜索路径）后，与安装副本**全文件逐字节相同**。
生成的 CMake 安装脚本也明确执行相同的 RPATH_CHANGE。没有改写任何二进制。
[动态段与安装脚本](raw/019_runtime_dynamic_difference.stdout)、
[全文件比较](raw/020_check_install_difference.stdout)。
libcxxabi 套件中的 libc++ 安装副本也是相同的 eda17265…；两处 libc++abi 文件均与构建物同 SHA。

首次断言把链接用安装副本与部署构建物混为同一字节身份；第二次只列入一个套件路径，
在另一个相同副本处再次停止。第三次额外比较无有效结果的 resume2，因它未输出 libc++abi
配置而停止，并非存在已观测参数值不同。保留 raw/017、021、023 的非零记录。
raw/024 明确旧有效记录来源仅 full 的 6,306 项与 resume1 的 1,542 项；两者完整配置相同。
raw/025 最终检查通过。raw/022 最后一条尝试读取不存在的历史日志导致总退出 2，
前面的摘要与符号读取成功，不将总退出码隐藏为 0。

## 编译配置

| 项 | 前 7,848 项与本轮 |
|---|---|
| 编译器 | 同一个 R68 armv7l_lit_host_clangxx.sh；实际 Clang 22.1.8 |
| 目标 | armv7l-tizen-linux-gnueabi；相同 sysroot、资源目录与链接器包装器 |
| 标准模式 | c++26 |
| 优化 | optimization=none，没有全局 -O 优化开关；不是运行库的 RelWithDebInfo 配置 |
| libc++ 链接 | -lc++experimental -nostdlib++ -L %{lib-dir} -Wl,-rpath,%{lib-dir} -lc++ -latomic |
| libc++abi 链接 | -nostdlib++ -L %{lib} -Wl,-rpath,%{lib} -lc++ -lc++abi -pthread -latomic |
| 其他编译选项 | 两套件的 cxx / flags / compile_flags / link_flags 共八项逐字相同，包括警告、线程、头文件和宏选项 |

完整长选项见 [当前逐字比较表](RESUME_CURRENT_CONFIG.tsv)，不是用上表摘要替代原文。
用户指定的四项编译身份以当前 lit 输出与历史两轮输出直接比较，不仅是路径没换。
官方测试各自附加的选项仍由原测试文件决定，没有全局改动它们。
[编译器版本](raw/resume_preflight4_compiler_version.stdout)、
[当前配置原文](raw/028_current_lit_configuration.stderr)、
[实际比较](raw/029_check_current_configuration.stdout)。

lit 的 --show-tests 会做环境特征探测，因此会编译/运行配置探针，但不执行列出的官方测试。
该阶段未重跑前 7,848 项。发现 11,402 个唯一测试，以原有效测试全名构造排除式，
正向检查 7,848 项全部被排除，余下集合与原缺项表的 3,554 项精确相同：
[选择核验](raw/029_check_current_configuration.stdout)、[精确清单](RESUME_SELECTED.tsv)。
正式续测复用原执行器、owner 身份、工作路径、超时、nice/ionice；并行度 2。

两次 SDB 回执中断后的变化另列：执行器副本仅增加清理回执丢失的独立路径不存在核验；
第三段改为单 worker，避免同时发起两个 SDB 会话，仍不超过并行上限 2。
这些不是更改编译/链接选项；原有效 7,848 项中 resume1 也使用过单 worker。
不把串行化当成“已证明传输根因”或性能可比性保证。

## 板卡与时间

重新读取编译线当前状态，明确“板子已释放”后才连接、部署。
续轮开始前板端 /opt/usr/impl_0908_lit 不存在，系统进程清单已记录。
运行控制器将前段约 40 分钟计入六小时额度，另在截止前预留五分钟停止/清理。
未把等待用户授权的间隔当成后台一直运行，也没有重置成额外六小时。
实际起止、截止时间与停止原因由 raw/lit_resume/time.json 记录。
