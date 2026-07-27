# round3 裁决判据模板

本模板用于 14 项候选的三方评审，不替代人工签核。

## 基本判据

1. 句柄/`void *` 只有在 provider 不解释 consumer-owned pointer、consumer
   不解释 provider-owned handle 布局时，才可建议
   `HANDLE_CONFINED_CANDIDATE`。
2. `callback != NULL` 时，**异常轴条目强制存在**：必须登记 callback 内的
   可抛操作、跨 DSO unwind 路径、封边点、抛出注入和二值 PASS/FAIL。
   grep 未发现 `throw` 不能豁免此轴。
3. callback 线程模型是显式前提：
   - `pkgmgrinfo_appinfo_get_list` 为同步直接调用，callback 与调用线程相同；
   - PPM async callback 的裁决前提是调用方在注册线程驱动 default GLib
     context，即 callback 线程等于注册/调用线程。若部署实际不满足，相关
     条目必须重开，不得继承签核。
4. borrowed `char *`/`const char *` 的对外有效期统一声明为：
   **至下一次同 handle 调用或 `destroy`，取先发生者**。consumer 必须在
   该点之前深拷贝，不得 `free` 或跨期缓存。
5. 证据不足必须保留 `INDETERMINATE`，不得静默转安全。
6. callback 存在时必须核查回调体对同 provider API 的再入；有再入则
   provider 状态结构（迭代器/引用）稳定性必须举证，否则“禁再入”必须
   写入接口契约。

## callback 异常轴 rider

每项 callback rider 同时满足以下条件才 PASS：

- 二值门：注入异常被指定封边点捕获，零 `terminate`、零跨 DSO unwind、
  零资源泄漏、零 stale request/部分结果误接受；任一非零即 FAIL。
- 时序门：P2 镜像组装前强制完成，不能降为 post-ship 测试。
- PPM 矩阵：mobile/wearable × 正常响应/错误响应/同步替换三分支；
  F4 旧实现双 invoke fixture 必须先复现 RED，修复后 callback count
  必须严格为 1。
- pkgmgr-info 矩阵：正常、provider error、consumer throw、callback
  error/partial-output 分支；catch 后不得把部分 vector 当成功结果。
- 错误码/可观察行为若因封边改变，人工只可在
  `ACCEPT_WITH_RELEASE_NOTE` 与 `FIX_WITH_SEAL` 中二选一；空栏表示尚未
  裁决。

逐项 rider 见 `g7_inventory_additions.tsv`。

## 变更记录

- 2026-07-25，signoff-prep：按
  `reviewer_claude round3 closure OQ-3b` 补入基本判据第 6 条。Q3 与人工
  签署均按启动决策推迟到正式开工阶段；本次仅使文书就绪，不表示三个
  rider 的 P2 二值门已经执行通过。
