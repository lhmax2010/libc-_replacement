# EWK adaptor 状态语义核查与修订实验

结论：**STATE_SEMANTICS_SOLVED，仅限本次真实 GNU provider 正常返回时的字段/复用语义**。不等于可部署认证，不涵盖异常、取消、内部引用、并发及反方向。修订原型与同侧对照在 x86_64 原生、armv7l **物理板**上均为每场景 5/5 正确；共 12 个场景。板子已清理并释放。

## 真实源码约定

原件：`chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc`，哈希在 `source_identity.json`；带行号全文摘录在 `../W1/raw/008_targeted.stdout`。

```cpp
// 实际源码 32–49 行节选
net::ParsedCookie new_cookie(cookie_str);
if (!new_cookie.IsValid() || new_cookie.IsHttpOnly() || new_cookie.IsSecure())
  return EINA_FALSE;
cookie.name = new_cookie.Name();
cookie.value = new_cookie.Value();
auto domain = new_cookie.Domain();
if (domain.has_value()) {
  cookie.domain = domain.value();
}
auto path = new_cookie.Path();
if (path.has_value()) {
  cookie.path = path.value();
}
```

静态结论：name/value 在接受输入后赋值；domain/path **存在才覆盖，缺省保留旧值**，不是追加。属性显式存在但值为空与属性不存在不同。32–34 行拒绝输入发生在任何字段赋值之前，保持原输出（包括 expiry）。54–77 行处理 Max-Age/Expires；接受且无有效时间属性时设置 session expiry 0。不能把“全部四字段皆为输出”理解为函数会初始化全部四字段。

## 修订方法与新发现

双编译单元之间仅传输入指针/字节长度、四组初始字段指针/字节长度、整数 expiry 与回传回调。GNU 一侧构造真实 `EWKCookieContents`，先复制旧状态，再调用通过明确句柄取得的真实 `ewk_parse_cookie`。libc++ 一侧以四个本地字符串接收结果，成功后提交；拒绝输入不改调用方输出。双方分别销毁自己的字符串，接收方不 delete 对方对象。`dladdr` 核对函数所属 provider，maps 核对双库、入口和 provider。产物仅在 tmp；源码快照是实验代码，不是平台补丁。

**初版没有通过，不能隐去**：首次把 libc++ 接收临时对象也写成 `EWKCookieContents`。两个 TU 生成同名、WEAK HIDDEN 的 `_ZN17EWKCookieContentsD2Ev`，链接后只保留一种析构实现。`raw/007_object_symbols.stdout` 给出两个对象文件的符号；`006_debug_state.stdout` 的实测栈显示 GNU backend 析构进入 free 后 SIGSEGV。初版对照 5 次通过，两个初版 adaptor 模式各 5 次故障，记在 `native_matrix.json`，不并入规范通过矩阵。

修订 front 改为 `std::string next[4]`，不在 front 构造/销毁共享名字的 EWK 包装类；GNU backend 保留真实类型。修订对象符号表在 `v2x86_64_symbols.stdout`、`v2armv7l_symbols.stdout`。这说明**标准库命名空间隔离不自动保护外部同名包装类的隐式成员**。仅加 hidden 不防止同一 DSO 内的弱定义合并。LTO、更换优化/编译器、其他隐式成员没有验证；这是一种受控二进制实现实验，不是可移植 C++ 的通用混用保证。

实验性异常策略也明确不同：backend catch-all 转负状态，front 抛本地 runtime_error；不冒充保留原异常类型/部分写入或强制展开。当前完成的修订只是字段状态协议，不可把实验错误处理原样当生产方案。

## 运行矩阵与计数单位

规范原始数据：`v2native_matrix.json`、`board_matrix.json`；汇总 `MATRIX.md`、`summary.json`。各命令、完整 maps、字段的长度/十六进制字节、目标退出码在 `raw/`。

| 场景号 | 具体条件与断言 |
| --- | --- |
| 0 | 空输出，完整 Domain/Path；四字段正确 |
| 1 | 复用输出，缺 Domain/Path；保留两个旧字段 |
| 2、3 | 只提供 Domain 或 Path；只覆盖所给字段 |
| 4 | 显式空 Domain；清空而非保留 |
| 5、6、7 | 空输入、HttpOnly、Secure；返回 false，五个原字段不变 |
| 8 | 160 字节初始 domain、161 字节 path、1024 字节值；逐字节且销毁完成 |
| 9 | Expires 固定日期；expiry 1445412480 |
| 10 | Max-Age=60；expiry 位于同次调用前后时钟各加 60 的区间 |
| 11 | 同一输出先写完整 cookie，再写缺省属性 cookie；两次具体状态均检查 |

