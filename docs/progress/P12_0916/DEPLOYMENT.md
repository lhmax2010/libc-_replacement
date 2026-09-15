# 构建、部署候选与复核入口

**状态：发布阻断。** 本文给出可审查的放法与操作入口，不是已经完成产品部署的说明。后端分配失败、生产方向和真实回调缺口见 [报告](REPORT.md)。两种放法由人工选择；本轮没有应用补丁到平台源码。

## 1. 文件与来源

- `code/src/ewk_front.cpp`：应用侧编译单元；原符号和唯一转发入口。
- `code/src/ewk_back.cpp`：实现侧编译单元；调用真实 provider，保留调用前状态。
- `code/src/bridge.h`：仅 POD、字节借用区和整数错误返回。
- `code/src/provider_loader.h`：固定路径/特定句柄、根 DSO 归属、自循环检查。
- `code/src/vector_{front,back}.cpp`：vector 数据层；`vector_provider.cpp` 明确是构造测试接收器。
- `code/src/*_app.cpp`、`protocol_unit.cpp`、`allocation_contract.cpp`：值断言、错误、边界及失败复现。
- `code/upstream/`：真实 SRPM 中转发目标源文件/GN/spec 的只读证据副本。身份见 SOURCE_INDEX；不是完整 Chromium 源码包。
- `code/testconfig/build/build_config.h`：独立 non-TV 测试配置，**不得安装到生产源码树**。

EWK 语义来源：原 `tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc`，SHA256 `a903f1a977ce4553339710fa0dfa0424a53479d6545629911a54537861d24d80`。第 32–34 行解析/拒绝在写输出之前；36–37 行更新 name/value；39–49 行按属性是否存在更新 domain/path；54–77 行决定 expiry。源码注释中的行号指此版本。完整原文/行号见 `raw/046_source_and_identity.stdout`。

## 2. 两种放法和 spec 条件

共同应用（在产品源码副本，不能把本文当成已执行记录）：

1. `0001-route-cookie-to-explicit-adaptor.patch` 修改 `tizen_src/downloadable/ewk_api_wrapper_generator.py`，其他 wrapper 仍沿用原实现句柄。
2. `0002-gn-explicit-link-inputs.patch` 修改同目录 `BUILD.gn`，并加入唯一入口声明头；通过显式 GN 参数传入预构建对象/DSO 和匹配的运行时链接参数。
3. 只选择下面两份 spec 补丁之一，不能两份一起用。

| 形态 | spec 补丁 | 包内安装内容 | 条件 |
| --- | --- | --- | --- |
| 独立 DSO | `0003-spec-separate-CANDIDATE.patch` | 更新 libchromium-ewk.so；同包 `%{_libdir}/libewk-cookie-adaptor.so.1` | 动态加载器能在受管理的系统 libdir 找到 SONAME；两个文件原子随同包交付；无用户可写搜索目录 |
| 内嵌 | `0003-spec-embedded-CANDIDATE.patch` | 更新 libchromium-ewk.so，不另装 adaptor DSO | 两套 .o 及运行时链接参数传给 ewk-interface；整个 DSO 的符号、分配与异常合同需要验收 |

独立形态通过 DT_NEEDED + 唯一入口决定路由，不需要“先加载某个库”。实验中的 `$ORIGIN` 只用来在 tmp 定位同目录候选；spec 候选安装到产品系统 libdir，不把实验路径或 LD_PRELOAD 写入镜像。实现路径编入后端配置，必须是产品可信、版本匹配的实际实现路径；当前实验路径不得进入生产包。

spec 候选要求 `ewk_adaptor_payload` 指向本交付 `code/` 内容，`ewk_adaptor_config` 指向经产品确认的 JSON；故意不虚构 Source 编号、未来 libc++ 开发包名字或 SDK 编译选项。payload/config 应作为可追溯的打包输入纳入产品源包，不能依赖构建机的临时目录。配置缺失即失败。候选在现有 GN graph 中增加参数，再只重建 `ewk-interface`；原 spec 的常规后续构建逻辑保留，不表示本轮执行过完整构建。

`package_build.py CONFIG OUTPUT --mode separate|embedded` 会串行构建两个 TU；内存上限为 min(4 GiB, 物理内存 50%)，nice19/ionice3，先检查至少 1 GiB 空间。OUTPUT 必须是新目录。此脚本只编小目标；完整产品构建另需遵守任务要求的时限、I/O 探测与资源约束。

JSON 必须包含：`compiler`、`gnu_flags`、`libcxx_flags`、`includes`、`link_flags`、`provider_path`、`provider_abi_evidence`、`consumer_abi_evidence`、`direction`。后两项必须能追溯到实际编译/头文件配置，不能只写“这是 libc++”。`direction=production` 选择 GNU front + libc++ back；`reverse-mechanism` 相反。当前成功的小目标配置见 `package_reverse.json` 快照，它只适用于当前实验路径/产物。

