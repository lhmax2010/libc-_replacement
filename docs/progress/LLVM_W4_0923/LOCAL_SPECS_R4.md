# 本地 spec 清单：R4 人工裁决登记

**QuickBuild 所需的全部本地配方改动均已进入 sandbox。** 此结论按本轮人工确定的Base-Toolchain当前验收配方范围作出，不把所有历史试验配方作为待发布输入。R3原始清单、diff、FAIL及R2全根表/NOT_OBSERVED原样保留。

## 26 项逐项关闭

下表“处置”是人工裁决登记，不重新宣称已对旧配方做构建验证。原diff的真实头为reference（sandbox）在---侧、历史used副本在+++侧：`-`表示sandbox有而旧副本无，`+`表示历史副本有；不将这一符号方向反读为遗漏。

| ID | 包名 | 历史spec路径 | 人工处置 | 结论 |
| --- | --- | --- | --- | --- |
| S003 | bpftrace | tmp/BPF_W1_0921/specs/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |
| S008 | llvm | tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l/local/sources/tizen_unified_standard/llvm-22.1.8-1/llvm.spec | 旧LLVM配方，缺f203923a已有的MLGO libc++资产与build_with_libcxx定义；已被取代。 | CLOSED_BY_HUMAN_DECISION |
| S019 | bpftrace | tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-gcc/local/sources/tizen_base_standard/bpftrace-0.24.2-1/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |
| S020 | bpftrace | tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/sources/tizen_base_standard/bpftrace-0.24.2-1/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |
| S021 | bpftrace | tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-gcc/local/sources/tizen_base_standard/bpftrace-0.24.2-1/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |
| S022 | bpftrace | tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-libcxx/local/sources/tizen_base_standard/bpftrace-0.24.2-1/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |
| S023 | libcxx-runtimes | tmp/GBS-ROOT/LIBCXX-R91-x86_64-retry1/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S026 | llvm | tmp/GBS-ROOT/R104-llvm-armv7l-libcxx/local/sources/tizen_unified_standard/llvm-22.1.8-1/llvm.spec | 旧LLVM配方，缺f203923a已有的MLGO libc++资产与build_with_libcxx定义；已被取代。 | CLOSED_BY_HUMAN_DECISION |
| S029 | boost | tmp/GBS-ROOT/R104-matrix-aarch64-libcxx/local/sources/tizen_unified_standard/boost-1.83.0-0/boost.spec | 按人工裁决为R104旧版本的flags块位置差异；采用R105验收版99cdc598…，该版与sandbox一致。 | CLOSED_BY_HUMAN_DECISION |
| S035 | boost | tmp/GBS-ROOT/R104-matrix-armv7l-libcxx/local/sources/tizen_unified_standard/boost-1.83.0-0/boost.spec | 按人工裁决为R104旧版本的flags块位置差异；采用R105验收版99cdc598…，该版与sandbox一致。 | CLOSED_BY_HUMAN_DECISION |
| S051 | tensorflow2 | tmp/GBS-ROOT/R105-tensorflow2-aarch64-gccactual/local/sources/tizen_unified_standard/tensorflow2-2.18.0-1/tensorflow2.spec | 人工将此组归为含r105_validation_force_gcc的验证专用配方/基线对照；按设计不推送，不作为QuickBuild配方遗漏。 | CLOSED_BY_HUMAN_DECISION |
| S053 | tensorflow2 | tmp/GBS-ROOT/R105-tensorflow2-armv7l-gccactual/local/sources/tizen_unified_standard/tensorflow2-2.18.0-1/tensorflow2.spec | 人工将此组归为含r105_validation_force_gcc的验证专用配方/基线对照；按设计不推送，不作为QuickBuild配方遗漏。 | CLOSED_BY_HUMAN_DECISION |
| S054 | tensorflow2 | tmp/GBS-ROOT/R105-tensorflow2-x86_64-gccactual/local/sources/tizen_unified_standard/tensorflow2-2.18.0-1/tensorflow2.spec | 人工将此组归为含r105_validation_force_gcc的验证专用配方/基线对照；按设计不推送，不作为QuickBuild配方遗漏。 | CLOSED_BY_HUMAN_DECISION |
| S056 | tensorflow2 | tmp/GBS-ROOT/R108-tensorflow2-armv7l-gccbaseline/local/sources/tizen_unified_standard/tensorflow2-2.18.0-1/tensorflow2.spec | 人工将此组归为含r105_validation_force_gcc的验证专用配方/基线对照；按设计不推送，不作为QuickBuild配方遗漏。 | CLOSED_BY_HUMAN_DECISION |
| S069 | pcre | tmp/GBS-ROOT/R29-pcre-x86_64-20260812-02/local/sources/r29/pcre-8.45-1.13/pcre.spec | R29早期实验；采用与sandbox一致的R104验收版。 | CLOSED_BY_HUMAN_DECISION |
| S070 | pcre | tmp/GBS-ROOT/R29-pcre-x86_64-20260812-03/local/sources/r29/pcre-8.45-1.13/pcre.spec | R29早期实验；采用与sandbox一致的R104验收版。 | CLOSED_BY_HUMAN_DECISION |
| S071 | pcre | tmp/GBS-ROOT/R29-pcre-x86_64-20260812-04/local/sources/r29/pcre-8.45-1.13/pcre.spec | R29早期实验；采用与sandbox一致的R104验收版。 | CLOSED_BY_HUMAN_DECISION |
| S097 | libcxx-runtimes | tmp/GBS-ROOT/R94-direct-clang-armv7l/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S098 | libcxx-runtimes | tmp/GBS-ROOT/R94-no-override-armv7l/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S099 | libcxx-runtimes | tmp/GBS-ROOT/R94-with-override-armv7l/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S100 | libcxx-runtimes | tmp/GBS-ROOT/R96-aarch64-gcc-env/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S101 | libcxx-runtimes | tmp/GBS-ROOT/R96-aarch64-gcc-normalized/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S102 | libcxx-runtimes | tmp/GBS-ROOT/R96-aarch64-llvm-final/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S103 | libcxx-runtimes | tmp/GBS-ROOT/R96-armv7l-gcc-env/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S104 | libcxx-runtimes | tmp/GBS-ROOT/R96-armv7l-llvm-env/local/sources/tizen_unified_standard/libcxx-runtimes-22.1.8-1/libcxx-runtimes.spec | R91–R96旧迭代；Patch0..9为GBS导出自动生成，R94 override为当时实验。已由含condvar LLVM_22_TIZEN_1及静态库改动的5c169afc取代。 | CLOSED_BY_HUMAN_DECISION |
| S110 | bpftrace | tmp/WEEKEND_0918/specs/bpftrace.spec | 旧bpftrace配方，早于static-link-libbpf与static构建段；已由7831fb34取代。 | CLOSED_BY_HUMAN_DECISION |

