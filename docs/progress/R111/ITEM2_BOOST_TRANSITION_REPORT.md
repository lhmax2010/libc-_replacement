# Boost 边界在 Base 适配后的状态核查

## 结论

本项由静态清单核对和已构建产物的符号核对组成，不实施迁移。

- 90 条边的 13 个 consumer source RPM 全部出现在既定 483 包迁移清单中：2 个
  位于 `PHASE1`，11 个位于 `PHASE2`。因此“是否在迁移计划范围内”的答案是
  13/13；本项没有取得这 13 个包已经以 libc++ 重建的产物，实际完成状态统一记
  `NOT_OBSERVED`，不能把计划成员写成已迁移。
- 90/90 条边的 provider 一端都是 `boost` 源码包产生的五类二进制子包：
  Boost.Filesystem 30、Boost.Iostreams 3、Boost.Program Options 6、Boost.Test
  42、Boost.Thread 9。该 90 条集合中没有“不涉及 boost”的边。
- Boost 适配提交
  `4168e873584ae50a12733262026c007dce18ec89` 只改 `packaging/boost.spec`，在 Base
  Toolchain 选择 Clang 时把构建和安装阶段统一切到 libc++；GCC 和非 toolchain
  构建保持原行为。三架构候选产物已验证上述五个 provider 子包使用 libc++、不
  依赖 libstdc++。
- 如果部署 libc++ Boost 时 consumer 仍是原冻结发布物，则这 13 个 consumer 都
  属于“尚未迁移的一侧”。旧记录中的边不会统一消失：对真实 aarch64 适配 Boost
  provider 复核 30 条旧边，30/30 至少保留一个同 mangled name 符号交集；其中
  15 条旧符号集合全部保留，15 条只保留部分。该检查证明仍存在可绑定的旧符号
  交集，也证明部分 ABI 名字已经改变；它不把逆向混用实验的具体故障形态外推到
  新方向。

## 输入身份

- 90 条旧边：`docs/progress/R82/tables/boost_edges_90.tsv`，SHA256
  `cfe8bf8749b018a7a5da2a79ba496276d60282fa0a61f5b1c183a5a05f353b03`。
- 最新分类/实测状态：`docs/progress/R110/tables/w3_edges_90_results.tsv`，SHA256
  `cba302177b41c81ba76f139c067be97edd9e0449c2398afb8093fa40df7c9177`。
- 迁移清单：`docs/progress/R24/tables/migration_packages_483.tsv`，SHA256
  `3d56ac7be96467394989a3b98a3738459d92185659a8bf8369b43d8c8fde7688`。
- Boost 六格构建矩阵：`docs/progress/R105/BOOST_MATRIX.tsv`，SHA256
  `348281b68f728b84b18fe9ccc4e352a1d2584cb8b52f80f6a7c8369dd5808595`。
- 适配提交推送核对记录 SHA256：
  `c7261b9c7bbd9fd7a54240f3b027132831f445588b729c68bf91bbd4a8f0c2eb`；记录中的
  local、remote 均为完整提交 `4168e873584ae50a12733262026c007dce18ec89`。

该提交不属于当前材料仓对象库；它位于只读包仓
`codes/R104/boost`。本项先在当前仓和 sibling 根仓查找均得到 unknown revision，
随后由既有报告中的精确包仓路径定位，没有把“当前仓没有对象”写成“提交不存在”。

## 13 个消费方逐项核对

“计划状态”来自精确 `source_rpm_identity` 连接，不以包名前缀猜测。完整机器表见
`tables/item2_consumer_migration.tsv`。

