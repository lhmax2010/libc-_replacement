# R28：永久 provider 清单的两处核查

## 结论摘要

本轮只复用 R11/R22/R24/R25/R27 已落盘数据，不重扫平台仓、不构建、不安装，也不修改任何既有报告或产物。

1. R27 中指向两个 gcc 源码身份的 8 条源码级永久边，展开为 18 条架构/消费者 ELF 证据。18/18 的 `DT_NEEDED` 均为 `libstdc++.so.6`，provider 二进制包均为 `libstdc++`，provider ELF 均为 `libstdc++.so.6.0.33`。这些边引用的是 gcc 提供的 **libstdc++ 标准库本身**，不是 gcc 的其他 C++ 库。
2. 因厂商包永久保留 libstdc++ 已是既定边界，gcc 的含义是“旧标准库实现继续存在”，不需要为 gcc 再产出一个 libc++ 版本的并行业务 provider 构建。故两个 gcc 源码身份应从“需双运行时构建的永久 provider”清单中剔除；这不表示删除或停止保留 libstdc++。
3. `gcc-14.2.0-1.13.src.rpm` 与 `gcc-14.2.0-1.15.src.rpm` 不是数据重复：同一 Base 快照中 x86_64 二进制映射到 1.13，aarch64/armv7l 映射到 1.15；两版各有 noarch `sanitizer-sources` 记录。按 R24 的精确源码 RPM 身份口径是两个节点，按 package Name 是一个 gcc 家族。
4. 4 个厂商源码身份中，只有 `hal-api-drm` 消费非 gcc 平台 provider；另外三个只连 gcc/libstdc++。
5. 修正后的永久双运行时平台 provider 为 **4 个**：app-core、bundle、gtest、rpc-port。加上 4 个厂商源码身份，修正后的永久集合为 **8 个源码身份**。
6. gtest 仍在修正后的永久清单内，其永久消费者是 `hal-api-drm-1.0.5-1.src.rpm`；按冻结边界，其旧构建须永久保留，而非仅在迁移窗口保留。

## 一、gcc 条目核查

### 1.1 ELF 与二进制包归属

8 条 gcc 源码级边对应 18 条架构/ELF 记录：

- x86_64 使用 `libstdc++-14.2.0-1.13.x86_64`，provider 路径 `/usr/lib64/libstdc++.so.6.0.33`；
- aarch64 使用 `libstdc++-14.2.0-1.15.aarch64`，provider 路径 `/usr/lib64/libstdc++.so.6.0.33`；
- armv7l 使用 `libstdc++-14.2.0-1.15.armv7l`，provider 路径 `/usr/lib/libstdc++.so.6.0.33`。

完整 18 条记录见 `tables/gcc_edge_elf_evidence.tsv`。逐符号、消费者源码包、消费者二进制包、架构和 ELF 的 986 条完整记录见 `tables/gcc_symbols_by_consumer_binary.tsv`；去重后的 176 个符号及其消费者汇总见 `tables/gcc_distinct_symbol_summary.tsv`。

所有记录同时满足：

- `needed_soname = libstdc++.so.6`；
- `provider_binary_package = libstdc++`；
- `provider_elf = .../libstdc++.so.6.0.33`。

因此这些符号来自 libstdc++，没有观察到 libgcc、libcc1、libgfortran 或 gcc 其他 C++ 库作为这 8 条永久边的 provider。

### 1.2 gcc 是否属于双运行时 provider

判断：**应剔除**。

这里剔除的是“需要同时提供 old/libstdc++ 构建与 new/libc++ 构建的永久业务 provider”清单。厂商消费者直接依赖的对象就是旧标准库 `libstdc++.so.6`；它永久保留属于既定厂商兼容边界，不存在为同一个 gcc provider 另做“libc++ 构建”的对应物。因此 R27 的 6 个精确源码身份中，两个 gcc 身份不应计入该清单。

### 1.3 两个 gcc 版本

R11 的 Base binary repodata 实测：

| 精确源码身份 | 实际架构记录 | libstdc++ 运行时 |
|---|---|---|
| `gcc-14.2.0-1.13.src.rpm` | x86_64；另有 noarch | `libstdc++-14.2.0-1.13.x86_64` |
| `gcc-14.2.0-1.15.src.rpm` | aarch64、armv7l；另有 noarch | `libstdc++-14.2.0-1.15.aarch64/armv7l` |

所以这是快照内真实的架构间 release 差异，不是同一架构或同一 NEVRA 被重复计入。若按精确源码 RPM 身份统计为 2；若按 provider package Name 归并为 1 个 gcc 家族。修正清单按语义剔除整个 gcc 家族，因此两个精确身份均移出，剩余平台 provider 数为 4。

## 二、厂商消费者分布

| 厂商源码身份 | 永久边数 | gcc provider | 非 gcc provider |
|---|---:|---|---|
| `drm-service-core-tizen-0.3.9-0.src.rpm` | 2 | gcc 1.13、1.15 | 无 |
| `hal-api-drm-1.0.5-1.src.rpm` | 6 | gcc 1.13、1.15 | app-core、bundle、gtest、rpc-port |
| `hal-backend-drm-generic-1.0.3-0.src.rpm` | 2 | gcc 1.13、1.15 | 无 |
| `pen-wave-1.4.0-2.src.rpm` | 2 | gcc 1.13、1.15 | 无 |

所以，**只有 hal-api-drm 消费非 gcc 平台 provider**；其余三个厂商源码包只连接 gcc 提供的 libstdc++。12 条逐边表见 `tables/permanent_edges_12_consumer_provider.tsv`，逐消费者汇总见 `tables/vendor_consumer_distribution.tsv`。

## 三、修正后的永久集合

修正后的平台 provider 为 4 个：

- `app-core-1.27.13-0.src.rpm`；
- `bundle-0.18.15-0.src.rpm`；
- `gtest-1.15.2-0.src.rpm`；
- `rpc-port-1.21.15-0.src.rpm`。

四者的永久消费者均为 `hal-api-drm-1.0.5-1.src.rpm`。其中 gtest 明确保留在永久清单；该边引用 gtest 类、构造/析构、RTTI 与带 `std::__cxx11::basic_string` 参数的符号，故其旧构建的保留期随永久厂商消费者存在，不是只覆盖迁移窗口。

按源码 RPM 身份计数：4 个平台 provider + 4 个厂商源码身份 = **8 个永久源码身份**。厂商侧仍是 5 个二进制 Name 映射为 4 个源码身份，本轮没有改变该口径。

完整清单见 `tables/corrected_permanent_platform_providers.tsv`，账目见 `tables/corrected_permanent_accounting.tsv`。

## 数据与边界

输入路径、SHA256 与大小见 `inputs/input_identity.tsv`。本任务没有修改 R27 或其他既有文档、没有生成修订补丁、没有做方案建议。