## 27 项无参照记录：不属于 Base-Toolchain 范围

按人工裁决为测试夹具及R29 Unified实验包，仅登记排除；原NOT_AVAILABLE不改写为已核实与原仓一致。

| ID | 包名 | 路径 | 原记录 | R4范围处置 |
| --- | --- | --- | --- | --- |
| S002 | bpftrace-undefined-harness | tmp/BPF_W1_0921/specs/bpftrace-undefined-harness.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S005 | tensorflow2-aot-%{aot_arch} | tmp/BUILD_W1_0905/tensorflow2-aot-source/packaging/tensorflow2-aot.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S010 | tensorflow2-aot-%{aot_arch} | tmp/GBS-ROOT/BUILD-W1-AOT/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/SOURCES/tensorflow2-aot.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S011 | tensorflow2-aot-%{aot_arch} | tmp/GBS-ROOT/BUILD-W1-AOT/local/sources/tizen_unified_standard/tensorflow2-aot-x86_64-2.18.0-1/tensorflow2-aot.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S057 | r29-consumer | tmp/GBS-ROOT/R29-consumer-default-x86_64-20260812-04/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S058 | r29-consumer | tmp/GBS-ROOT/R29-consumer-default-x86_64-20260812-05/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S059 | r29-consumer | tmp/GBS-ROOT/R29-consumer-default-x86_64-20260812-06/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S060 | r29-consumer | tmp/GBS-ROOT/R29-consumer-libcxx-x86_64-20260812-01/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S061 | r29-consumer | tmp/GBS-ROOT/R29-consumer-libcxx-x86_64-20260812-02/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S062 | r29-consumer | tmp/GBS-ROOT/R29-consumer-libcxx-x86_64-20260812-03/local/sources/r29/r29-consumer-1-1/r29-consumer.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S063 | flatbuffers | tmp/GBS-ROOT/R29-flatbuffers-x86_64-20260812-01/local/sources/r29/flatbuffers-24.3.25-1/flatbuffers.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S064 | flatbuffers | tmp/GBS-ROOT/R29-flatbuffers-x86_64-20260812-02/local/sources/r29/flatbuffers-24.3.25-1/flatbuffers.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S065 | flatbuffers | tmp/GBS-ROOT/R29-flatbuffers-x86_64-20260812-03/local/sources/r29/flatbuffers-24.3.25-1/flatbuffers.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S066 | flatbuffers | tmp/GBS-ROOT/R29-flatbuffers-x86_64-20260812-04/local/sources/r29/flatbuffers-24.3.25-1/flatbuffers.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S067 | lottie-player | tmp/GBS-ROOT/R29-lottie-x86_64-20260812-01/local/sources/r29/lottie-player-0.2.5-1/rlottie.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S068 | lottie-player | tmp/GBS-ROOT/R29-lottie-x86_64-20260812-02/local/sources/r29/lottie-player-0.2.5-1/rlottie.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S087 | r30-pthread-cancel-probe | tmp/GBS-ROOT/R30-semantic-default-armv7l-20260812-01/local/sources/r30_semantic_default_armv7l/r30-pthread-cancel-probe-1-1/r30-pthread-cancel-probe.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S088 | r30-pthread-cancel-probe | tmp/GBS-ROOT/R30-semantic-libcxx-armv7l-20260812-01/local/sources/r30_semantic_libcxx_armv7l/r30-pthread-cancel-probe-1-1/r30-pthread-cancel-probe.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S089 | r30-pthread-cancel-probe | tmp/GBS-ROOT/R30-semantic-libcxx-x86_64-20260812-01/local/sources/r30_semantic_libcxx_x86_64/r30-pthread-cancel-probe-1-1/r30-pthread-cancel-probe.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S090 | r30-pthread-cancel-probe | tmp/GBS-ROOT/R30-semantic-revised-libcxx-armv7l-20260812-01/local/sources/r30_semantic_libcxx_armv7l/r30-pthread-cancel-probe-1-1/r30-pthread-cancel-probe.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S091 | r30-pthread-cancel-probe | tmp/GBS-ROOT/R30-semantic-revised-libcxx-x86_64-20260812-01/local/sources/r30_semantic_libcxx_x86_64/r30-pthread-cancel-probe-1-1/r30-pthread-cancel-probe.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S092 | r31-cancel-matrix | tmp/GBS-ROOT/R31-matrix-libcxx-x86_64-20260812-01/local/sources/r31_libcxx_x86_64/r31-cancel-matrix-1-1/r31-cancel-matrix.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S093 | r31-cancel-matrix | tmp/GBS-ROOT/R31-matrix-libstdcxx-x86_64-20260812-01/local/sources/r31_default_x86_64/r31-cancel-matrix-1-1/r31-cancel-matrix.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S094 | r32-forced-unwind-candidates | tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-01/local/sources/r31_libcxx_x86_64/r32-forced-unwind-candidates-1-1/r32-forced-unwind-candidates.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S095 | r32-forced-unwind-candidates | tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/sources/r31_libcxx_x86_64/r32-forced-unwind-candidates-1-1/r32-forced-unwind-candidates.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S096 | r32-forced-unwind-candidates | tmp/GBS-ROOT/R32-candidates-libstdcxx-x86_64-20260813-01/local/sources/r31_default_x86_64/r32-forced-unwind-candidates-1-1/r32-forced-unwind-candidates.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |
| S109 | bpftrace-undefined-harness | tmp/WEEKEND_0918/specs/bpftrace-undefined-harness.spec | NOT_AVAILABLE | OUT_OF_SCOPE_BY_HUMAN_DECISION |

