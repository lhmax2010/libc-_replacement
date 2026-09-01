# QuickBuild 构建顺序判定

## 结论

仅凭 `Support: libc++-devel libc++abi-devel`，**不能确认 QuickBuild 会先构建
`libcxx-runtimes` 再构建消费者；不得依赖它实现自动排序**。当前环境无法读取
QuickBuild 服务端调度器版本、任务图或 buildroot 求解日志，平台实际行为记为
`NOT_OBSERVED`。

本机可核验的 OBS build core 提供了反向证据：

- `/usr/lib/build/Build.pm` 的 `get_build`（第 513–561 行）把 `required` 和
  `support` 都加入实际构建使用的包集合；
- 同文件 `get_deps` 上方注释明确说它返回“会影响其他包构建”的包集合，且是
  “get_build reduced by support packages”（第 616 行）；第 641–647 行又把
  `support` 从结果中过滤掉。

这说明在本机实现里，`Support:` 表示“放进 buildroot”，不是 source package
之间的可靠调度边。`gbs 2.0.8 qbbuild --help` 只列出 Full/Partial、包清单和
snapshot 等参数，没有声明 `Support:` 的平台调度语义。本机 build core 文件
也不属于已安装 RPM，无法据此确认 QuickBuild 服务端采用同一版本；所以这里只
作保守执行依据，不把它写成已观测到的服务端事实。

## 建议的两阶段执行

1. 第一阶段使用**尚未加入本次 `Support/Keep` 行的 bootstrap 配置**：先构建
   sandbox LLVM/Clang，再构建 `libcxx-runtimes.spec`，并把各架构生成的
   `libc++`、`libc++-devel`、`libc++abi`、`libc++abi-devel` 发布进第二阶段
   能看到的仓库/snapshot。`libcxx-runtimes.spec` 明确
   `BuildRequires: clang = 22.1.8`，所以 LLVM/Clang 在前。
2. 第二阶段应用本任务的两份 project_config 补丁，再启动其余包的全量构建。
   每个 buildroot 此时才能解析并安装刚发布的两个 devel 包。

第一阶段不能直接依赖已打 Base 补丁的配置。该配置既把两个 devel 包放入
`Support:`，又在 `Keep:` 中保留 libc++/libc++abi 四个包；按本机
`get_build` 的逻辑，构建 provider 自己时存在要求其尚未生成子包的自举风险。
若 QuickBuild 有经过验证的 bootstrap 例外或显式阶段依赖，可把“两轮”做成
一次平台编排中的两个阶段；在取得证据前，不应假设它存在。

## 人工确认检查点

QuickBuild 平台侧至少核对以下各项：

1. 包清单/任务图是否把第二份 `libcxx-runtimes.spec` 识别为独立 source
   package，而不是只构建 `llvm.spec`；
2. 任务图是否存在 `llvm -> libcxx-runtimes -> consumers` 的显式边；
3. provider 成功后，四个二进制包是否已发布到消费者所用的同一
   repository/snapshot；
4. 代表性消费者启动前，buildroot 求解日志中是否显示
   `libc++-devel-22.1.8-*` 和 `libc++abi-devel-22.1.8-*` 的准确 NEVRA 与仓库
   来源；
5. 是否出现 `nothing provides libc++-devel`、`unresolvable`，或选择了旧
   NEVRA；
6. 消费者的 `/usr/include/c++/v1` 是否来自新 `libc++-devel`，并包含层 C
   头文件防护；
7. 若平台声称单轮自动排序，保存任务图和至少一个消费者“等待 provider 发布
   后才启动”的时间线证据。

以上任一关键项不可见或不成立时，使用两阶段方案。
