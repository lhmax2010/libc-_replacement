# Boost 剩余符号边实测（本轮）

## 结论边界

本轮结论为 **`PARTIAL`**。在既有 27/90 条已测边基础上，本轮在
**aarch64、QEMU 用户态**新增测到 22 条边，累计为 **49/90**；仍有
**41/90** 未在其对应架构和符号组上实测。本轮没有把 QEMU aarch64
结果外推到物理 aarch64、x86_64、armv7l，也没有把一个代表符号组的
结果外推到同一边上的所有 C++ 符号。

## 选取及理由

沿用上一轮的 90 条边分类，并严格按以下次序选择：

1. 先补 aarch64 上已有同符号组、但该架构尚未观测的布局敏感边：
   Boost.Test 7 条、Boost.Filesystem 4 条、Boost.Iostreams 1 条、
   Boost.Program Options 2 条，共 14 条。
2. 再补“C++ ABI 耦合但布局未证明”：Boost.Test 5 条、Boost.Thread
   3 条，共 8 条。Boost.Thread 在 x86_64 通过只能说明该架构和该符号
   组的实测结果，不能代替 aarch64 实测；Boost.Test 在这一分类中也
   不能因分类名含“未证明”而默认安全。

逐边清单见 `tables/w3_selected_edges.tsv`，更新后的完整 90 条表见
`tables/w3_edges_90_results.tsv`。

## 方法和正向对照

- provider 均来自 R11 Base aarch64 二进制 RPM，实际加载的是平台的
  Boost 1.83.0 DSO，不是模拟 provider。五个 provider 的 SHA256 记录在
  `raw/080_extract_selected_r11_boost_providers.stdout`。
- consumer 以 aarch64 目标编译，使用 R11 的 libc++ 22.1.8 或
  libstdc++ 14.2.0 头文件和运行库。构建产物经 `file` 确认为 aarch64
  ELF；混合格的动态依赖包含 libc++、libc++abi 与 libstdc++。
- 每个混合格执行 5 次；每个对应的 libstdc++ 同库正向对照也执行
  5 次，共 10 格、50 次。
- 每次由进程自身读取 `/proc/self/maps`。所有 25 次混合格均确认
  `libc++.so.1`、`libstdc++.so.6` 与相应 provider 同时加载；所有 25 次
  对照格确认仅有 libstdc++ 和 provider（没有 libc++）。
- 每格断言具体状态或具体值；同时观察析构。Boost.Thread 的
  `once_flag` 是平凡析构类型，故销毁路径标为 `COMPLETED_TRIVIAL`，
  不是把“不适用”伪装成非平凡析构已测。
- Boost.Test 的输入使用平台 Boost 头文件中已核对的两个指针布局，
  并保留用户声明拷贝构造所导致的“非平凡按值参数”调用属性；源定义
  证据在 `raw/086`、`raw/088`。这一步用于让目标编译器生成正确的
  AArch64 参数与隐藏返回值约定，而不是沿用 x86_64 的寄存器假设。

执行环境是 QEMU 用户态而非物理板。它足以观测本轮确定性的对象布局、
返回值、状态转换与析构结果；不据此声明真实内核时序、信号交付或设备
行为已覆盖。

## 实测结果

| provider / 调用形态 | 混合运行库 5 轮 | libstdc++ 对照 5 轮 | 析构观察 |
|---|---|---|---|
| Boost.Test `normalize_test_case_name`，provider 返回 `std::string` | 5/5 返回后读取为长度 56，期望长度 16；具体值断言失败，退出 41 | 5/5 长度 16，十六进制为 `616c7068615f626574615f67616d6d61`（`alpha_beta_gamma`），退出 0 | 混合侧 5/5 consumer 析构完成且 `dlclose` 完成 |
| Boost.Filesystem `status` + `read_symlink` | 5/5 `status_ok=0`、`value_ok=0`；返回路径长度 8 且文本损坏，退出 41 | 5/5 `status_ok=1`，精确返回 `payload.txt`（长度 11），退出 0 | 两格均 5/5 完成 provider 构造对象的 consumer 析构 |
| Boost.Iostreams `mapped_file_source::open` | 5/5 在给出值断言前抛出 `std::length_error` 并终止，退出 134 | 5/5 打开成功、长度 19、内容精确匹配，退出 0 | 混合侧 5/5 析构未观测；对照 5/5 完成 |
| Boost.Program Options `variables_map` | 5/5 初始大小 0、插入后大小 1、值 82 精确通过；随后在作用域退出的析构阶段 SIGSEGV，退出 139 | 5/5 值与析构均通过，退出 0 | 混合侧 5/5 值正确但析构未完成；对照 5/5 完成 |
| Boost.Thread `enter/rollback/commit_once_region` | 5/5 精确状态 `1,1,0`，退出 0 | 5/5 精确状态 `1,1,0`，退出 0 | `once_flag` 为平凡析构，5/5 作用域退出完成 |

