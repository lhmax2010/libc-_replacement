# 等待规格、写者回滚与析构改动

> 2026-09-11 元数据同步：正文提交引用已换为补齐 Change-Id 的等树新提交；原构建／验证时间与结果不变，历史原始记录仍对应旧提交。[新旧映射与树核验](../../P6_0909/resume/SHA_MAPPING.tsv)。

结论：**PATCHES_READY**。约 18 分钟，未触及三小时上限；起止以 `raw/001_resource.time.json` 和推送后记录为准。

## 分支与提交

先核实并申报基线 `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa`，再在 `tmp/IMPL_0908/source/` 的独立、稀疏仓库建立 `sandbox/lhmax2025/libcxx-noexcept-relief`。对象只读共享，不修改 `codes/` 工作树或它的 refs/config。

| 提交 | 改动 |
|---|---|
| `16a73b0af4d7f9194ae3a12d4934965d2f4005b7` | 四种等待去规格，现代声明／定义与 C++03 镜像同步；通知规格不变 |
| `72e893320c94db78b8661441628dfe9ba1e2cb02` | 普通／定时写者完成守卫；异常时在内部锁尚持有时清写者位、通知第一队列 |
| `8e8335839aad10eb1ac876c49afe32355ff51cf5` | 现代 wbuffer_convert 析构不调用 __close，保留资源删除、helper 与 ABI 标注 |

三个提交均有英文说明与中文正文，不以内部编号解释语义；完整 diff／消息在 [snapshots](snapshots/)。sandbox 已正常推送，远端 SHA 等于第三提交。原基线 sandbox、`tizen_base`、`tizen_base_next` 推送前后 SHA 均未变。[推送](raw/043_push.stderr)、[远端核验](raw/044_post_push.stdout)。服务端关于标题／正文长度的格式警告已原样保留；不是构建警告，未为修改已推提交而使用 force。

## 实施范围与保留项

普通与 system-clock 两个库函数的现代头文件声明必须同时去规格；若只改源文件定义会产生声明不一致。steady/custom 的头内声明、定义，以及 C++03 四个成员均同步。守卫与此前验证的顺序相同：先占写者位，再创建守卫；成功及原有正常超时清理后标记 completed。保留已有读者计数，无对象布局改动，没有加入 call_once 或任意用户回调的回滚。

析构改动仅在指定的现代头文件，不推断其他语言模式或镜像设施已同步实施。`__close` 的原功能保留，标注此类已无内部调用；显式 `pubsync()` 未改。

四既有补丁文件 SHA 不变。对四补丁涉及的所有源文件与基线逐字比较，全部相等；五个头文件为 `cxxabi.h`、`__ostream/basic_ostream.h`、`future`、`istream`、`string`。新构建头目录亦逐项查到对应强制展开处理。[完整保留检查](raw/019_preservation.stdout)、[构建头及 ELF](raw/025_build_identity.stdout)。

## 构建与实测

x86_64 原生宿主；Tizen Clang 包装器、完整源码 CMake libc++／libc++abi 构建，GNU 展开器、现有 LLVM_22 版本脚本。不是重链接少量替换对象的覆盖库。匹配基线来自同一远端基线，另建独立源码／构建目录。

严格构建最终均成功，`-Werror` 能力检测已通过，修改版与基线编译日志均无 warning/error；检索以前序实际驱动警告作正向对照。CMake 的 CMP0219 和缺 Clang 开发包提示两侧共有，后者意味着 clang-tidy 子测试不具备环境，不冒充官方套件全可运行。[修改版](raw/030_build_strict.stdout)、[基线](raw/032_baseline_strict.stdout)、[核验](raw/034_static_build_gate.stdout)。

每次轻量探针先核实实际加载这份完整 libc++ 与 libc++abi：

| 用例 | 次数 | 具体结果 |
|---|---:|---|
| 普通等待、system、steady、自定义时钟 | 各 5 | canceled join，外层清理 1；头内路径随新头重编 |
| 新头同步／获取与析构样例 | 5 | 普通写／读锁、定时获取成功；显式同步输出精确为 abcdef；未同步样例析构前后均只有 a |

正式轻量矩阵共 25 次：[smoke.tsv](smoke.tsv)、[执行总记录](raw/033_smoke_final.stdout)。不是替代后续完整两架构取消、正常路径及官方套件验证。

## 失败过程和校准

- 稀疏源码首次缺少 LLVM libc 的共享头；补齐源码后需重新 CMake 配置才能建立头依赖。两次缺头失败原样保留，未改平台源码绕过。
- 编译阶段误沿用链接专用 `--rtlib=libgcc`，引出驱动警告，并使 `-Werror` 功能检测失败。最终只在两套隔离配置的链接阶段保留该选项，重新检测并全量重编成功；未屏蔽源码警告。
- 首个析构样例错误地预期“无同步就零输出”，实际首次 overflow 已输出 a；失败退出 4 保留。核对源码后，断言改为析构前后精确相等及显式同步完整，记录有 5 字节缓冲内容未写出，而非把初始失败改记成功。
- 初次五头清点猜错路径，失败保留；改为从四补丁的 diff 文件集合导出真实五头及全部涉及源文件，再逐字比较。

脚本、构建配置和 patch 快照 SHA 在 [SNAPSHOT_SHA256SUMS](SNAPSHOT_SHA256SUMS)。原始命令和退出码逐次保存于 raw；少量会话中的只读展示仅留会话，不伪称已全部另行归档。

## 后续限定

本阶段仍是单版本库；旧调用方的双版本契约由下一阶段实现。没有连接开发板、没有平台安装。`packaging/SOURCE_PROVENANCE` 仍记录此前 validated 子树／LLVM_22 锚点，本阶段未把它改写为“新方案已完成全量验证”；新源码与构建物身份另在本报告和原始记录给出，不能用旧锚点代替新方案发布验收。