## 本轮两项候选的6个副本

远端5c169afc的树与本地db5b49af候选完全相同，故下列已授权功能改动已进入sandbox；不再标待推送。

| ID | 包名 | 路径 | R4状态 |
| --- | --- | --- | --- |
| S105 | libcxx-runtimes | tmp/NIGHT_0917/finish_validation/rpm/SPECS/libcxx-runtimes.spec | PUSHED_5c169afc |
| S106 | llvm | tmp/STATIC_0917B/rpm-aarch64/SPECS/llvm.spec | PUSHED_5c169afc |
| S107 | libcxx-runtimes | tmp/STATIC_0917B/specs/libcxx-runtimes.spec | PUSHED_5c169afc |
| S108 | llvm | tmp/STATIC_0917B/specs/llvm.spec | PUSHED_5c169afc |
| S111 | libcxx-runtimes | tmp/WEEKEND_0918/specs/libcxx-runtimes.spec | PUSHED_5c169afc |
| S112 | llvm | tmp/WEEKEND_0918/specs/llvm.spec | PUSHED_5c169afc |

## 范围与保留项

其余38项与sandbox一致、15项与原仓一致的R3记录沿用。R3中111条未能定位宿主路径的历史命令记录继续保留NOT_OBSERVED，不声称此次恢复了其路径或重做全根溯源；它们不成为在人工已批准的当前配方范围之外无限追溯的任务。

本次关闭的是当前QuickBuild配方与分支前置核查，不是QuickBuild构建、服务端配置或镜像安装验证通过。