每进程执行 12 场景，每种路径 5 进程，两架构共 6 格/30 次进程运行/360 次场景观察。**12 不是 12 种接口；360 不是 360 次独立进程运行**。其中：

- GNU 同侧控制：两架构各 60/60 场景正确，销毁 60/60。
- libc++ 消费方→修订 adaptor→真实 GNU provider：两架构各 60/60 正确，销毁 60/60。
- 故意关闭初始状态复制的同源码负向模式：两架构各 20/60 正确、40/60 错值；每进程均仅 4/12 正确并退出 28，但 12/12 作用域销毁完成。该负向模式仍调用真实 provider，不是模拟 provider。这是方法确实能检出状态遗漏的证据。

所有规范运行均有 maps 条件通过。ARM 的 provider SHA 与此前实测相同，见 `014_provider_identity.stdout`；上传库与本地 SHA 逐件一致。正时间不按跨进程字节比较，避免将走时误判为 adaptor 差异。

反方向 GNU 消费方→libc++ EWK provider：**NOT_OBSERVED**。前置阶段没有得到该真实 provider；组件编译失败和构建依赖说明已落盘。不是用 GNU provider 包装成 libc++ 外观后就宣称补齐反向。

## 已查接口中同类情况：明确分母

本轮定界复核 **11 个具名函数/重载定义**，不是整个 EWK 或 Dali API 普查：EWK parser 1 个；Dali `Property::Value::Get` 的 bool、float、int32、Dali::String、Property::Array、Property::Map 六个引用输出重载；XWalk Instance 的两种 SyncReply 写入及两种同步消息返回函数 4 个。具体源码行号见 `raw/005_get_state.stdout`、`003_similar_sources.stdout`，源文件 SHA 在 `source_identity.json`。

- **7 个有“未写入时保留调用前输出”的可观察条件**：EWK 1 个，以上 Get 六个。Get 对类型不匹配分支不写输出并返回 false；见 property-value.cpp:1146–1213、1362–1401。因此即使输出只含标量，也不能在失败时把零初始化临时值无条件拷回。EWK 是缺省字段部分保留；Get 是不匹配时整体保留，**不是同一个细节**。
- XWalk `SyncReplyToJS`、`SyncBinaryReplyToJS`（334–340 行）是赋值替换 instance 内部值，并不读取调用者输出结构。这 2 个不计入前述 7 个。
- 两种同步消息返回函数（270–306 行）调用回调后 move 内部 reply，另有无回调早返回。它们依赖 instance 生命周期与调用序列，计为另外 2 个状态序列候选，**未实测**，不声称与 EWK 同一种复用 bug。

原先已查 Native 四族所选包装及 Dali 其他重载未纳入这个 11 定义的分母，不能把 7/11 推成全平台比例。Dali 六个是静态核查，不新增“已运行”的接口数；全部外部接口中还有多少同类项：NOT_AVAILABLE，尚无全量语义审计。

识别方法：非 const 输出引用/指针是候选，不是证明；检查每条 return/异常路径的赋值覆盖，区分“缺省”“显式空”“失败保持”；再看实例缓存、swap/move、回调是否填值及未回调路径。用非空旧值、长短分配、重复调用及错误输入差分对照原 provider。头文件一般不能推出全部这些语义。

## 可反驳条件、限定与清理

若同一 provider/运行库身份、同样输入与旧状态下，修订原型相对同侧控制的正常返回值或字段再次不同，则本报告限定的状态修复结论被反驳。若要求原异常类型/部分写入、地址稳定性、迭代器寿命、分配时机、取消、并发可重入或反方向完全保真，现有矩阵**尚不能证明**；不能以本结果接受这些额外承诺。

`board_uploads.json` 六件上传按精确路径删除，`016_board_cleanup` 目标退出 0；独占目录不存在。`011_diagnostics_before` 与 `017_diagnostics_after` 清单无新增；`board_cleanup.json` 留证。物理板已清理并释放。未修改系统库、图形配置、平台源或既有补丁；保留其他会话文件。

脚本快照与 SHA256 在 `snapshots/` 和 `SHA256SUMS`。初版 front 源以 `frontend_initial.cpp` 留存；规范修订构建与运行命令前缀为 `v2`，tmp 单独目录，初版二进制身份/结果未覆盖。编排脚本的最终版本新增 version 参数，不声称最终脚本 SHA 就是第一次编排 SHA；历史实际命令完整归档。
