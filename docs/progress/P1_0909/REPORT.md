# 两天任务包开工前的四项确认

核查日期：2026-09-09。性质：只读查证、历史实测证据复核；本次没有构建或运行测试，
没有部署、清理、修改平台配置或推送 Gerrit。仅新增本目录交付材料。
资源检查 light 返回 0（PASS），见 [原文](raw/001_resource.stdout)。

## 总览

| 确认项 | 结果 | 对开工前提的影响 |
|---|---|---|
| 双版本 | 设计、真实库本地验证和 sandbox 提交均已完成；另有 ARM 修饰名修正 | 不能继续按“双版本尚未实施”安排；也不能按“旧对象重链即可安全取消”安排 |
| 开发板 | 当前连接成功；昨夜残留仍在，八份诊断 ZIP 已确认归属；其他会话占用不能完全确认 | 昨夜不可达不再是当前状态；清理仍待运行时线 W2，不能按干净板开工 |
| overflow 测试 | 静态判断可以在合理适配范围内用 libstdc++ 编译；原测试断言不全可移植 | 区分编译可行与原测试通过；本次实际编译、运行均为 `NOT_OBSERVED` |
| 四类实施改动 | 相对既定基线共五个提交，全部已在 sandbox 远端 | 有两架构定向实测；官方套件有已知新增失败，ARM 仍缺 3,554 项，不能称全部验证通过 |

## 一、双版本的实施状态

**静态核查**：[原阶段报告](../IMPL_0908/W2/REPORT.md) 与当前源码一致。
当前分支 `sandbox/lhmax2025/libcxx-noexcept-relief` 的本地与远端 HEAD 均为
`f3c1af692b579add991861e1f7c4950f6af39932`，本次只做 `ls-remote`，没有推送该分支。
见 [当前提交和远端查询](raw/002_local_status.stdout) 末尾。

### 设计写法

版本脚本实际位于 `packaging/libcxx-llvm22.map`，不是 libcxx/lib 下：

```text
LLVM_22 {
  global:
    *;
  local:
    __libcpp_tizen_cv_*;
};

LLVM_22_TIZEN_1 {
} LLVM_22;
```

普通 `wait` 和 system-clock `__do_timed_wait` 各有旧、新两个包装函数，共四个。
旧包装保留 `noexcept`，新包装允许展开。它们是 `extern "C"` 自由函数，
显式首参数为 `condition_variable* self`（对应成员函数的 this），其余参数保持原类型。
通过 **公开 `self->native_handle()`** 传递底层句柄到文件内 static 共用等待实现；
**没有私有成员访问写法，也没有对象布局强转**。

`.symver` 的 `@LLVM_22` 指向旧包装，`@@LLVM_22_TIZEN_1` 指向新默认包装；
`remove` 与版本脚本的 `local` 配合，避免 helper 实现名导出。
完整代码及行号见 [源码记录](raw/008_local_sources.stdout)；版本脚本及构建差异见
[配置记录](raw/009_portability_and_changes.stdout)。

`LIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS` 默认 OFF，平台 recipe 设置 ON；
要求 Linux ELF、`__1` ABI 命名空间，定义加在 shared libc++ 目标上。
不是任意命名空间或静态库的通用双版本方案。
steady/custom 两个头文件内联等待入口仍须重编调用方，不能依赖更新动态库修正旧内联代码。

初始双版本提交 `72e7a0f2` 使用的定时别名在 ARM 不正确；后续 `f3c1af69`
按 `intmax_t` 的实际类型选择修饰名（LP64 为 long 的 `l`，本次 ARM 为 long long 的 `x`），
并加类型静态断言。修正后两架构的旧成员名与别名逐字核对通过，见
[历史 ELF 核验](../IMPL_0908/W3/raw/035_alias_verification.stdout)。该缺陷已修正，不是当前阻塞。

### 四种绑定情形的实测结果

以下是**历史实测复核，不是 P1 新跑**。原阶段 x86_64 每格五次；最终阶段在
x86_64 原生、armv7l 物理板分别重跑，每架构每格五次，观测一致。
各格运行时都加载指定新完整运行库，并有实际 maps 和导入版本核验。

| 调用方形成方式 | 实际导入版本 | 已观测行为（每架构 5/5） |
|---|---|---|
| 已链接的旧 ELF | LLVM_22 | 进入 terminate，探针终止处理器退出 86 |
| 旧 quiet.o 仅重链接新库 | LLVM_22_TIZEN_1 | 线程回收为 canceled，但 frame_cleanup=0、target_unlock=0，竞争者不能获得锁 |
| 新头文件对象链接新库 | LLVM_22_TIZEN_1 | canceled，frame_cleanup=1、target_unlock=1，竞争者获得锁 |
| 新头文件对象先链接旧单版本库，再换新运行库 | LLVM_22 | 仍进入 terminate，退出 86 |

