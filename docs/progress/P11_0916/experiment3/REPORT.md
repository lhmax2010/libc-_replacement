# 真实反方向 EWK 矩阵的前置复核

结论：libc++ 构建的真实 EWK provider 为 **NOT_AVAILABLE**；GNU 消费方→libc++ provider 的 12 场景运行矩阵为 **NOT_OBSERVED**。按人工续任务要求，不阻塞文档修正。

## 已试方法与实际结果

1. 复核既有构建可行性报告与原失败命令：[原报告](../../ADAPTOR_0912/W1/REPORT.md)、[本轮记录](raw/002_prior_feasibility.stdout)。产品参数仍沿用已确认的 elementary-tizen，不自行换产品或改源码。
2. 重新执行原真实 `ewk_cookie_parser.cc` 编译命令，**唯一变化是输出写到本轮 tmp**，没有改 parser 实现，也没有执行会覆盖旧记录的原脚本。源文件 SHA256：`a903f1a977ce4553339710fa0dfa0424a53479d6545629911a54537861d24d80`。实际仍退出 1，缺少 `base/functional/callback_forward.h`，连本翻译单元 `.o` 都未产出。[完整命令](raw/003_component_retry.command.txt)、[完整 stderr](raw/003_component_retry.stderr)、[结构化结果](PREREQUISITE.json)。
3. 再核对真实 GN 构建目标：chromium-ewk 共享库目标输出 chromium-impl，包含 parser，但还依赖 base/content 等；不是只编译一个 cookie 函数即可替代的现成小目标。PATH 中 gn 本次未找到，ninja 可用；这只说明所查 PATH，不证明磁盘上无任何 GN。[实际输出](raw/005_existing_output_paths.stdout)。
4. 上一阶段冻结 ELF 清单提供的是已识别 GNU 组件及 SDK 对应产物，没有取得新的、可验证为 libc++ 的 EWK 实现库。未将 GNU 库改名、薄壳、复制的 parser 或模拟组件当作反向 provider。

## 所缺与结论强度

需要该产品版本的完整源码及生成头、base/net 等依赖闭包和可复现构建目标，或者组件方交付真实 libc++ EWK 产物与身份、构建配置、依赖清单。当前不能以一次缺头错误证明技术上永远构建不了，也没有可靠吞吐数据估计全量构建时长。本轮沿用原断点，不擅自选择新的组件裁剪/重写方案。

由于前置不成立，两个架构的反向直接调用/adaptor 对照均未运行，不能填造 5/5 或用既有 360 次相反方向场景观察代替。没有占用开发板、没有新板上上传或残留；编译线此前释放记录已在实验 1 的状态查证命令中归档。

自行判断仅限重跑原最小组件编译并换隔离输出路径；没有修改平台源码、配置、既有实验材料或实施任何部署方案。
