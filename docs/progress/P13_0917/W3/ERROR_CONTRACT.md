# adaptor 同步错误契约与尚未成立的保证

状态：**契约的可实现部分已实施；完整资源释放保证未成立，RELEASE_BLOCKED。** 原后端 5/5 SIGABRT 已修复，不再是当前阻断原因。生产方向、真实回调、armv7l 等仍未验证。

## 1. 返回与错误映射

公开 `ewk_parse_cookie(const std::string&, EWKCookieContents&)` 的符号和 Eina_Bool 返回类型不改：成功 EINA_TRUE，失败 EINA_FALSE。**只有内部 POD 通道返回 1/0/负 errno**，不能把 -12 当成原 EWK 的返回值。

| 情形 | 内部 POD 返回 | 公开返回/errno | 依据 |
| --- | --- | --- | --- |
| 解析成功 | 1 | true；保留 provider 的 errno 观察值 | 12+17 场景及直接接口对照 |
| provider 正常拒绝输入 | 0 | false；保留原观察值，不强制设置 EINVAL | 空输入对照：errno 初始 17，三种形态返回后仍为 17 |
| std::bad_alloc | -ENOMEM | false / ENOMEM | 原失败用例、真实 64 MiB 输入、各阶段注入及真实 malloc 故障 |
| std::length_error | -EOVERFLOW | false / EOVERFLOW | 后端 reserve(max_size+1) |
| std::invalid_argument | -EINVAL | false / EINVAL | 注入测试；真实 parser 抛该类型为 NOT_OBSERVED |
| 其他 std::exception | -EIO | false / EIO | runtime_error 注入 |
| 非标准同步异常 | -EIO | false / EIO | 自定义类型注入 |
| 非空长度对应空指针、缺少内部参数 | -EINVAL | 内部拒绝；合法前端不会构造此参数 | POD 单元测试 |
| 超过本侧 max_size、长度求和溢出 | -EOVERFLOW | false / EOVERFLOW | 单元及边界测试 |
| 缺实现库、缺符号、错误 DSO 归属、自指向、加载重入 | -ENOENT/-ENOSYS/-ELIBBAD/-ELOOP/-EDEADLK | false / 对应 errno | 部分实测、其余静态分支；不得统称所有加载条件已实测 |

成功/普通拒绝不是“errno 清零”的约定。调用方不能仅凭残留 errno 判断本次错误；本接口原返回类型不能无损地区分所有错误分类。代码只在映射到具体 adaptor 错误时覆盖 errno。真实 EWK 对极大 Max-Age 的既有行为不在此擅自修正。

catch 分支只返回或赋值整数；不调用 what、不创建字符串、不记录可能分配的日志。测试日志在 handler 之外，发布候选不编入测试日志/注入入口。该陈述针对 adaptor 源码，不是异常运行时内部所有操作都不分配的证明。

## 2. 失败时状态

**已验证的保证（固定 x86_64 配置与已测同步路径）：**公开调用返回映射错误时，调用方四个字符串的内容及存储地址、expiry 均保持原状；下次正常调用可恢复。后端先保留调用前四字段，再在自身 ABI 下调用；前端四个新字符串全部准备完成之后才交换有变化的字段。旧 expiry 从不读入，避免依赖未初始化标量。

**已验证的清理范围：**在 adaptor 的指定注入点失败，作用域析构完成；180 次有正向对照的分配窗口中，所跟踪的 malloc 家族分配全部配对释放。该结果覆盖前端、后端和异常对象在该窗口中的分配，不是并发或全程序无泄漏证明。

**未成立的保证：provider 内部实际分配失败后，所有内部资源都释放。** W3 直接对真实 malloc 点注入失败，公开返回虽已是 false/ENOMEM 且输出不变，但出现稳定未配对分配，详见下节。不得把这条写成已保证。

## 3. 与真实原始 GNU API 的直接对照

P12 的 GNU-only `-12` 对照调用的是 `p12_cookie_parse`（adaptor POD 后端），不是原始 Eina_Bool 入口。因此本轮另用 GNU-only 应用直接调用真实 provider 的 `ewk_parse_cookie`，不经过 adaptor，作为原接口基准。

