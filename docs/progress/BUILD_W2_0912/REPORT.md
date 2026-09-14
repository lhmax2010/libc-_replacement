# bcc-tools 与 bpftrace 的 libc++ 适配验证报告

## 1. 结论

结论为 **`PARTIAL`**。

`bcc-tools` 的 aarch64 与 x86_64 四格通过；armv7l 的 libc++ LLVM 输入
不可用，重建又被异常缓慢的构建根 I/O 清理阻断，因此两格为
`NOT_OBSERVED`。六格未闭合，候选未提交、未推送。依赖顺序要求
`llvm → bcc-tools → bpftrace`，故没有越过门禁实施 `bpftrace`。

## 2. 前置与断点恢复

- LLVM 远端前置：
  `sandbox/lhmax2025/libcxx-ehabi-backport` =
  `f203923a1508c9344f5fc6b17bd8822f011655c4`；见 `raw/003_*` 与
  `raw/021_*`。
- 该 LLVM 提交的三架构 libc++、MLGO 执行、GCC 与非 libc++ 同环境对照
  十二格已在上一轮闭合，本轮不重跑。
- 首次运行在 aarch64/GCC 构建中被系统崩溃打断，`raw/019_*` 没有自动
  退出码，也没有 release 112.2 RPM。2026-09-14 续跑时先执行资源门禁、
  进程与产物审计；见 `raw/020_*`、`raw/021_*`。
- 初始门禁与恢复门禁均为 `PASS`。

## 3. bcc-tools 改动候选

只修改 `packaging/bcc-tools.spec`，共增加 15 行：

```diff
+%if %{defined _toolchain}
+%if %{toolchain_is clang}
+%define build_with_libcxx 1
+%endif
+%endif
 ...
+%if 0%{?build_with_libcxx}
+BuildRequires:  libc++-devel
+BuildRequires:  libc++abi-devel
+%endif
 ...
+%if 0%{?build_with_libcxx}
+export CXXFLAGS="${CXXFLAGS} -stdlib=libc++"
+export LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed"
+%endif
```

启用条件与既定设计一致：仅 `_toolchain` 已定义且
`%{toolchain_is clang}` 为真时启用；GCC 路径不引入 libc++ 选项；未定义
`_toolchain` 的配置保持现状。

本地仓状态：分支 `sandbox/lhmax2025/libcxx-migration`，HEAD
`dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2`，只有上述未提交 spec 修改。
远端目标分支仍为同一 SHA；见 `raw/050_*`。

## 4. bcc-tools 验证结果

| 架构 | libc++ | GCC | LLVM 边界 | 结果 |
| --- | --- | --- | --- | --- |
| aarch64 | PASS | PASS | 两端 libc++ | 2/2 格闭合 |
| x86_64 | PASS | PASS | 两端 libc++ | 2/2 格闭合 |
| armv7l | NOT_OBSERVED | NOT_OBSERVED | NOT_OBSERVED | 0/2 格闭合 |

### 4.1 aarch64

libc++ 格：

- 完整构建退出 0，release 112.1；
- 编译器为 Clang 22.1.8；
- 3 个非调试 RPM；其中 2 个 ELF 记录 `libc++.so.1`，0 个记录
  `libstdc++.so.6`；
- 未定义动态符号含 `std::__1` 151 处，`std::__cxx11` 0 处；
- 配套 `libLLVM.so.22.1` 依赖 libc++、libc++abi 与 libgcc_s，不依赖
  libstdc++。

证据：`raw/016_*`、`raw/018_*`、`checks/bcc_aarch64_libcxx/`。

GCC 格：

- 完整构建退出 0，release 112.2；
- 实际编译器 GNU 14.2.0；
- 构建日志中 `-stdlib=libc++` 与 `-lc++abi` 为 0；
- 3 个非调试 RPM；其中 2 个 ELF 记录 `libstdc++.so.6`，0 个记录
  `libc++.so.1`；
- `std::__cxx11` 69 处，`std::__1` 0 处。

证据：`raw/029_*`、`raw/030_*`、`checks/bcc_aarch64_gcc/`。

### 4.2 x86_64

libc++ 格：

- 完整构建退出 0，release 112.3；
- 编译器为 Clang 22.1.8；
- 3 个非调试 RPM；2 个 ELF 依赖 libc++、0 个依赖 libstdc++；
- `std::__1` 151 处，`std::__cxx11` 0 处；
- 配套 `libLLVM.so.22.1` 依赖 libc++、libc++abi、libgcc_s，不依赖
  libstdc++。

证据：`raw/033_*`、`raw/034_*`、`checks/bcc_x86_64_libcxx/`。

GCC 格：

- 完整构建退出 0，release 112.4；
- 实际编译器 GNU 14.2.0；
- 禁止的 libc++ 选项为 0；
- 3 个非调试 RPM；2 个 ELF 依赖 libstdc++、0 个依赖 libc++；
- `std::__cxx11` 69 处，`std::__1` 0 处。

证据：`raw/037_*`、`raw/038_*`、`checks/bcc_x86_64_gcc/`。

### 4.3 armv7l 未闭合原因

1. 当前可找到的三份 armv7l `libllvm-22.1.8-109.3` 经独立解包、
   `readelf -d` 与动态符号核查，均依赖 `libstdc++.so.6`，含
   `std::__cxx11`、不含 `std::__1`，不能作为 libc++ 边界输入；见
   `raw/039_*`、`raw/040_*`。
