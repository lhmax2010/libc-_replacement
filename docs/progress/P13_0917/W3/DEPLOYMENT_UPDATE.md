# 修复版构建与部署边界更新

当前状态：**RELEASE_BLOCKED：普通异常边界已修复，但真实 provider 分配失败后的完整清理保证未成立；析构双异常要求待裁决。** 不实施平台、不安装镜像。

## 当前代码入口

当前候选是本目录 code/ 下的源码/脚本快照，不是 P12/code 的历史快照。工作改动仍在 tmp/P12。W1、W2 各阶段快照与已推送证据保持原样，用于追溯。

1. 保留 P12 已给的两种转发放法：独立 DSO 的明确 DT_NEEDED/唯一入口，或两 TU 内嵌。P12 的 generator/GN/spec 候选 patch 继续作为基础，不改变平台源码。
2. **新增必需步骤**：每个 adaptor TU 编译后运行 `pin_runtime.py`，以该侧实际 runtime 的默认版本导出为依据绑定引用，同时隔离 weak/COMDAT 辅助定义。只有固定 x86_64 配置开放；关键异常/分配符号没有受支持的版本时拒绝构建，不默默回退到加载顺序。
3. 当前成功 linker 是 GNU gold 2.42。原工具链 linker 的版本化 vtable 链接失败、no-relax 对照均保留。没有更换系统 linker；构建命令明确指定 `-fuse-ld=/usr/bin/ld.gold`。产品工具链能否提供兼容 linker，需实际 GN/RPM 验证。
4. `package_build.py` 配置在 P12 字段之外，新增必填 `gnu_runtimes`、`libcxx_runtimes`（实际 ELF 路径数组）和 `linker`。见 PACKAGE_CONFIG.json。不能把其中当前 tmp 路径放入产品包；产品侧须提供匹配真实 ABI/头文件/库的配置。
5. 独立 DSO 和内嵌对象两种小目标均已用新构建入口编译。发布候选重编了**没有测试宏的后端和前端**，并重跑 12 场景矩阵：两放法各 12×5 通过，同侧对照通过，混库无 adaptor 对照仍失败。见 RELEASE_RESULTS.json。测试 allocator 与 p13_test_* 不装入发布 DSO。

GN/spec 补丁本轮没有在实际产品构建环境执行；将编译器 driver 参数加入 GN 链接配置也需按该产品 toolchain 规则复核。不能把独立小目标构建成功写成 RPM/镜像安装验收成功。两种部署形态仍由人工选择。

## 复核

在已备齐 P12 真正 SDK/源副本的工作区，`p13_driver.py` 使用新的 build_P13 和分阶段证据目录；W1 原失败及状态保持、W2 回归/扩展/计数、W3 契约对照分别有入口。重跑必须使用新的记录前缀/目录，不能覆盖已提交原始输出。

`p13_w3.py package` 生成的输出目录必须不存在。package manifest/各对象绑定 JSON 给出版本映射和 SHA；发布候选的符号检查要确认没有 p12_test_*、p13_test_*。动态 runtime 的 SHA/ABI namespace/头文件配置必须一同审核。

公开 EWK 始终返回 bool；同步异常转 errno、失败输出不变的具体范围见 ERROR_CONTRACT.md。原始 GNU API 在分配失败时可抛 bad_alloc；adaptor 返回 false/ENOMEM 是有意改变，不能说完全一致。

## 不可跳过的剩余项

真实 provider 的异常清理缺口、析构双异常要求的裁决、生产方向的 libc++ EWK 产物与验证、armv7l、并发、真实回调、产品 linker/GN/RPM/升级回退。P12 预备的生产方向及真实回调编排脚本继续保留，仍缺真实输入/driver，不能冒充已经可运行的完整业务路径。
