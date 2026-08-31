# 依赖、容量与结果判读

## 执行依赖

- 源码：Gerrit `platform/upstream/llvm` sandbox ref，固定 HEAD
  `08a64b6eb95ed74339f1d862a6c22a2f93fa78f9`；不得退回 tizen_base。
- 配置：任务给定两份 project_config，必须先核对本报告记录的 SHA256。
- 工具：GBS 2.0.8（本机实测版本）、Git、RPM/rpmbuild/rpm2cpio、cpio、
  createrepo、patch、coreutils、findutils、ripgrep；sudo/chroot 能力。
- 网络：Gerrit SSH 与两个 Tizen snapshot HTTPS 仓；全量源码服务/内部镜像
  是另一台服务器特有前置，本机材料中 `NOT_AVAILABLE`。
- 构建顺序：sandbox LLVM/Clang → sandbox libc++/libc++abi provider → 把
  provider 放入优先本地仓 → 其余全部包。不能先编消费者再补头文件。

容量没有在本机执行全量构建，精确峰值 `NOT_OBSERVED`。给人工服务器的保守
容量预算是：每个架构至少预留 500 GiB；若三架构根和日志同时保留，预留
1.5 TiB。内存建议 32 GiB，最低 16 GiB；手册将包级并行和包内并行都限制
为 2。该容量是执行预算，不是实测需求；执行方应在每架构开始前记录
`df -h`/`free -h`，低于预算时停下。

## 结果组织

每架构独立目录；每条命令拆成 `.command.txt/.stdout/.stderr/.exitcode`；
GBS success/fail log 原样保留。还需保存：

- sandbox SHA、upstream tag、配置 SHA256；
- provider RPM 文件名、NEVRA、SHA256、`rpm -qpl/-qpR/-qp --provides`；
- GBS index、成功/失败包清单、完整失败日志；
- 全局 diagnostics 索引，但不能用索引代替原日志；
- 与同 snapshot 既有平台构建状态的逐包对照。

## 失败判读优先级

1. 先验证失败 buildroot 实际装的是 `22.1.8-83.1` sandbox clang、libc++、
   libc++abi 与 devel 包。若不是，归为配置/仓优先级问题，不判补丁失败。
2. 失败点在层 A/B/C 修改行、`__forced_unwind` 类型、cxxabi 头、libc++ 头
   实例化或 libc++/libc++abi 链接时，列为本方案相关候选，保留预处理输出
   和完整 link command。
3. 网络、源码下载、磁盘、chroot、签名/仓元数据问题归环境失败。
4. 与相同 snapshot 既有构建同样失败的归既有问题；没有 matched baseline
   时只能 `INDETERMINATE`，不能猜测。

## 已知 45 个源码 RPM 身份

完整名单在 `tables/known45_source_rpms.txt`。它们此前已被登记为需源码适配
的集合，可能表现为：缺失 libstdc++ 内部头、GNU extension namespace/
类型不可用、`std::__cxx11`/ABI namespace 假设、保留 `std` 内部成员、
`cxxabi` API 差异、configure/feature probe 选错，最终在编译或链接阶段
报错。名单含同包不同 Release/架构工具链身份；应按 source RPM 身份匹配，
不要把 45 误称为 45 个唯一 package name。

这些预期并不自动等于“允许忽略”：逐项确认诊断与既有登记位置一致；若失败
落在新位置或指向四补丁，仍需升级为新问题。

## 既有 13 个硬编码 `-lstdc++` 身份

完整位置在 `tables/hardcoded_lstdcxx_13.tsv`。R66 只证明源码树存在引用，
生产构建可达性为 `NOT_OBSERVED`。全量构建中可能出现三种形态：

- 路径未启用或上层参数覆盖，构建通过；
- 链接命令同时含 libc++ 与 `-lstdc++`，可能构建通过但产物出现双运行时
  `DT_NEEDED`，这是必须报告的迁移污染；
- ABI namespace/符号不兼容、链接顺序或 `--as-needed` 导致 undefined
  reference / cannot find library，构建失败。

因此不能预先写成“13 个必失败包”。对构建通过者仍要用 `readelf -d` 检查
是否同时依赖 `libc++.so.1` 和 `libstdc++.so.6`。

## 需要我们处理的结果

- provider 自身编译/打包失败；
- 任何包因新头文件、`__forced_unwind`、libc++abi symbol/version、链接脚本
  或 sandbox package selection 失败；
- 相对 matched baseline 新增的编译/链接失败或告警；
- 产物意外同时依赖两套 C++ runtime；
- 消费者实际使用旧 `/usr/include/c++/v1` 或旧 NEVRA。

纯网络/空间/权限失败不是源码缺陷，但仍须完整保留；已知 45/13 只用于快速
分流，不得替代逐包证据。
