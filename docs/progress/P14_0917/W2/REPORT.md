# 真实 GNU EWK 分配失败：对象定位与候选修复

状态：**PARTIAL：独立复现和对象定位完成；局部 GN 候选补丁已生成并通过应用检查，尚未构建或验证修后结果。** 没有改平台源码、配置或原始 provider。

## 1. 独立复现（实测）

GNU-only 程序通过真实 `ewk_parse_cookie` 入口调用 `libchromium-impl.so`，不经过 adaptor。每次打印 maps，并断言 GNU 标准库和指定 provider 已加载、libc++ 未加载。无 libc++ 的检测有 W1 显式加载组作正向对照。

输入是 256 个 `N` 作名称、512 个 `V` 作值，附 `Domain=new.example; Path=/new`。输入及预热在观察窗口外建立。只令当前线程、本次调用的第 4 次 malloc 家族请求返回空指针；其余分配正常。GNU operator new 随后抛 bad_alloc，由应用捕获。测试 allocator 不属于生产方案，没有用它强行释放任何未知所有权指针。

| 情形 | 重复数（进程） | 每进程结果 |
| --- | --- | --- |
| 正常调用及输出销毁 | 5 | 具体四字段、expiry 正确；live=0，bytes=0 |
| 一次第 4 次分配失败 | 5 | caught=1，输出不变，live=3，bytes=834 |
| 连续 10 次上述失败 | 5 | caught=10，输出不变，live=30，bytes=8340 |
| 原始 GNU 入口的既有 12 场景 | 5 | EXACT=12/12、DESTROYED=12/12 |

具体数字已由 `audit_original.py` 对逐次原始输出和 SHA256 作断言，见 ASSERTED_ORIGINAL_RESULTS.json。所有计数器 overflow=0。正常调用样本覆盖了计数器对真实分配/释放的观察，不能只以“没有崩溃”作结论。

## 2. 遗留的是哪些对象

增加不分配内存的固定记录表和展开栈采集；输出在计数窗口关闭后进行。`raw/symbol_original_4.stdout` 保存每笔分配的栈、对象地址、存活字节。指针关系的自动核对见 ALLOCATION_OWNERSHIP.json。

| 分配序号 | 请求字节数 | 实际内容/关系 | 源码对应 |
| --- | --- | --- | --- |
| 1 | 257 | 名称 `N×256` 与结尾零；存活内容前 64 字节为 N | `net/cookies/parsed_cookie.cc:576`，局部 pair.first 的字符串 |
| 2 | 513 | 值 `V×512` 与结尾零；存活内容前 64 字节为 V | 同文件 585，pair.second 的字符串 |
| 3 | 64 | 两个 GNU string 的成对存储；偏移 0/32 的指针精确等于前两块，长度精确为 256/512 | 同文件 634，移动 pair 到 `pairs_` |
| 4 | 128（失败） | 从一对扩容到两对所需的 vector 存储；与第 3 次同一分配调用栈 | 同文件 634，再次 emplace_back |

257+513+64=834。136 字节的异常对象另有分配记录，但已释放，不计入泄漏。连续失败后线性累积，不是一次性缓存初始化。观察窗口内输出对象已经销毁，输入在窗口外，不能把这些对象误计为遗留输出。

`ParsedCookie` 构造函数 147 行调用 `ParseTokenValuePairs`，后者 545 行建立局部 pair，576/585 行建立字符串，634 行移入成员 `pairs_`。头文件中 `pairs_` 是标准 vector；析构在 157 行为 default。**源码已经使用 RAII，没有一个应补写 delete 的裸指针所有权。** 第 4 次失败发生在构造未完成期间，不能指望调用完整对象的析构；需要构造异常路径清理已经构造的成员和局部对象。

关键原文（`net/cookies/parsed_cookie.cc`；完整带行号上下文在 raw/002_source.stdout、raw/003_more_source.stdout）：

```cpp
// 147
ParseTokenValuePairs(cookie_line, *status_out);
// 157
ParsedCookie::~ParsedCookie() = default;
// 545
std::pair<std::string, std::string> pair;
// 576
pair.first = std::string(token_start, token_end);
// 585
pair.second = std::string(value_start, value_end);
// 634
pairs_.emplace_back(std::move(pair));
```

证据强度区分：堆块内容、指针关系、调用地址、缺少清理元数据是实测/ELF 静态事实；将无调试符号的内部地址命名为对应源码函数，依据调用结构与数据布局，是明确标注的结构对应推断，不是 DWARF 行号解析。发布库未提供可直接用于这些地址的内部符号名。