完整对照 `full_CONTRACT_COMPARISON.json`：原始入口、独立放法、内嵌放法三种形态；7 个分配序号/正常/拒绝配置，各 5 次，共 105 个进程。原始入口逐次 maps 断言 GNU 存在、libc++ 不在；adaptor 两形态逐次断言双库/入口/provider 存在。GNU-only 的“无 libc++”有双库组的正向检测对照。

| 观察 | 原始 GNU 入口 | adaptor 两种放法 |
| --- | --- | --- |
| 正常合法输入 | true，具体字段正确 | 相同语义与字段 |
| 空输入 | false，输出保持，errno 保留 17 | 相同 |
| 触发真实分配失败 | 部分格 bad_alloc 逸出，由应用 typed catch 接到；原函数未返回 bool | 不逸出，返回 false/ENOMEM，输出不变 |
| 失败后配对计数 | 部分格仍有未配对分配 | 部分格同样有未配对分配 |

因此，**异常传播约定不完全一致**。转为 bool+errno 是本任务明确要求的转换，但依赖捕获 bad_alloc 的旧调用方将不再走同一处理路径，不能说原接口行为完全未变。当前样本中 adaptor 未再因普通错误终止，也未将半构造输出交给调用方；这不构成任意失败下“绝不更差”的通用证明。

不能以“原 provider 也有问题”排除风险：异常变成返回值后，调用方可能继续或重试，而本轮连续失败已有累计未配对分配的实测。具体业务调用方是否走相同错误处理路径为 NOT_OBSERVED，因此本轮不能签发“不会更差”的无条件保证。

## 4. 新的资源清理阻断

失败序号是整个当前调用分配窗口的序号；不同形态分配步骤不同，**同一序号不代表同一个 provider 内部位置**，不得据字节数大小比较谁更好。

- 独立和内嵌形态：序号 4 失败时，各 5/5 观察到返回 ENOMEM、输出不变，但 live=2/bytes=770；序号 8 时，各 5/5 live=3/bytes=1026。
- 原始 GNU 入口：序号 4 失败时，5/5 应用捕获 bad_alloc，live=3/bytes=834。
- 排除一次性初始化解释的复测：同一进程连续 10 次相对序号 4 失败，各形态重复 5 个进程。adaptor 两形态分别每进程 live=20/bytes=7700，GNU 原始入口 live=30/bytes=8340；调用方输出仍未改变。见 `repeated_CONTRACT_COMPARISON.json`。
- 正常成功格在计数窗口结束时仍由调用方持有新输出字符串，live>0 **不是泄漏判断**，未误报为失败。上述异常格需另行解释其未配对分配，不能把成功格的对象所有权与异常格混用。

静态核查：真实 `ewk_parse_cookie` 地址 0x603a1e6 的 FDE 范围到 0x603a451，引用 CIE 的 augmentation 是 `zR`，没有 personality/LSDA；见 `raw/006_parser_frames.stdout` 和反汇编。**推论：该栈帧没有提供由异常运行时调用的 C++ 清理描述；外部 catch 无法补回其内部局部对象清理。** 结合稳定累计的未配对分配，指向 provider 异常清理缺口；尚未逐个定位每一块分配对象，不冒充完整泄漏分析。

没有把测试计数器用于生产中强行 free 未配对指针：它不能证明任意指针的生产所有权。修正 provider 的异常清理支持超出“不改平台源码/配置”的授权；已提问，未自行实施。**当前不能签发完整资源释放保证或上线通过。**

## 5. 明确不保证

并发/竞争、取消及强制展开、栈溢出、信号、悬空指针/损坏对象、任意 allocator、任意运行时版本、生产方向真实 libc++ EWK、armv7l、真实 SetSyncBinaryReply 注册回调，均不在本轮已验证保证内。

特别是析构：析构内部捕获第二异常、正常退出时析构抛出、catch 后换异常再抛，均有存活实测；**已有异常展开期间第二异常逸出析构，外层 catch 不能阻止 terminate，实测两放法各 5/5 SIGABRT**。字面要求“这种情形也不终止”没有满足，等待人工厘清契约，而非默认排除后宣称全部通过。