| consumer source RPM | 旧边数 | 旧边 provider | 计划阶段 | 适配完成产物 |
|---|---:|---|---|---|
| `askuser-notification-0.8.4-1.src.rpm` | 3 | Boost.Test | `PHASE1` | `NOT_OBSERVED` |
| `capi-appfw-capmgr-0.0.4-1.src.rpm` | 3 | Boost.Program Options | `PHASE2` | `NOT_OBSERVED` |
| `cert-svc-3.0.0-0.src.rpm` | 3 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |
| `device-certificate-manager-3.0.1-0.src.rpm` | 3 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |
| `dotnet-launcher-8.0.11-1.src.rpm` | 24 | Boost.Filesystem | `PHASE2` | `NOT_OBSERVED` |
| `key-manager-0.1.90-1.src.rpm` | 15 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |
| `libcryptsvc-0.0.2-6.src.rpm` | 3 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |
| `libphonenumber-7.0.6-1.src.rpm` | 3 | Boost.Thread | `PHASE2` | `NOT_OBSERVED` |
| `libwebappenc-0.1.1-1.src.rpm` | 6 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |
| `libzypp-17.23.7-1.src.rpm` | 3 | Boost.Thread | `PHASE2` | `NOT_OBSERVED` |
| `mic-bootstrap-1.0-1.src.rpm` | 3 | Boost.Thread | `PHASE2` | `NOT_OBSERVED` |
| `security-manager-2.4.0-0.src.rpm` | 18 | Filesystem、Iostreams、Program Options、Test | `PHASE1` | `NOT_OBSERVED` |
| `yaca-0.0.6-0.src.rpm` | 3 | Boost.Test | `PHASE2` | `NOT_OBSERVED` |

所以没有“不在迁移计划内”的 consumer。另一方面，“都会改用 libc++”目前只能
表述为计划范围事实；本项材料不能证明 13 个包都已经完成适配、重建和发布。

## Boost 适配覆盖的 provider 面

提交在 `%{defined _toolchain}` 且 toolchain selector 为 Clang 时定义
`build_with_libcxx`，增加 libc++/libc++abi 构建依赖，把 `-stdlib=libc++` 和
`-lc++abi` 传给 Boost.Build，并在 `%build` 与 `%install` 的第二次 `b2 install`
之间保存、恢复同一组 flags。提交不修改 Boost C++ 源码、SONAME 或 consumer。

三架构 libc++ 构建矩阵：

| 架构 | ELF | 依赖 libc++ | 依赖 libstdc++ | 中性 ELF | `std::__1` 标记 | `std::__cxx11` 标记 |
|---|---:|---:|---:|---:|---:|---:|
| armv7l | 39 | 31 | 0 | 8 | 1792 | 0 |
| aarch64 | 36 | 29 | 0 | 7 | 1762 | 0 |
| x86_64 | 39 | 31 | 0 | 8 | 1799 | 0 |

真实 aarch64 候选中的五个相关 DSO 均存在，`DT_NEEDED` 均含 `libc++.so.1` 和
`libc++abi.so.1`，未观察到 `libstdc++.so.6`。所以在 Base Toolchain 的 Clang
条件成立时，90 条边所涉及的五类 Boost provider 都在该 Boost 包适配的构建覆盖
内。GCC 与非 toolchain 格按提交定义不会切换，这个条件不能省略。

## 适配后的旧符号交集

为避免把“boost 包完成适配”误写成“旧边自动消失”，本项把 aarch64 冻结 consumer
记录的每条旧精确 C++ 符号集合，与真实 libc++ Boost 候选 DSO的动态定义符号逐个
连接：

| 旧边分类 | 至少一个同名符号仍存在 | 无旧符号交集 | aarch64 边数 |
|---|---:|---:|---:|
| 布局敏感 | 21 | 0 | 21 |
| C++ ABI 耦合但布局未证明 | 9 | 0 | 9 |
| 合计 | 30 | 0 | 30 |

30 条中 15 条的旧记录符号全部仍存在，15 条只保留部分；495 个去重前逐边旧符号
记录中，448 个在新 provider 仍有相同 mangled name。完整逐边值和保留符号见
`tables/item2_aarch64_old_consumer_symbol_overlap.tsv`。

