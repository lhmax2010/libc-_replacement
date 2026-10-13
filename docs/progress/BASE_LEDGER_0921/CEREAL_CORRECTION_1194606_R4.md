# cereal R4 验收与推送补记

判定口径继续为 12 需要改 / 55 无需改 / 6 按设计排除 / 0 未评估。新增的 cereal 现已验收推送，因此需改集合的 12 个包均有已推分支；这不等于本次 QuickBuild 的 ICU/qemu-accel 后续修复也已验收推送。

cereal 精确 manifest 源码 28aedc95 上保留 d62ccf97 的 CLANG_USE_LIBCPP 修法，追加 a681c106a5866b6113b90a25307bf3abb3e76334：performance.cpp 两处 char 分布改为 int 分布后转 char。四种宏组合检查通过；固定 RBS 20261012.081855 输入的 x86_64 构建退出 0，performance 链接完成，原 basic_binary_oprimitive / basic_binary_iprimitive 未定义引用已实测消失。performance 直接依赖 libc++.so.1、不依赖 libstdc++.so.6；cereal-devel 与前一发布版完整文件清单相同，90 个头文件逐字节一致。

远端 sandbox/lhmax2025/libcxx-migration 已核对为 a681c106a5866b6113b90a25307bf3abb3e76334。证据：../QB_1194606_FIX/verify-cereal_R4/、raw/R4_012_cereal_push.*、raw/R4_013_cereal_remote_after.*。此前 R3 失败记录与原历史台账保持不动。
