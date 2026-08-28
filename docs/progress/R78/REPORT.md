# R78：新旧标准库共存场景验证

## 结论

`PARTIAL`

两架构的可控同进程形态均完成：正常 C ABI 路径通过；线程取消从
libstdc++ 插件的 `condition_variable::wait` 发起，强制展开跨过旧插件和
libc++ 主程序后，两侧析构各执行一次且 `pthread_join` 返回
`PTHREAD_CANCELED`；普通 C++ 异常跨界则在两架构均无法按共享的
`BoundaryError` 类型捕获，只落入 `catch(...)` 并退出 12。

至少一个真实安装组件的场景已实际通过：armv7l 板上的
`/usr/lib/libicuuc.so.78.1` 直接依赖 `libstdc++.so.6`，补丁 libc++ 主程序经
其 C API `u_getVersion` 得到 78.1.0.0。`LD_DEBUG` 和进程加载枚举同时证明
进程中存在任务版 `libc++.so.1`、`libc++abi.so.1`、系统
`libstdc++.so.6` 和真实 ICU DSO，退出 0、行为断言 PASS。

仍记 `PARTIAL` 而非 `COEXISTENCE_VERIFIED`，因为最重要的厂商个案
`pen-wave` 在本板镜像缺少其 `DT_NEEDED libGLESv2.so.2`，实际 C API 未能
进入；不能用 ICU 或受控插件的成功替代 `pen-wave` 自身结论。

实际运行时间：2026-08-28T21:03:24+08:00 至
2026-08-28T21:20:28+08:00，约 17 分钟；设计阶段约 20 分钟以内，未超过
80 分钟硬上限。资源门禁 `medium` 退出 0，可用内存约 21 GiB。

## 场景设计与真实性

完整表见 `tables/scenario_design.tsv`。优先顺序如下：

1. 真实已安装的旧运行时 DSO，由迁移后的 libc++ 主程序经 C ABI 调用；
2. 受控插件中让取消点位于旧侧、调用栈跨越两套运行时；
3. 故意跨界传递普通 C++ 异常，用来检验此前“禁止跨界 C++ 语义”的边界；
4. 分进程共存只记录设计，不占用本次同进程验证时间。

真实性依据：

- `pen-wave` 源包只带预编译库；armv7l 库 SHA256
  `0dd24e53a6f87f64c4db3fc1828f8564f6a3e0f28a31c77caacc8112f0bf3843`
  与板上实际安装文件一致，直接需要 `libstdc++.so.6`，且头文件公开大量
  C linkage API。
- 板上 ICU 78.1 是现存产品 DSO；拉回文件 SHA256
  `f9690941908929db307395cb905a8b957e95b9285434eb72ae2147b978cb3e93`，
  ELF 直接需要 `libstdc++.so.6`，并导出无 C++ 类型的 `u_getVersion`。
- R11 包统计显示 armv7l 运行时 ELF 样本中存在 121 个同时含 C++ 与可操作
  纯 C ELF 的包，说明 C/C++ 边界形态并非纯理论构造；这只是包级形态依据，
  不等于 121 包均经本轮运行。

## 用例结构与判据

### 正常路径

`new_main` 只直接依赖补丁 libc++/libc++abi；`libold_plugin.so` 只直接依赖
libstdc++。主程序用 `dlopen` 加载旧插件，通过 C 参数缓冲区调用插件内部
`std::string` 操作，断言结果为 `old:ping` 且旧侧 guard 析构一次。

真实 ICU 探针采用相同边界但不使用自制插件：通过 `dlsym` 调用系统 ICU 的
真实 C API，断言版本，并枚举加载对象。

### 跨界线程取消

调用链是：补丁 libc++ 主程序中的裸 `pthread_create` 线程入口 →
libstdc++ 插件 `old_cancel_wait` → libstdc++ `condition_variable::wait` →
`pthread_cond_wait` 取消点。线程入口和调用链用户函数均未声明 `noexcept`。
主线程只在旧插件报告已进入 wait 后调用 `pthread_cancel`。

行为判据不是单看退出码，而是同时要求：`pthread_cancel == 0`、
`pthread_join == 0`、join 值为 `PTHREAD_CANCELED`、新旧两侧 guard 析构计数
都等于 1。armv7l 和 x86_64 全部满足。

### 跨界普通异常

旧插件抛出共享头定义的 `BoundaryError{77}`，新侧先尝试同类型 catch，再设
catch-all。两架构都进入 catch-all 而非 typed catch，退出 12。该结果不是
产品补丁回归，而是刻意触犯既有禁止边界后的观察。

## 验证矩阵

逐格结果及证据路径见 `tables/scenario_matrix.tsv`。摘要：

