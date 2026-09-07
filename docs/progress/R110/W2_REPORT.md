# aarch64 运行时覆盖可行性评估

## 结论

QEMU 用户态对 aarch64 **可用，已实际执行确认**。它不在宿主 PATH 中，但冻结 Base 仓的 `qemu-linux-user-9.2.3.1-3.4.x86_64` 载荷内有静态 `qemu-aarch64 9.2.3`；用它执行平台 aarch64 glibc 2.40 程序成功，并在同一进程装载、初始化和析构平台 `libc++.so.1.0` 与 `libstdc++.so.6.0.33`，精确返回 `glibc 2.40`，退出码 0。

这证明现有材料可用于 aarch64 的装载共存预检和确定性用户态 ABI/布局实验；它不把既有 aarch64 缺口补全，也不替代真实目标内核上的取消、调度、信号时序、设备 provider 和产品镜像验证。

## 可用执行手段

完整对照见 `tables/w2_execution_methods.tsv`。

- 宿主 PATH 中没有 `qemu-aarch64`，但冻结 RPM 已下载并解包，按二进制包 SHA256 精确定位后可以直接调用；模拟器是静态 x86_64 ELF，版本 9.2.3。
- 平台 aarch64 glibc、libc++/libc++abi/libunwind、libstdc++ 和 libgcc 载荷都可从同一冻结仓按 RPM SHA256 定位，足以组装显式 loader/library-path。
- `binfmt_misc` 报 `enabled`；本轮没有证明具体 aarch64 handler 注册，实际试跑不依赖它。
- `podman`、`docker`、`systemd-nspawn` 和 `qemu-system-aarch64` 均不在 PATH。容器即使存在也只会封装文件系统，不能独立完成异构指令执行。
- 小型交叉构建部分可行：宿主 `llvm-mc` 与 `qemu-accel` 载荷中的 aarch64 GNU linker 已实际生成 AArch64 ELF。完整 C++ 编译应沿用现有 buildroot；解包的 qemu-accel 工具采用 `/emul` 绝对前缀，不宜直接当普通 sysroot。

### QEMU 用户态的可信边界

适合：确定性指令与用户态布局、动态 loader 行为、显式值断言、析构路径、无需设备的纯用户态 C++ 调用，以及 pthread/futex/signal 的功能预检。

不宜作为最终证据：目标内核取消实现、真实调度与竞争窗口、信号精确时序、自然底层错误概率、硬件/驱动交互、启动链、完整产品镜像的插件装载顺序、性能和长期压力。QEMU 用户态把目标系统调用翻译给宿主 x86_64 内核；“在 QEMU 下没有崩溃”不等于平台没有问题。

## 已有结论逐项评估

五项都需要在 aarch64 上单独标注结果，不能从 x86_64 或 armv7l 外推。逐项依据、可由 QEMU 完成的部分和必须使用物理板或等价完整目标环境的部分见 `tables/w2_revalidation_matrix.tsv`。

其中，跨库布局的确定性值和销毁路径只依赖 aarch64 ABI、真实 provider 和两套运行库时，QEMU 用户态可形成有力运行证据，纯布局并不天然要求物理板。线程取消、五处等待/异常站点以及异步取消正交性都涉及信号、futex、unwinder 和调度；QEMU 可做功能预检，但在当前没有同内核 full-system 环境的条件下，平台级结论仍需物理 aarch64 目标。共存场景中的纯装载可用 QEMU，设备绑定 provider 和真实产品装载拓扑需要完整目标环境。

## 最低成本实际试跑

选取“两个 C++ 运行库在同一 aarch64 进程共存”的**装载与生命周期子集**，理由是它无需重新构建产品组件，可以同时检验：

1. aarch64 指令执行；
2. 平台动态 loader 与 glibc；
3. libc++、libc++abi、LLVM libunwind、libstdc++ 和 libgcc 的依赖解析；
4. 两套 C++ 运行库的初始化与析构；
5. 精确业务输出值，而非只看退出码。

输入身份：平台 glibc 2.40-2.9、libc++ 22.1.8-19.1、libstdc++ 14.2.0-1.15、libgcc 14.2.0-1.15。脚本用 loader 的 `--preload` 装入两套运行库，`LD_DEBUG=files` 给出两者各自 `generating link map`、`calling init` 和 `calling fini`；平台程序输出精确等于 `glibc 2.40`。所有断言通过，脚本退出 0。

该结果只说明“QEMU 环境能承载此类 aarch64 装载/生命周期测试”，不等于跨库对象传值安全，也不覆盖线程、异常穿越或真实 provider。

### 失败尝试的解释

首次自制无 C runtime 启动探针经历三步：宿主无 `clang`（退出 127）；改用 `llvm-mc` 后 linker 因共享库依赖未给 `rpath-link` 拒绝（退出 1）；允许共享库未解析后生成的非标准双 `DT_NEEDED` ELF 在 QEMU 中收到 SIGSEGV（退出 139）。该 ELF 绕过 glibc 的标准启动序列，不能作为平台共存失败证据，所以未纳入结果格；原始输出完整保留。随后改为真实平台 glibc PIE 程序和真实 loader，成功结果不覆盖前述非标准探针问题。

另一次无界目录定位在 30 秒后按精确 PID 终止，补记 143，并改为按 package record SHA256 定位。没有把搜索超时写成材料不存在。

## 后果严重性的依据（不排序）

- 线程取消、等待站点和异步取消正交性若在 aarch64 不同，可能表现为进程终止、清理遗漏或锁/所有权状态异常；它们影响控制流与资源一致性。
- 跨库布局若不同，可能出现“退出 0 但值已错误”，或在消费方销毁 provider 构造对象时延迟失败；静默错值不能由“没有崩溃”排除。
- 共存装载、符号归属或异常展开若不同，影响可能扩展到同进程多个组件，而不是一个调用点。

以上只给出后果依据，没有排列先后，也没有替人工选择执行顺序。

## 证据索引

- 手段与限制：`tables/w2_execution_methods.tsv`
- 逐项重验矩阵：`tables/w2_revalidation_matrix.tsv`
- 可重复成功脚本：`code/w2_run_preload_coexist.sh`
- 脚本 SHA256：`raw/032_hash_w2_scripts.stdout`
- 成功试跑：`raw/031_run_aarch64_preload_script.*`
- 环境清点：`raw/021_*`、`raw/030_*`
- 失败尝试：`raw/023_*` 至 `raw/028_*`