86 是测试处理器的标记，不能写成默认 SIGABRT。旧 quiet.o 是具体代码布局的结果，
不能推断所有旧对象都滞留，也不能把 canceled 当作清理成功。
证据：[原绑定表](../IMPL_0908/W2/bindings_native.tsv)、
[最终原生矩阵](../IMPL_0908/W3/matrix_x86_64_final.tsv)、
[最终实板矩阵](../IMPL_0908/W3/matrix_armv7l_final.tsv)、
[本次只读计数核验](raw/011_evidence_audit.stdout)。

另有 system-clock 旧／新版本 × 两架构 × 五次的独立补测，见
[定时版本表](../IMPL_0908/W3/timed_versions.tsv)。双版本本身不是未完成项；
未验证的产品打包部署、aarch64、全部旧对象布局等边界仍保留。

## 二、开发板当前状态与残留

**本次状态实查**：`sdb connect 192.168.108.26:26101` 返回 connected，随后远端命令
实际返回身份、进程和目录信息，不是只凭主机退出码认定成功。
设备为 Raspberry Pi 4 Model B Rev 1.5、armv7l、Linux 6.12.80，查询身份 root。
见 [连接原文](raw/004_board_connect.stdout)、[身份与进程](raw/006_board_identity.stdout)。
查询时间以各 raw 的 `.time.json` 主机 UTC 时间为准；板端 date 单独保留，不假定两端时钟完全同步。

### 按昨夜清单逐项检查

以 [昨夜残留清单](../IMPL_0908/W3/BOARD_RESIDUALS.md) 为检查基准；
本次结果见 [逐项 stat、目录与 ZIP cwd 原文](raw/007_board_residuals.stdout)。

| 精确路径 | 当前实查 |
|---|---|
| `/var/tmp/impl_0908` | 存在；含 old、new、probes、SHA256SUMS |
| `/opt/usr/impl_0908_lit` | 存在；含 baseline、runtime、fixtures、runs |
| `/opt/usr/home/owner/share/tmp/impl_0908_payload.tar` | 存在，15,544,320 字节 |
| `/opt/usr/home/owner/share/tmp/impl_0908_old_timed` | 存在，145,836 字节 |
| `/opt/usr/home/owner/share/tmp/impl_0908_run-7610e781ded34a4487806c1f82e90d08.tar` | 存在，30,720 字节 |
| `/opt/usr/home/owner/share/tmp/impl_0908_run-827fe9828f724efa880fdae3fdc519ac.tar` | 存在，61,440 字节 |
| `/opt/usr/home/owner/share/tmp/impl_0908_run-966adc7e416d4154a9a536a4878eb104.tar` | stat 返回 1，明确 No such file or directory；当前不存在，不能反推历史上从未存在 |

`runs/` 当前保留两个工作目录，分别为 `run-7610e781ded34a4487806c1f82e90d08`
和 `run-827fe9828f724efa880fdae3fdc519ac`。本次没有重新校验运行库摘要，内容完整性
复验为 `NOT_OBSERVED`；存在性不等于可直接复用。缺文件检查以同目录四个存在文件作正向对照。

### 崩溃诊断包

在 `/opt/usr/share/crash/dump/` 的 `t.tmp.exe_*_20260908*.zip`、
`t.tmp.exe_*_20260909*.zip` 范围内查到八份；逐份只读包内 cwd，均指向
`/opt/usr/impl_0908_lit/runs/`。没有读取环境变量、完整转储，也没有解包落地。

| ZIP 文件名 | 字节数 | 包内 cwd 的 run 后缀 |
|---|---:|---|
| t.tmp.exe_12031_20260908112709.zip | 138564 | 4005472b528249a59c72989e72074b63 |
| t.tmp.exe_16996_20260908131255.zip | 136294 | c5b2ed4b465045ae84201ce5bc79aeaf |
| t.tmp.exe_26713_20260908133235.zip | 134800 | 7ec712e683d74c229ff9993ca6c1e6bb |
| t.tmp.exe_26998_20260908133247.zip | 135838 | ddca6ae69f7741e4b3ba000940d70252 |
| t.tmp.exe_27220_20260908133255.zip | 134965 | 93c30fdae78c467aaf6e26ae73e29812 |
| t.tmp.exe_28667_20260908114329.zip | 140588 | e1eb1fb27be54cc9a18c24cc4c063d6c |
| t.tmp.exe_28747_20260908114332.zip | 141611 | 16db9e5460964f1cbc8ea73f1a33124b |
| t.tmp.exe_31433_20260908133536.zip | 139167 | 6810c6edfeff4f7c95801b5d419ffca4 |

