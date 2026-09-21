# 本轮执行口径与待确认项

- 对账材料已原样复制到 docs/progress/BASE_LEDGER_0921 并推送；远端提交 c7aa2c5e72f78516e40b7a5423a4b83673608bad。复制前后 SHA256 逐文件一致，未改原文。
- 六格指两架构 × 三路径；现有 spec 每格产 main/common/static 三份 RPM，完整时应有 18 份 RPM，而非把每格误称一份 RPM。
- 使用前轮已有候选 spec 与未定义工具链测试入口，逐字节复制、不作新编辑。私有任务目录变化及 w5T1.xzdio → w5.xzdio 记录于 argv-differences.json。
- 旧 ARM 88% 是 main 编译进度，static 尚未开始。保留旧断点；本轮使用新 LLVM 输入与独立 topdir 做完整 -bb，不把旧对象当作新输入验证结果。
- 新 22 个 ARM LLVM RPM 和既有 12 个 aarch64 RPM 全部复制校验；安装其中 8 个构建所需 LLVM 包。GCC/未定义路径保留既有 libstdc++ LLVM 环境，不混装 libc++ 依赖。
- aarch64 前轮 static 仍依赖 libbpf.so.1；本轮不擅自修改 spec 或 CMake 来修复。若再次观测，将单列未闭合，不以写包成功代替静态候选验收。
- ARM 安装事务的 --test 返回 19，211 条冲突全部为同 NEVRA 的 llvm-static-devel。是否允许在本轮私有副本中使用 --replacefiles 替换这些已核实的同包文件，已询问人工；在答复前不绕过冲突。原环境未修改。
- RPM strip 后符号名可能消失。将分别报告 RPM 内 ELF 的字符串实测、同轮未 strip ELF 的字符串实测以及 .text/.rodata 对照；不会把“未 strip 有”写成“RPM strings 有”。
- 本轮不做物理板验证、不推包仓、不替换 Source1002。QuickBuild 按人工裁决等待 static 子包问题闭合。
- aarch64 libc++ 本轮已整轮退出 0、写出 3 个 RPM；载荷核验只在额外 `libbpf.so.1` 上未闭合。精确 `_ZNSt3__1` 为 16,480 行，精确 `_ZNSt7__cxx11` 为 0；同轮未 strip ELF 与 RPM ELF 的 .text/.rodata 摘要一致。没有写包失败，不触发失败现场 GDB 分支。
- 新观测：aarch64 原 Source1002 ELF 的精确 `_ZNSt7__cxx11` 为 0，但扩大到 `St7__cxx11`/`std::__cxx11` 的检索命中 257 行（例如类型名中的 `RKNSt7__cxx1112basic_string...`）。这说明窄前缀零命中不能作为没有该 ABI 痕迹的证明；本轮仅登记字符串痕迹，不据此还原原资产的完整链接参数、编译器或来源。原文未作回改。
- 精确 `__cxx11` 前缀检索的阳性检查：现有 aarch64 GCC 环境 `libstdc++.so.6.0.33` 命中 732 行 `_ZNSt7__cxx11`，SHA256 为 `4c35b97f02b53c3ce8d21f586917c1170d841934aeaf67ccb9eae33b5c09b5f9`；命令与结果在 raw/091、092。它确认检索可命中该写法，不证明检索覆盖了所有 ABI 形态。