## 3. 为何现有 RAII 没起作用

`raw/021_frames_corrected.stdout` 显示公开入口、对应构造路径、解析循环及分配调用所在函数的 FDE 使用 `zR` CIE，无 personality/LSDA 清理描述。旧 `016` 的最后一个辅助地址落在相邻函数，已在 `021` 改为真实分配调用返回地址之前的一字节，旧记录未覆盖。

源码 `build/config/BUILDCONFIG.gn` 默认使用 no_exceptions；`build/config/compiler/BUILD.gn:2571–2596` 将 exceptions/no_exceptions 分别映射为 `-fexceptions`/`-fno-exceptions`。现有产物没有上述清理描述，与异常关闭构建的解释一致。**该产物完整原始编译命令为 NOT_AVAILABLE**，因此不声称已经拿到它的实际 command line。

结论：已有对象所有权可以定位到 `ParsedCookie::pairs_` 及首个 name/value；当前二进制在异常越过时不执行这些帧的清理。仅增添另一层 RAII 或在外层 catch，不能补回已越过帧的成员清理。

## 4. 候选补丁及其限制

文件：`provider-exception-cleanup.CANDIDATE.patch`。基线是 `chromium-efl-1.1.144-1.src.rpm`：

`2ae259fd7f28355b1639d15009a9b66d38de98ab590bd534e29ad1d443a4e121`

真实 GNU provider SHA256：

`18f4ea7cf5962c8d9ecb2436343a881852c5e730326a98c18029a3d5883b27b7`

源仓 Git revision 为 **NOT_AVAILABLE**：当前输入是 SRPM 快照，没有观测到可核实的源码仓提交；没有用版本号伪装 Git revision。SOURCE_INDEX.json 和 PATCH_IDENTITY.json 提供逐文件基线与候选 SHA256，baseline/ 与 candidate/ 保存两份 GN 文件。

候选将两个原有 `.cc` 分别放到局部 source_set，保留已有 RAII，单独启用 exceptions；补上 net 目标的私有依赖可见性和头文件循环包含许可。其他源文件不整体切换异常设置，公开签名和解析源码不改。不添加重复或不安全的析构/清理逻辑。

**这是构建层的候选草案，不是已经采纳的产品改动。** 它偏离任务书示例中的“再加守卫/catch”，原因是现有 RAII 已经存在；已就局部 GN 候选方向向人工提问，尚未收到答复，不把该方向写成人工已认可。

已做：补丁在独立基线副本上 `git apply --check` 和实际 apply，通过；应用后两文件 SHA 与候选一致。首次 diff 路径格式错误的失败记录保留，修正后的检查在 raw/024_candidate_corrected.*。

未做成：从真实 SRPM 选择提取 8,651 个源码/头/构建文件，约 90.8 MB；尝试编译两个真实 `.cc`，均停在缺 `base/debug/debugging_buildflags.h`，见 raw/compile_source_*.stderr。当前 PATH 无 GN、没有该产品可用 Ninja 图及生成头/参数闭包。没有猜测生成头里的宏值，没有用模拟 parser 或旧 GNU 库冒充修后产物。

因此 **修后泄漏量=0：NOT_OBSERVED；修后 12 场景：NOT_OBSERVED**。候选 GN 的依赖闭包、实际编译参数、链接及其他分配点仍须验证。不能写“已修复”或“patch 已验证”。缺的是该源码版本的产品 GN 参数、生成头及可重建相关目标的依赖闭包。

准备了 `verify_candidate.py`：要求提供真实重建库与其构建清单，拒绝原始库同 SHA 替代；执行失败单次/连续 10 次及 12 场景，各 5 次。当前缺输入时实测退出 77，明确 NOT_AVAILABLE，不生成通过矩阵。

## 5. 范围与自行判断

使用 x86_64 原生、GNU14 SDK 与真实 GNU provider；armv7l 为 NOT_OBSERVED_BOARD_OFFLINE。计数是单线程窗口内 malloc 家族，不是任意 allocator、并发、所有失败位置或完整浏览器的泄漏证明。没有为失败后半修改输出承诺原接口不存在的强保证；本次第 4 次失败点尚未进入输出写入。

本轮自行选择第 4 次失败复现、增加调用栈与对象字节核对、提出局部翻译单元异常清理候选。外部缺陷分类依据人工裁决；定位和补丁验证强度依各自证据，不混用。