昨夜已确认的 16996 包作为归属方法正向对照；另外三份原候选现也确认，另补查四份。
这是**上述筛选范围的八份**，不是全盘所有诊断或所有残留的完备统计。
均原地保留；本次没有下载备份或删除。

### 其他会话和进程

两次 `ps` 快照及可读 `/proc/<pid>/exe`、`cwd` 检查见
[进程路径原文](raw/010_board_process_links.stdout)。
当前有长期登录 shell：PID 686（owner，cwd `/opt/usr/home/owner`）和
PID 1642（root，cwd `/root`），另有 PID 741（graphics 用户的长期 shell，带 sleep 子进程）。
它们不是本次查询新启动的 shell；其操作者、是否属于其他编译会话或未来预约为 `NOT_OBSERVED`。

快照可读进程的 exe/cwd 没有命中昨夜两处任务目录；相同解析方法能识别 `/usr/bin/bash`
作正向对照。本次查询 shell 自身也在原始列表中。未观测到可识别的昨夜测试或 MLGO
验证进程，但**不把这个瞬时快照写成“没有其他会话”或“开发板已独占空闲”**。
未读取 shell 历史或凭据，没有停止任何进程。

## 三、overflow.pass.cpp 的 libstdc++ 编译可行性

**静态判断：可以在合理适配范围内编译；实际编译与运行均 `NOT_OBSERVED`。**
核查对象是实施树中 `libcxx/test/std/localization/locales/locale.convenience/conversions/`
`conversions.buffer/overflow.pass.cpp`，全文及行号见 [源码记录](raw/005_design_and_test.stdout)，
SHA256 见 [输入摘要](raw/011_evidence_audit.stdout)。

该文件直接包含标准头 `<locale>`、`<cassert>`、`<codecvt>`、`<fstream>`、`<sstream>`，
唯一测试辅助头是 `test_macros.h`，没有其他直接测试辅助头。
后者用于 `TEST_HAS_NO_FILESYSTEM` 条件；本次读取的辅助头包含标准 `<version>`／
`<ciso646>`，并有 GCC 的识别分支，不是仅能由 libc++ 编译的私有运行库头。
记录见 [辅助头检查](raw/009_portability_and_changes.stdout)。

文件开头 `_LIBCPP_DISABLE_DEPRECATION_WARNINGS`、`_LIBCPP_ENABLE_CXX26_REMOVED_CODECVT`、
`_LIBCPP_ENABLE_CXX26_REMOVED_WSTRING_CONVERT` 是 libc++ 的构建开关，不能作为
libstdc++ 功能保证。可以静态规划选 C++14（设施仍存在且避开 C++17 弃用警告），
保留测试辅助头搜索路径，或在确认目标有文件系统后去掉这个辅助条件，并显式补 `<cstdio>`
声明 `std::remove`。这只是可行性分析，本次没有改测试或执行编译命令。

所用 API 在本机 `/usr/include/c++/13/bits/locale_conv.h` 中存在：
`wbuffer_convert`、构造器、受保护 `overflow` 等；`test_buf` 访问的 `pbase/pptr/epptr/gbump`
属于基础流缓冲接口，不是访问 libc++ 私有字段。

**能编译不等于原样可通过。** 原文件第 17 行已注明不完全可移植：

- 第 54–60 行要求初始写缓冲指针为空、首次 overflow 后容量恰为 4095。
  本次读取的 libstdc++ 构造器在底层缓冲非空时立即 `setp`，内部长度为 32；
  这些实现细节断言不能作为跨库通过标准。
- 第 74–81 行对 `pubsetbuf(0,0)` 后指针状态的要求，同样不能据接口存在就断言相同。
- 第 94–112 行要求三次 sputc 后仅经析构就得到九个 UTF-8 字节。
  libstdc++ 的对应析构为 `= default`（第 442 行），没有转换同步调用。
  因此不能把“能编译”解释为它会满足该输出断言。具体输出数量在 P1 为 `NOT_OBSERVED`。

### 可移植的析构子段设计稿（未编译、未运行）

因为编译可行，“不能编译才设计替代”的条件并未成立。下面仍给出一个便于审阅的
**析构子段**设计，不代替原测试的内部缓冲布局检查；不建立可执行产物。
保留原测试的文件流形态（三个宽字符、作用域析构、读取底层字节），避免把原源码
第 90 行提及的 stringstream 差异混入对照。未来运行时文件须放在专用临时目录。