2. 原 armv7l libc++ 候选未单独保留，已被之后的 GCC 基线构建覆盖。
3. 以已确认的 LLVM 候选工作树重建时：
   - 脏工作树导出全树索引异常缓慢，先中止；随后把准确的一个 spec 与
     三份资产做成仅用于本地构建的 detached 快照，未推送；
   - 首次重试遇到中断遗留的零字节 `gbp_index.lock`；确认无进程后只删除
     该锁并重跑；
   - 增量更新旧根时遇到旧/新 RPM 事务中的 tar 文件冲突；
   - 标准 `-C` 清理重建时，删除旧 LLVM 根连续 60 分钟处于不可中断
     I/O，空间仅早期释放约 1 GiB。为防系统再次失稳，中止构建。

以上均发生在 LLVM 输入准备阶段，未形成 armv7l bcc 候选，因此不能把
它们解释成 bcc 编译失败；该架构两格如实记 `NOT_OBSERVED`。

## 5. 构建环境技术性非零

以下非零均在最终判据之前，并已保留原始命令、输出与退出码：

- 本地日志包装器相对路径错误：即时退出 127，按恢复记录补录为
  `raw/024a_*`；修正仅为改用绝对路径。
- GBS spec 预解析器不能展开 `%toolchain_is`，把条件 BuildRequires 当成
  无条件依赖。测试辅助仓只补入对应架构的 libc++/libc++abi runtime 与
  devel RPM；实际 rpmbuild 的 GCC 宏分支仍为假，最终以“命令行禁用选项
  为 0、产品只依赖 libstdc++”闭合，不据辅助仓内容推断。
- 滚动 Base/Unified 仓跨版本造成 tar 冲突；改用 Base-only 仓后 aarch64
  与 x86_64 四格均完成。
- 固定 Base 快照的单个 RPM URL仍可读，但仓级 repodata 返回 404，不能
  用作构建仓；没有把它冒充可复现固定仓。

## 6. bpftrace

由于 `bcc-tools` 六格未闭合，按明确前置规则没有开始 `bpftrace`：

- 本地仍在 `tizen_base`，HEAD
  `30e51cd665360f85b665308dc13ba27df0c5f739`；
- 远端 `sandbox/lhmax2025/libcxx-migration` 仍为同一 SHA；
- 无工作树修改、无构建、无提交、无推送；
- 当前 spec 第 11 行是 `ExclusiveArch: armv7l aarch64`。任务要求的
  x86_64 格与该声明存在冲突；因前置未满足，本轮未自行改变架构范围。

证据：`raw/050_*`。

## 7. 11 包状态与兼容性复核

最新状态为：9 个已完成并推送；`bcc-tools` 部分验证、未推送；
`bpftrace` 未实施。详见 `PACKAGE_STATUS.tsv`。

- 先前 8 包的 spec 结构与三架构 GCC 实测沿用
  `docs/progress/BUILD_W3_0909/COMPATIBILITY_AUDIT.tsv`；该输入 SHA256 已记
  入 `raw/052_*`。
- LLVM 最终候选及 bcc 本地候选都把 libc++ 选项限制在
  `_toolchain` 已定义且 `toolchain_is clang` 的条件内；见 `raw/052_*`。
- bcc 的 aarch64/x86_64 GCC 实测均没有泄漏 libc++ 选项；armv7l 未观测，
  因此不能声称 bcc 三架构兼容性已完成。
- bpftrace 尚无适配改动，不存在可审计的新增选项。

## 8. 自行判断与尚存疑问

本轮自行判断：

1. 把 spec 预解析器误判条件依赖与实际 rpmbuild 宏求值分开；辅助仓只用于
   让解析阶段继续，最终判据仍是实际命令和产物。
2. 在已确认 armv7l LLVM RPM为 libstdc++ 后，不用它代替 libc++ 输入，
   因为这会主动制造任务正在避免的布局敏感混合边界。
3. 构建根清理连续 60 分钟不可中断等待时，为保护系统中止；不把环境阻断
   写成包失败。

尚存疑问/需后续处理：

1. 需要一个可用的 armv7l libc++ LLVM RPM，或在 I/O 正常的构建机上重建，
   才能闭合 bcc 剩余两格。
2. `bpftrace` 当前 `ExclusiveArch` 不含 x86_64；六格门禁是否要调整为其
   实际支持的两架构，或另行授权扩展 x86_64 构建范围，需要人工裁决。
3. 固定 Base 快照缺少仓级 repodata，后续若要求可重复固定仓构建，需要
   可用的归档仓元数据或由 QuickBuild 提供等价输入。

未修改上游 C++ 源码，未推正式分支，未 force push。

## 9. 材料完整性自检

- `MATRIX.tsv` 为 6 条数据、每条 7 列；`PACKAGE_STATUS.tsv` 为 11 条数据、
  每条 5 列。
- 除系统崩溃中断的 `raw/019_build_bcc_aarch64_gcc` 外，所有已经结束的
  记录均有退出码文件；`raw/019_*` 保留了当时的命令与不完整输出，没有
  事后臆造退出码。
- `raw/053_*` 的首次自检会在命令执行期间把自身列为“尚无退出码”；包装器
  返回后已实际写入退出码 0。排除这一自引用时序后，`raw/054_*` 复检退出
  0，且只剩上述 `raw/019_*` 这一处预期的崩溃断点。
- 本次日志包装器 SHA256 为
  `b77cdf5e81f373c9d2bc91c9747f09094d64dca154cad086b4dd9c795996cc1a`；
  产品核查脚本 SHA256 为
  `1b27af429dd06f14a403af209857e2a6bbbb9834bae5de6e907308e04bc5815c`。