必须核查 GNU ABI=1 是否匹配已发布消费者，以及 libc++ ABI 命名空间、头文件配置与实现是否匹配。哨兵会阻止已知尺寸/头文件类别漂移，但不能证明未知配置兼容。本轮两份 patch 都经 `git apply --check` 并在独立副本应用，两种 package_build 均成功；**GN 生成、RPM 构建/安装及产品依赖解算仍是 NOT_OBSERVED**。

## 3. 当前环境从空构建目录复现

在拥有 INPUT_ARTIFACTS 中真实产物以及原 SDK 头文件的工作区：

```bash
python3 docs/progress/P12_0916/code/replay.py \
  --workspace /home/toolchain/development/libcxx_runtime_validation \
  --platform-workspace /home/toolchain/development/libc++_replacement \
  --output /home/toolchain/development/libcxx_runtime_validation/tmp/P12/replay_new
```

输出目录必须不存在且位于该工作区的 `tmp/P12/` 下。脚本从快照复制源文件，调用真实生成器、构建两 TU/转发源目标、运行 12 场景、边界、单元、发布候选和分配失败复现。所有运行命令/退出码落在新目录 `evidence/`。这不是在没有 SDK/RPM 的任意机器上自给自足的工具；缺材料不能以模拟件替代。

注意：`allocation_check.py` 的功能是**完整记录已有失败**，自身成功退出只代表收集完成；必须读 ALLOCATION_CONTRACT_RESULTS，不能据 replay 退出 0 签发通过。

## 4. 生产方向补测

`future_ewk_matrix.py MANIFEST OUTPUT` 已可执行；缺真实产物/manifest 返回 77、打印 NOT_AVAILABLE。manifest 需填写：

```json
{
  "direction": "gnu-consumer-to-libcxx-provider",
  "provider_build_evidence": "实际编译与ABI配置记录文件",
  "loader": "实际绝对路径",
  "library_path": "受测依赖目录列表",
  "provider": "真实libc++实现绝对路径",
  "app_gnu": "按GNU应用侧头文件编译的ewk_app",
  "app_libcxx": "按provider匹配头文件编译的ewk_app",
  "control": "同批真实转发源目标无adaptor",
  "separate": "生产方向独立adaptor转发目标",
  "embedded": "生产方向内嵌adaptor转发目标",
  "sha256": { "loader": "填实际SHA，其余六个产物键同样逐项填写" }
}
```

此块是字段说明，不是有效 manifest；脚本逐项验证文件及完整 SHA，缺项不会继续。生产侧编译用 `package_build.py` 配合真实产品配置；应用用现有 `ewk_app.cpp`，分别选择对应标准库头文件，链接/运行时配置同 provider 的实际产物。生成器及 main 必须来自该产品版本。

脚本执行 libc++ 同侧正向对照、GNU 两种 adaptor、GNU 不带 adaptor 四格，各 5 次，逐次断言 maps 和 12 场景/销毁。无 adaptor 格记录实际观察，**不预设生产方向也必然 SIGSEGV**。当前只验证了缺前置时返回 77；真实矩阵未运行。

## 5. 真实回调补测入口与剩余工作

`future_callback_matrix.py MANIFEST OUTPUT` 定义真实宿主 driver 的编排入口：输入实例案例及二进制字节，输出实际回调的字节/长度、源销毁、接收方释放、maps。5 类输入各 5 次。manifest 给 `host_command` argv 和 `real_provider_path`。

**尚缺真实 host driver，不是已写完 XW 注册实现。** driver 必须连接真实插件加载、实例注册、SetSyncBinaryReply、消费响应及销毁链；JSON 是测试编排协议，不是平台 API。需另补与实际同侧宿主的对照。不能把本轮构造接收器接到这个脚本后宣称真实路径已验。缺前置返回 77 的分支已运行。

## 6. 部署验收/回退条件（未执行）

安装前关闭本报告的技术阻断，确认旧名称入口对应哪种应用 ABI；不要让一个旧名自动承载两种未知 C++ 布局。对产品 package 检查 `readelf -dWs`：独立形态的 DT_NEEDED/SONAME、原入口、唯一入口、无测试导出、无 tmp RUNPATH。以生产方向矩阵、错误路径、真实回调和实际包安装/升级结果验收。

产品发布系统需成组交付/回退转发层、adaptor（如独立）、匹配实现与依赖；进程重启释放旧映射。版本归属、包名及回退事务由产品构建/发布流程落实；本轮没有安装、卸载或替换任何系统库，也没有执行上述部署操作。
