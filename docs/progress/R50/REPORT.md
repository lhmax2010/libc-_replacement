# R50：armv7l 上的层 C 站点分类复核

## 结论

armv7l 的逐站点符号分类与 x86_64 一致：34 个非汇聚站点中，18 个
可由 `libc++.so` 交付（其中 2 个经显式实例化载体交付），16 个由
消费者持有。计入既有的 16 个 `ios.cpp` 汇聚站点后，armv7l 仍为
**库内批 34 / 头文件批 16**。两架构按更严格一侧合并后的数量仍为
**34 / 16**。

板上原生对照验证了两个库侧站点 `S05` 与 `S08`：只更换补丁
`libc++.so`、不重编消费者时，两个用例都从退出 134 变为退出 0，
`pthread_join` 均返回 `PTHREAD_CANCELED`；镜像格“补丁头重编消费者
+ 旧库”仍退出 134。该结果与符号级 `库侧导出 + 消费者 UND` 判定
一致。

因此，“按改动能否由库交付划分、头文件批必须在首批消费者编译前
就位”的分批框架在 armv7l 上仍成立。本报告只记录事实，未修改已
落章正文。

## 0. v4 重建版身份标注

第零节先行完成，并以独立提交
`042e0750713b58647eb3ec793af9de143cde6307` 推送。仅
`docs/decisions/README.md` 增加了任务书给定的逐字说明；落章正文与
所有历史版本未改。

- `solution_definition_v4.md` 总计 351 行。
- 独立逐字节交叉比对一致：317 行，占 90.313390%。
- 317 行之外：34 行，占 9.686610%。
- 351 行均来源于当日第一份完整只读评审原始输出；其中第 35–351 行
  又由第二份分段只读评审原始输出独立印证。第 1–34 行只有第一份
  原始输出这一条来源，故无独立佐证，不作为依据引用。

原始盘点、SHA、行数计算、提交和远端核验见 `zero/`。

## 1. 输入、基准库与方法

方法完整申报见 `METHOD.md`。R47 输入与方法材料的 SHA256 见
`commands/007_r47_method_and_inputs.log`。

主判定库使用平台发布 RPM：

- RPM：`libc++-22.1.8-19.1.armv7l`
- RPM 路径：`tmp/R12/downloads/libc++-22.1.8-19.1.armv7l.rpm`
- RPM SHA256：`9bda3e524b7ff5384c44baa1d85808ec71cc78db7c17bf71726e24c6c6bd4c99`
- 库路径：`tmp/R12/extracted/platform_llvm_runtime/armv7l/usr/lib/libc++.so.1.0`
- 库 SHA256：见 `commands/008_arm_library_identity.log`

另与本项目 C2 GBS 版对照。两者去掉符号版本后，1968 个导出名称
逐一相同；发布 19.1 库没有符号版本节点，C2 库的同名导出带
`LLVM_22`。这一差异改变版本形态，不改变本任务所测站点是否导出。
原始差异见 `commands/009_*`、`010_*`、`012_*`。

消费者探针采用 armv7-a / cortex-a8 / NEON / softfp / Thumb 与
hidden visibility 的生产式选项，在 O0、O2 两档实测。完整命令和
目标文件 SHA256 见 `commands/011_build_arm_probes.log`；完整 nm 原文
见 `raw/nm/`。

## 2. 符号级分类结果

逐站点结果在 `tables/armv7l_site_classification_34.tsv`。

| 非汇聚站点口径 | 站点数 |
|---|---:|
| 库侧直接导出、消费者 UND | 16 |
| 经导出载体交付、消费者载体 UND | 2 |
| 消费者本地定义或完全内联 | 16 |
| 合计 | 34 |

O0/O2 不改变批次。`S23` 与 `S32` 在 O2 下完全内联，但代码仍位于
消费者；其余头侧站点形成消费者本地弱定义，ARM 生产式可见性下为
hidden。库侧直接站点在平台发布库中均为弱导出、无版本节点；C2
对照库中为 `W@@LLVM_22`。

与 x86_64 没有逐站点批次差异。架构层面的符号形态差异（包括
ARM `streamsize` 签名和可见性）见
`tables/architecture_differences.tsv`，均未改变 UND/本地定义结论。

## 3. 板上对照实验

选样为：

- `S05`：`istream:808`，`basic_istream<char>::getline(char*, int, char)`；
- `S08`：`istream:915`，`basic_istream<char>::read(char*, int)`。

两者均是平台发布库中弱导出、消费者 O0/O2 均为 UND，且可用阻塞
`read(2)` 形成明确取消点。测试使用裸 `pthread_create`，线程函数
非 `noexcept`，应用代码无 catch，板上原生运行且未使用模拟器。

有效矩阵原文为 `raw/armv7l_board_matrix_valid.raw.log`，结构化结果为
`tables/runtime_matrix.tsv`。关键库本机与板上 SHA256 均为
`9cb6b0ef0c265039d8dc25d5928795c593086e6139eab4b687d35bc59ed47050`；
每个关键格均同时记录 SHA256 与 `LD_DEBUG=libs`，实际加载路径为
`/var/tmp/r50_20260820/key/libc++.so.1`。

首轮矩阵未形成行为观测：测试 DSO 按生产式 hidden visibility 编译，
但装置入口 `r50_run` 未显式导出，所有 libc++ 格在 `dlsym` 阶段退出
66。该无效原文保留为 `raw/armv7l_board_matrix.raw.log`。随后仅给测试
入口增加 default visibility，并在 `readelf --dyn-syms` 预检通过后
重跑；被测标准库路径和判据未改。

SDB 首次 preflight 报 target not found；在不超过三次的约束内重新
连接后成功。部署与最终清理均完成；`/var/tmp/r50_20260820`、共享
tar 和相关进程均为零，登记例外 `/home/owner/r44b_push_probe` 未触碰。

## 4. 交付与边界

- `tables/armv7l_site_classification_34.tsv`：逐站点二维分类；
- `tables/classification_counts.tsv`：x86、ARM 与严格合并计数；
- `tables/architecture_differences.tsv`：架构差异；
- `tables/runtime_matrix.tsv`：板上镜像矩阵；
- `code/`：探针、逐选项构建脚本、部署/运行/清理脚本；
- `commands/`、`raw/`：命令、退出码与未经整理原文。

对照实验抽样 2/18 个 ARM 库侧非汇聚站点；它验证分类方法对这两个
站点的运行时含义，不把抽样结果外推为其余站点的运行时全覆盖。
