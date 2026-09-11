# 平台 API 跨标准库验证：疑问与自行判断

## 已采用的执行解释

- 沿用已授权的隔离验证方式，临时构建物放 `tmp/API_0911/`，不进 git；证据与脚本放 `docs/progress/API_0911/`。
- 同侧控制未通过或 provider 未加载，只证明该测法尚未建立，不能证明接口不兼容。
- W2 任务书中关于编译单元与双库导出的概括需要核查适用前提；不将待核论断当作既定技术事实。

## 尚存疑问

1. **W1 产品依赖闭包**：`libelementary.so.1` 有 `elementary` / `elementary-tizen` 两个冻结候选。请确认本次产品/profile 及应采用的匹配 rootfs，不能自行选择。候选完整版本见 `API_0911/W1/load_results.json`。
2. **W1 实测前提**：Native app-control 的依赖在本次受限 ELF 候选检索中未取得 `libsessiond.so.0`；这不是证明全平台无该库。binder 的诊断批次 40 次尚未完成闭包，暂停续试，不声称已穷尽。
3. **W1 双方向**：未取得此三组目标的已迁移 libc++ 真实 provider 与运行上下文；没有据中断目录扫描声称它们不存在。需组件维护方提供或确认可复用的准确构建及初始化方式。
4. **W1 实验边界**：不以手写 vector 回调代替引擎实现；不以模拟 Dali provider 代替 binder→Dali。行为矩阵均保留 NOT_OBSERVED。