这组结果只回答符号交集：

- 同名仍存在的调用可能继续绑定，但旧 consumer 与新 provider 对 C++ 对象的布局
  和所有权解释不同；本项没有执行“libstdc++ consumer → libc++ provider”的新
  方向运行用例，因此具体表现为 `NOT_OBSERVED`。
- 已改名的旧引用可能在静态链接或动态装载时暴露为未解析；是否发生取决于该
  consumer 的实际引用集合和链接/装载方式，本项没有重建 13 个 consumer，结果为
  `NOT_OBSERVED`。
- 先前运行实验是相反方向（libc++ consumer → libstdc++ provider），已观察到
  错值、终止和析构失败；不能把那些具体次数或故障类型直接填入新方向。

armv7l 和 x86_64 的构建矩阵证明 provider 运行库选择一致，但对应解包产物在本项
精确符号复核时并非都可得；逐边新旧符号连接只标为 aarch64，不向另两架构外推。

## 过渡期事实

在“新 Boost 已部署、某个 consumer 仍是冻结 libstdc++ 产物”这一明确前提下，
该 consumer 与 Boost 的边界由原先同为 libstdc++ 变成不同 C++ 运行库。13 个
consumer 都有这种条件路径，因为它们都有旧边且本项没有观察到已迁移产物。

风险形态由两部分组成：保留同 mangled name 的边仍可能跨 ABI 传递对象；部分
改名的引用还可能不再解析。实际产品是否同时安装这两批产物、每条边在该窗口中的
具体动态结果，都为 `NOT_OBSERVED`。

时序上存在以下事实可能性，不代表推荐：

- consumer 的 libc++ 重建需要可用的 libc++ Boost provider，故构建完成时间可以
  是 provider 在前；
- 构建完成时间与进入同一个发布仓/镜像的时间不是同一个动作，技术上可以把
  provider 与一组 consumer 在同一发布快照切换；
- 也可以在 consumer 准备完成前仍让产品镜像引用旧 Boost 构建；是否允许同时保留
  两套包、是否支持原子仓快照或回滚，本项没有发布系统材料，记 `NOT_AVAILABLE`；
- 现有迁移表把 Boost、`askuser-notification`、`security-manager` 放在
  `PHASE1`，其余 11 个列在 `PHASE2`。若发布严格按阶段立即替换 provider，阶段间
  会存在上述条件窗口；现有表没有声明“每阶段立即部署”，不能据阶段号推断窗口
  一定发生。

## 是否存在不涉及 Boost 的边

不存在于这 90 条输入集合中。机械核对的 provider binary 计数为 30+3+6+42+9，
合计 90，均是 Boost 子包；正向对照就是五个已知 Boost provider 类别全部命中。

这不表示平台所有 Base–Unified C++ 边都涉及 Boost；它只限定本任务所称的“90 条
Boost 语义边”。Boost 适配后，这组历史边的调用关系不会因 spec 只改工具链 flags
而自动从 consumer 源码中删除，但其精确符号交集和运行库组合会发生上述变化。

## 脚本与限制

- `code/analyze_boost_transition.py` SHA256：
  `2614e6af44bba799dc3ff6afe98dd81a0e5a201d030b5cc9d9df392cf769db23`。
- 脚本断言：90 条输入、13 个去重 consumer、90/90 provider 为 Boost、13/13
  精确命中迁移计划，以及 30 条 aarch64 边完成候选符号连接。
- 初次在当前材料仓和 sibling 根仓读取 `4168e873` 均返回 unknown revision；精确
  包仓定位后成功。一次对外部 artifacts 目录的递归 `find` 因存储延迟被精确终止；
  随后以五个已知包名和 DSO 名直接访问完成核查。失败尝试原样保留。

本项没有修改 Boost、consumer、平台源码或任何补丁，不给迁移时序建议。