```cpp
#include <codecvt>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

// 参数：专用临时目录内的输出文件；destructor 或 explicit。
int main(int argc, char** argv) {
  if (argc != 3) return 2;
  const std::string mode = argv[2];
  if (mode != "destructor" && mode != "explicit") return 2;
  using Conv = std::wbuffer_convert<std::codecvt_utf8<wchar_t>>;
  using Traits = std::char_traits<wchar_t>;
  {
    std::ofstream bytes(argv[1], std::ios::binary);
    if (!bytes) return 3;
    {
      Conv converter(bytes.rdbuf());
      for (wchar_t c : {wchar_t(0x4e51), wchar_t(0x4e52), wchar_t(0x4e53)}) {
        auto value = converter.sputc(c);
        if (!Traits::eq_int_type(value, Traits::to_int_type(c))) return 4;
      }
      if (mode == "explicit" && converter.pubsync() != 0) return 5;
    } // 转换器先析构；destructor 模式不显式同步转换器。
    bytes.close(); // 只关闭底层字节流，不同步已销毁的转换器。
    if (!bytes) return 6;
  }
  std::ifstream input(argv[1], std::ios::binary);
  if (!input) return 7;
  const std::string got((std::istreambuf_iterator<char>(input)),
                        std::istreambuf_iterator<char>());
  if (input.bad()) return 8;
  const std::string expected("\xe4\xb9\x91\xe4\xb9\x92\xe4\xb9\x93", 9);
  std::cout << "mode=" << mode << " bytes=" << got.size()
            << " exact_utf8=" << (got == expected) << '\n';
  return got == expected ? 0 : 1;
}
```

三次 `sputc` 对应原第 96–98 行；转换器先于底层流析构对应第 94–99 行；
长度和完整字节串比较对应第 103–112 行。`explicit` 是新增正向对照，不属于原析构测试。
输出不足会返回 1 并打印具体字节数，不把它伪装成运行环境故障或仅以“不崩溃”判定通过。
源码设计没有推断任何库的实测字节数，也不处理是否豁免原测试这一人工裁决。

## 四、四类实施改动的当前状态

以既有 sandbox 基线 `c3f8578a4db871d9d6de96d751f4c2ea7b1638fa` 为起点，
当前新增 **5 个提交**；不是说仓库全部历史仅有五个。完整 SHA 见 [提交记录](raw/002_local_status.stdout)。

| 提交 | 内容 |
|---|---|
| 753ed75f299eb7d693df490a7a36834599199de4 | 解除四种条件变量等待的规格，同步声明、定义与 C++03 镜像；通知接口规格不变 |
| 2b94110f0ef5e036a17e8cfea80d0c69b0330935 | 普通／定时 shared_mutex 写者预占状态的展开回滚 |
| aafbe73ba928785fff19baf964b5a31c33be91b8 | 现代 wbuffer_convert 析构不调用 __close，资源释放及 helper 保留 |
| 72e7a0f21ceeb1a2a4de60fc5137be194c748b90 | 两个库外部等待入口双版本、版本脚本、平台构建开关 |
| f3c1af692b579add991861e1f7c4950f6af39932 | 修正 ARM 定时入口的 intmax_t 符号修饰名 |

五个提交均在本次查询的远端 HEAD 历史内。本次没有新增实施提交。

| 改动 | 实施与已验证情况 | 保留的未验证／限制 |
|---|---|---|
| 解除规格 | 已实现；普通、system、steady、自定义 Clock 等待在两架构均有取消回收、沿途清理断言；正常通知与超时检查 | 任意 Clock／调用链不在有限样本证明内；内联路径必须重编；普通异常无人捕获不等于不会终止 |
| 写者回滚 | 已实现；普通库内、定时头内守卫在展开时清写者位并通知；两架构检查读者计数保留、排队进展、后续重用与正常路径值 | 定时路径需要重编；任意模板参数、用户回调、所有调度交错不能静态证明；不扩充为全部回滚场景通过 |
| 析构不同步 | 已实现现代头；两架构显式同步保留全部六字节，不显式同步旧组六字节、新组一字节；析构取消对照已做 | C++03 镜像的 wbuffer 析构不在本提交改动中；旧内联对象不会随库更新修正；官方 overflow 新增失败尚在 |
| 双版本 | 已实现并含 ARM 修正；四类绑定、system-clock 旧／新版本及符号名均已两架构验证 | 不覆盖 aarch64、所有旧对象布局、静态链接、其他 ABI 命名空间；旧对象重链接不能保证清理 |

