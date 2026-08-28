# R79：产品源码中的异步取消模式检索

## 结论摘要

冻结的 371 个成功提取 source RPM 实例中：

- 任意取消相关 token 命中 45 包、170 文件、418 行；
- `pthread_setcanceltype` 命中 15 包；
- `PTHREAD_CANCEL_ASYNCHRONOUS` 命中 11 包；
- 直接字面调用
  `pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, ...)` 命中 5 包、17 行；
- 上述 5 包中，2 包只在内核自测源码出现，3 包在生产源码出现：
  `lightweight-web-engine`、`llvm` 的 OpenMP runtime、`mesa`；
- 静态源码可确认处于“C++ 线程启用异步取消且存在取消发起路径”形态的为
  2 包：`lightweight-web-engine` 与 LLVM `libomp`；
- Mesa 的两个异步取消线程和取消发起路径同样明确，但入口与已见调用链均为
  C 源码，未观察到 C++ EH/析构帧，故不计入“已确认 C++ 风险形态”。

这些是静态源码事实，不是运行时复现；本任务没有构建或执行产品，也不提出
处置建议或优先级。

实际执行时间：2026-08-28T21:23:23+08:00 至
2026-08-28T22:16:14+08:00，约 53 分钟。light 资源门禁退出 0。

## 一、数据来源、分母与方法

数据来源是 `tmp/corpus/extracted/` 的冻结 T1 source RPM 展开语料。分母由
`progress/T1/extraction_results.tsv` 中 `status == EXTRACTED_OK` 唯一定义：
371 包；输入表 SHA256 为
`5498c430baecd81e689a44d8266d5597a92779680105ede0c320f9b1ea013031`。
另一个 `chromium-efl-1.1.144-1.src.rpm` 提取失败或不完整，不计入分母。
`tables/package_denominator.txt` 保存 371 个包名。

全量扫描约 49 GiB 展开树，使用 `rg --no-ignore --threads 2`，检索：

- `pthread_setcanceltype`；
- `PTHREAD_CANCEL_ASYNCHRONOUS`、`PTHREAD_CANCEL_DEFERRED`；
- `pthread_setcancelstate`；
- `pthread_cancel(`。

`--no-ignore` 是必要条件，因为仓库 ignore 规则覆盖 `tmp/`。最终扫描正常退出
0，原始 418 行完整保存在 `tables/rg_all_token_hits.txt`，SHA256
`6f6eec7e04da0f5b3976f8b97ee22e91544cf437848db55f2f569d8b3b96ce6d`。
包级 token 计数在 `tables/package_token_counts.tsv`。

机械扫描后只读取命中文件及对应 spec/构建文件，复核：调用是否真的选择
ASYNC、是否为声明/文档/测试、函数是否是线程入口、是否存在对该线程的
`pthread_cancel`、生产构建是否包含该源码。

## 二、直接异步模式命中

完整逐行分类见 `tables/async_mode_sites.tsv`，包含 17 个字面 setter 行和
1 个测试中的别名宏 setter 行。

### 1. lightweight-web-engine

- 位置：
  `src/core/modules/worker/WorkerThread.cpp:182`；
- 形态：`initializeWorkerThread()` 先 ENABLE，再设置 ASYNCHRONOUS；同一类的
  `destroyWorkerThread()` 对 `m_workerThread.native_handle()` 调用
  `pthread_cancel`；
- 线程属性：C++ `WorkerThread` / `std::thread` 路径，确实是可能被取消的
  线程；
- 静态判定：`CONFIRMED_SOURCE_SHAPE`。源码同时确认异步模式、C++ 线程和
  取消发起路径；没有运行时落点证据。

### 2. LLVM OpenMP runtime (`libomp`)

- 位置：`openmp/runtime/src/z_Linux_util.cpp:562` 和 `:640`；
- 形态：`__kmp_launch_worker` 与 `__kmp_launch_monitor` 在
  `KMP_CANCEL_THREADS` 下设置 ASYNCHRONOUS 并 ENABLE；
- 编译门：`openmp/runtime/src/kmp.h:48` 默认定义
  `KMP_CANCEL_THREADS`，仅 Android/WASI 等路径取消定义；Tizen 不属于源码中
  这两个排除条件；
- 取消发起：同一门控下 `__kmp_terminate_thread` 调用 `pthread_cancel`；
- 产品包含：平台 `llvm.spec` 启用 `openmp` project 并把 `libomp.so` 纳入
  `libomp` 子包；
- 静态判定：两个 C++ runtime 线程均为
  `CONFIRMED_SOURCE_SHAPE`。未观察实际产品是否在运行中进入终止路径及取消
  恰落位置。

### 3. Mesa Vulkan WSI

- 位置：`src/vulkan/wsi/wsi_common_display.c:1344`、`:2218`；
- 形态：`wsi_display_wait_thread` 与 `udev_event_listener_thread` 设置
  ASYNCHRONOUS；对应 teardown/stop 路径调用 `pthread_cancel`；
- 构建条件：`wsi_common_display.c` 在 KMS DRM 且非 Android 时进入 Vulkan
  WSI 源列表；平台 spec 有条件启用 Vulkan driver；
- 静态判定：两个线程“确实异步可取消”，但线程入口和复核到的循环均为 C，
  `NOT_CONFIRMED_CPP_EH_RISK`。没有证据证明其栈包含本方案防护所针对的 C++
  EH/析构帧。