完整逐轮结构化结果为 `build/w3/runtime_results.tsv`；逐轮命令与退出码
为 `build/w3/runtime_commands.tsv`；逐轮 stdout/stderr 均在
`build/w3/`。最终矩阵自校验输出
`ASSERTIONS=PASS architecture=aarch64 cases=10 repeats_per_case=5 controls=PASS maps=PASS`。

这些结果同时给出三种不同形态：静默错值但析构完成、值正确但析构
崩溃、以及精确通过。因此不能用“没有崩溃”替代值与所有权正确性判断，
也不能用“值正确”替代销毁路径正确性判断。

## 覆盖变化

| 分类 | 本轮新增 | 累计已测 | 本轮后仍未测 |
|---|---:|---:|---:|
| 布局敏感 | 14 | 33 | 30 |
| C++ ABI 耦合但布局未证明 | 8 | 16 | 11 |
| 合计 | 22 | 49 | 41 |

累计数字中的“已测”表示该边上选定的代表符号组已在该架构实测，仍不
表示该边的全部 C++ 符号均逐个测试。

## 剩余 41 条及风险说明

- 布局敏感 30 条：其中 21 条仍没有被选中的符号组，2 条是既有
  Filesystem 代表组在 armv7l 之外的架构尚缺，7 条是 Boost.Test
  代表组在 armv7l 尚缺。已有多个家族和架构出现错值、异常终止或
  析构失败，但未测条目仍记 `NOT_OBSERVED`，不以家族结果代填。
- C++ ABI 耦合但布局未证明 11 条：3 条仍未选符号组，8 条为已选
  Boost.Test/Boost.Thread 组在 armv7l 尚缺。Boost.Test 本轮出错而
  Boost.Thread 本轮通过，实测再次表明这一分类内部不能统一推定
  “安全”或“必然失败”。
- 逐条剩余清单和原风险字段见 `tables/w3_remaining_edges.tsv`；分类和
  状态计数见 `tables/w3_edge_status_summary.tsv`。

## 失败尝试与测法校准

- 最初复用 x86_64 的显式隐藏返回指针写法；AArch64 ABI 不同，该构建
  被主动停止，不能算运行时结果（`raw/042`、`raw/045`）。
- 外部挂载目录读取头文件发生长时间块等待；已保存进程状态后停止，
  改从同一 R11 RPM 在 `/tmp/r110_w3_build` 展开头文件和运行库，最终
  构建命令仍逐条记录（`raw/093`、`raw/095`、`raw/101`）。
- 第一版最小 `basic_cstring` 只复现两指针数据布局，遗漏其非平凡拷贝
  构造导致的 AArch64 间接传参属性，混合格和对照格均 5/5 `bad_alloc`。
  因正向对照同时失败，该批结果被判定为测法错误并修正；最终对照
  5/5 通过（`raw/107`、`raw/110`）。
- Filesystem 与 Iostreams 首轮因 provider 的运行时依赖未加入搜索路径
  而退出 127；补入同一 R11 的 Boost.Atomic、zlib、libbz2 后重跑，
  最终矩阵通过自校验（`raw/114`、`raw/117`、`raw/121`、`raw/122`）。

## 可复核产物

- `code/w3_build_aarch64.sh`：最终 SHA256
  `0755ff2995c9629eeef555b0b176cf6057bb0596f6db7872ebb4235f26e04bd7`。
- `code/w3_run_aarch64_matrix.sh`：最终 SHA256
  `b8c95a889773ebd4d55f14f703061caebecedf0bf31bd82e90a1a3310c4a1925`。
- `code/w3_boost_test_portable.cpp`：最终 SHA256
  `d96176991dd35fc47485f0fe94dc05c88e1ea02f5a748fdba40f9955c289567b`。
- `code/w3_maps_probe.hpp`：SHA256
  `2c1a37f58a2b3a10d76595f43d23785a251ed65b537e4b97d87347e6a6d29a33`。
- `code/w3_update_edges.py`：SHA256
  `cf1099bdc74b43b0873168b8dcb981634e19302f24e1c0fca99015618723cdcf`。
- 原始命令、stdout、stderr、退出码：`raw/040` 至 `raw/124`；构建与逐轮
  运行的子命令分别在 `build/w3/build_commands.tsv` 和
  `build/w3/runtime_commands.tsv`。

本报告只补充事实，不对迁移方案、优先级或是否接受风险作决定。