两架构定向最终矩阵各 225 次，另有定时版本 20 次，总 **470 次**具体断言符合预期。
这个“符合预期”包含旧 ELF 终止、旧 quiet.o 锁滞留等负向对照，不是 470 次都无问题。
详见 [历史两架构报告](../IMPL_0908/W3/REPORT.md)。

官方套件则不是全通过：x86_64 完成 11,402 项，libc++ 为 10,009 PASS、129 FAIL、
1,156 UNSUPPORTED、27 XFAIL；libc++abi 为 61 PASS、20 UNSUPPORTED。
与历史逐项对照新增失败是 overflow。armv7l 从两次执行合并取得 7,848 项有效 libc++ 结果，
剩余 **3,554 项**（39 项只有基础设施失败、3,515 项尚未执行，含 libc++abi 全部 81 项）。
当前 ARM 有效范围内新增失败同为 overflow；不能外推缺口。
aarch64、产品 GBS/RPM 打包部署均 `NOT_OBSERVED`。本次连通不补齐任何测试分母。

## 五、对两天包的影响、建议与待审阅点

已成立的前提：实施分支和双版本可作为已提交输入；两架构已有定向证据；
开发板当前可读访问；ARM 的 3,554 项缺口有逐项清单；libstdc++ 的源码可行性不是硬性障碍。

需要保留或调整的前提：板端尚未清理，诊断包由原四份候选补为八份已确认；
不能认定开发板已被独占；双版本并不能补回旧对象清理；overflow 存在缓冲实现差异和
析构行为差异，不能将 libc++ 内部布局断言直接当跨库裁决标准；当前不是官方全套通过。
两天包的完整任务书未在本次另行核对，不能替未知任务逐项批准开工。

**用板顺序建议（由人工裁决）**：确认现有会话释放使用权后，先安排编译线 W1 的单个
MLGO 验证，再移交运行时线 W2 清理本批残留并连续运行 3,554 项补测。
依据是用户给出的前者短、后者约六小时：短任务先完成可避免等待长任务窗口，随后给长任务
连续时段，减少交叉占用。六小时为任务书估时，非本次实测。若 MLGO 本身也要求先清理
这些隔离残留，则具体交接点需两线负责人确认；P1 没有清理、排程或授权占用开发板。

本次自行采用的解释与判断全部列明：

1. “不修改任何文件”按不修改既有／平台文件执行；为满足末尾交付要求，仅新增
   `docs/progress/P1_0909/`，采用最终明确的 docs 路径，不另建顶层 progress 副本。
2. 以已有实施基线计新增提交；只查询 sandbox 远端，不更新其他线分支。
3. 编译可行性为静态判断，附加最小源码仅为报告内设计稿；没有跑编译器或探针。
4. 诊断包按限定文件名和日期范围检查，以包内 cwd 归属，不以时间或同名独自认领。
5. 无任务目录进程是瞬时、可读路径范围的观察，不据此断言其他会话无占用。
6. 用板顺序属于用户明确要求的建议，不是实施决定。待人工确认顺序、使用权及清理交接；
   本次没有因上述未知点自行推进板上测试。

## 记录纪律与交付

正式核查命令、原始 stdout/stderr、退出码、UTC 起止时间在 [raw/](raw/)。
`005` 首次误查版本脚本路径返回 2，错误原文保留；随后 `008/009` 定位并读取正确路径，
没有把检索路径错误写成脚本缺失。板端逐项 stat 返回码嵌在原始输出，不用最后的主机 0
覆盖其中的缺文件结果。预备工作区发现和部分只读翻阅仅有会话记录；没有补造为落盘原始命令。
文件生成使用 apply_patch，其原文保存在会话工具记录；原始查询结果未加工覆盖。
首次全暂存区空白检查返回 2，指出 sdb 原文行尾空格、带行号源码空行等原始记录格式；
为保留原文不修剪证据，另对报告和核验脚本单独进行空白检查，结果见 delivery 回执。

本次只读核验脚本 [audit.py](audit.py) 已落盘，SHA256 及所用记录器、资源闸门、测试源码、
辅助头、libstdc++ 头文件摘要在 [核验输出](raw/011_evidence_audit.stdout)。
既有 R115 两个脏文件不属于本任务，保持原样；不暂存其他任务未跟踪材料。
提交、推送及远端核对命令的落盘回执放在 `raw/delivery/`；最后一次推送的回执产生于提交后，
本地保留，不递归追加“收据的收据”。远端最终 SHA 另在交接回复明确给出。

完成后停止，交人工审阅。