### 4. 内核源码包中的测试命中

`rpi4-linux-kernel` 的四个 x86_64 KVM selftest，以及 `emulator-kernel` 的四个
KVM selftest和一个 vsock test，均有真实 setter 和测试内取消路径。它们位于
`tools/testing`，逐项列在表中，归类 `TEST_ONLY`；不计为产品运行时风险包。

### 5. 文档与静态分析测试

LLVM clang-tidy 文档一行、checker 测试两个直接 ASYNC setter 和一个别名宏
`ASYNCHR` setter，是专门用于识别不安全异步取消的测试输入，不是产品线程。

## 三、常量、宏与间接路径

逐项摘要见 `tables/constant_and_indirect_sites.tsv`。

- LLVM compiler-rt 与两个冻结 GCC source RPM 实例中的 sanitizer interceptor
  接收调用者提供的 `type` 并转发给真实 `pthread_setcanceltype`。它们是可见
  的间接框架路径，但本身不选择 ASYNC；线程是否异步取决于外部调用者。
- LLVM clang-tidy checker按宏展开识别 ASYNC，属于静态分析工具，不是运行
  时 setter。
- GCC libphobos、CoreCLR WASI 兼容头只有常量/声明；未发现对应 setter 调用。
- 两个 gettext source RPM 实例中的常量位于 gnulib test；未发现 ASYNC setter。
- lightweight-web-engine 与 lwnode 捆绑 GC 文档明确记载 async cancellation
  对 collector 不安全；lwnode 未发现选择 ASYNC 的 setter。
- 没有识别出 `dlsym("pthread_setcanceltype")` 或等价动态解析调用。扫描中出现
  的函数名字符串主要是错误消息、sanitizer 注册和 clang-tidy 规则。

## 四、统计口径与影响事实

| 口径 | 包数 / 371 | 说明 |
|---|---:|---|
| 任意取消相关 token | 45 | 包含普通 deferred cancel、测试、声明和文档 |
| `pthread_setcanceltype` token | 15 | 包含声明、interceptor、测试、DEFERRED |
| ASYNC 常量 token | 11 | 包含文档/头文件/测试 |
| 直接字面 ASYNC setter | 5 | 3 生产源码包 + 2 测试源码包 |
| 生产源码直接 ASYNC setter | 3 | LWE、LLVM OpenMP、Mesa |
| 已确认 C++ 异步可取消源码形态 | 2 | LWE、LLVM OpenMP |

`pthread_cancel` token 命中 42 包，但 POSIX 新线程默认是 deferred；没有 setter
或间接配置证据时，不能据此把这 42 包算作异步取消使用者。

“已确认 C++ 风险形态 2 包”只表示源码同时具备 C++ 线程、ASYNC setter 和
取消路径。它不证明某次取消会落在无取消点忙循环，也不证明运行时一定绕过
C++ EH；这两点本轮均为 `NOT_OBSERVED`。

## 五、无法覆盖的情形

- 371 包之外、提取失败或未纳入冻结 T1 清单的源码；
- 预编译 DSO、私有源码、运行时生成代码和外部组件内部行为；
- 函数名被加密/拼接后动态解析，或不含字面 token 的自定义系统调用封装；
- interceptor/框架参数由外部组件在运行时传入 ASYNC 的情况；
- 源码有条件编译但最终产品配置、启动参数或动态 feature 决定是否进入的路径；
- 运行时从 ASYNC 切回 DEFERRED 的时序，以及取消实际落点；
- 只存在 `pthread_cancel` 而取消类型由别处设置的跨包组合；本次语料内可见
  setter 已扫描，但发布 ELF/跨组件配置未做数据流关联；
- 二进制文件中的字符串或符号：ripgrep 默认不把二进制内容当源码文本结论。

## 六、执行异常与自行判断

- 第一次 Python 顺序读取在约 3 分钟中断，因大型展开源码成本过高；没有输出
  判定结果。改用同分母的 ripgrep。
- 第一次 ripgrep 漏 `--no-ignore`，因 `tmp/` 被忽略而退出 1、零命中；修正后
  完整扫描正常退出 0。
- 一次为诊断零命中的宽扫描在 60 秒中断；一次 LLVM 包级宽复核在 120 秒
  中断；均改为明确命中文件/目录，原尝试和退出 130 已记录。
- 自行判断：以 source RPM 实例而不是去重软件名为分母，保留同一版本的多个
  发布实例；与 T1/R66 口径一致。
- 自行判断：只有生产源码中 C++ 线程同时具备 ASYNC setter 和取消路径才计入
  “已确认风险形态”；C-only Mesa 和测试程序不计入该数字。
- 尚存需人工裁决疑问：无。

本任务只做检索与统计，没有修改产品源码、没有构建、没有使用开发板、没有
提出处置建议或优先级。

## 材料导航

- `tables/rg_all_token_hits.txt`：418 行完整机械候选；
- `tables/package_token_counts.tsv`：45 包逐 token 计数；
- `tables/async_mode_sites.tsv`：直接/别名 ASYNC setter 逐项分类；
- `tables/constant_and_indirect_sites.tsv`：常量、声明、interceptor、框架路径；
- `raw/019_*`、`raw/023_*`、`raw/028_*`：生产命中上下文与构建包含证据；
- `commands/`：命令原文与退出码。