| 架构 | 真实 ICU C API | 正常 C ABI | 跨界取消 | 跨界普通异常 | `pen-wave` |
|---|---|---|---|---|---|
| armv7l | PASS / 0 | PASS / 0 | PASS / 0 | typed catch 失败 / 12 | 缺 GLES / 3 |
| x86_64 | `NOT_OBSERVED` | PASS / 0 | PASS / 0 | typed catch 失败 / 12 | `NOT_AVAILABLE` |

所有运行保存完整 `LD_DEBUG=libs` 输出；加载枚举确认每个成功的受控格同时有
libc++、libc++abi 和 libstdc++。armv7l 使用 R68/R69 最终补丁运行库；
x86_64 使用同一最终补丁构建产物。构建命令原文、ELF `DT_NEEDED`、SHA256、
stdout/stderr 均在 `commands/` 和 `raw/`。

## 与既有结论的关系

观察支持并细化此前结论：

- “两套运行时可在同一进程共存”得到实际支持，包括真实 ICU C API 和两架构
  受控正常路径；
- “禁止跨界传递 C++ 语义”得到普通异常反例支持：相同源码类型并未在另一
  运行时被 typed catch；
- POSIX 强制展开是本方案专门兼容的机制，不应与普通 C++ 异常合并表述。
  本轮只观察到受控取消栈在两架构正确完成，不能据此允许任意 C++ 对象、
  allocator、RTTI 或异常跨界。

因此建议性结论不变（本任务只做验证而未修改产品）：共存边界必须保持 C ABI
或其他明确无 C++ 所有权/异常语义的协议；本报告没有把测试代码写入产品。

## `pen-wave` 专门确认

- 板上实际文件存在，且 hash 与源包预编译 armv7l DSO 一致；
- 补丁 libc++ 探针实际执行 `dlopen`，动态加载器因缺少
  `libGLESv2.so.2` 返回失败；远端退出 3；
- 因 DSO 未加载完成，`SetResourcePath` / `GetResourcePath` 未被调用，结果为
  `NOT_OBSERVED`，没有补造 GLES stub，也没有改产品或系统文件；
- 该失败只说明当前开发板镜像的依赖闭包不足，不能判定具备完整图形栈的产品
  镜像会成功或失败。x86_64 供给包没有 x86_64 预编译库，记
  `NOT_AVAILABLE`。

## 技术性错误与处置

- 首次部署的远端 chmod 子句因本地/远端变量转义错误指向 `/new_main`，但
  `sdb push` 实际文件均已落到预期任务目录。只读定位确认根目录无误放文件，
  随后用硬编码完整路径补做 chmod/symlink；未删除未知文件。
- 首次 `pen-wave` 运行暴露 `sdb shell` 不传递远端程序退出码；新增包装器在
  远端输出 `__R78_REMOTE_EXIT_CODE` 并由主机解析，已对相关格重新执行。
- x86_64 首次运行脚本错误地把构建 sysroot 的 glibc 加入宿主
  `LD_LIBRARY_PATH`，进 main 前退出 139。删除该注入后从 x86 三格重跑，
  结果稳定；原失败完整保留。
- 两次板上 DSO 扫描得到 0 的原因是板上没有 `readelf`，不是没有旧运行时
  DSO；能力检查和更正后的 ICU `ldd` / 拉回 ELF 分析均保留。

以上均只改测试脚本或诊断流程，没有修改四个补丁、平台源码、系统库或
`codes/`。

## 未覆盖范围、自行判断与疑问

- 未覆盖：具备 GLES 完整依赖闭包的 `pen-wave` 实机调用；aarch64；其他真实
  厂商 DSO；跨界 C++ 对象所有权、allocator、RTTI 和回调并发组合。
- 自行判断：以板上 ICU 作为补充的真实旧运行时组件，因为它可由 ELF 和真实
  C API 完整验证，而不能用缺依赖的 `pen-wave` 冒充成功。
- 自行判断：第二架构沿用既有全量验证的 x86_64，与 armv7l 组成两架构矩阵。
- 自行判断：结论选 `PARTIAL`，尽管核心可控矩阵和真实 ICU 已通过，因为
  `pen-wave` 是任务背景中的长期共存关键个案且本轮未进入其 API。
- 尚存需人工裁决疑问：无。后续若要验证 `pen-wave`，需要一块合法安装
  `libGLESv2.so.2` 及其图形依赖的目标镜像；本轮不自行改变板卡环境。

## 清理与材料

最终只删除精确目录
`/opt/usr/home/owner/share/tmp/r78_coexist_20260829`，确认目录不存在且相关
进程为零。未删除板上预存的 `/usr/lib/libpen-wave.so.1.4.0` 或 ICU。

材料目录：`code/`（完整源码和脚本）、`build/`（两架构产物）、
`artifacts/board/`（真实 ICU DSO 证据）、`tables/`、`commands/`、`raw/`。
未向 Gerrit 或外部源码仓推送。
