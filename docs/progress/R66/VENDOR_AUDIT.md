# R66 厂商相关包可迁移性复核

## 结论

此前把四个源码包统一称为“厂商预编译、永久保留旧运行时”并不准确。
冻结的 371 包源码语料实测显示，三个包具有可重新编译的实现源码；只有
`pen-wave-1.4.0-2` 只交付头文件、资源和五架构预编译 DSO，没有实现源码。

因此按“交付物在技术上能否从本源码包重编”的口径，永久旧运行时约束为
**1 个源码包身份**，对应 **2 个二进制包名**（`pen-wave`、
`pen-wave-devel`）。R22 的“5 个二进制包名 / 4 个源码身份”是发布制品
边界口径，不等于源码可重建性口径。

## 逐包证据

完整汇总见 `tables/vendor_rebuildability.tsv`，命令和退出码原件在
`progress/R66/commands/025_...030_*.log`。

- `drm-service-core-tizen-0.3.9-0`：spec 的 `%build` 调用 CMake 和
  `make`；源码树包含完整 `.cpp/.h` 实现，CMake 从这些文件生成共享库和
  可执行文件；源码 payload 内没有 ELF 或归档库。结论 `SOURCE_BUILDABLE`。
- `hal-api-drm-1.0.5-1`：生产实现为 C，TIDL 也显式生成 C；唯一 C++
  源码是 `tests/drm_hal_test.cpp`。spec/CMake 会编译库、service plugin 和
  haltests，payload 没有预制 ELF。结论 `SOURCE_BUILDABLE`。
- `hal-backend-drm-generic-1.0.3-0`：完整 C++ 源码由 CMake 编译成共享
  backend，payload 没有预制 ELF。结论 `SOURCE_BUILDABLE`。
- `pen-wave-1.4.0-2`：没有实现源码；CMake 只按架构选择并复制
  `libhand-drawing-engine.so.1`。五个 DSO 均实测 `DT_NEEDED` 含
  `libstdc++.so.6`，并含 GLIBCXX/CXXABI 引用。结论 `PRECOMPILED_ONLY`。

## hal-api-drm 四依赖复核

“永不迁移”来自 R13/R22 对**当前发布二进制**的厂商边界单列，不是源码
或工具链的技术限制。源码实测进一步区分：

- `bundle`、`rpc-port`、app-framework 在生产源中通过 C API 使用；
- gtest/gmock 只服务 C++ haltests；
- R22 的已发布 haltests ELF 确有 gtest 的 19 个 C++ ABI 引用；
- R22 将 app-core/bundle/rpc-port 某些边记为 C++ 耦合，是 provider DSO
  自身泄露出的 `std::__cxx11::basic_string` 动态符号相交；这不把调用处
  的 C 源码改写成 C++ API。

所以，只因 `hal-api-drm` 当前被厂商边界单列而对四个平台包施加的“永久
旧运行时”约束可以解除；但“源码可重编”不等于“已经完成 libc++ 重编
验证”。四依赖的具体迁移顺序、provider DSO 是否清除泄漏符号，以及
haltests 的 gtest 运行时一致性仍需真实构建验证，本任务没有实施构建，
均不推断为通过。

## 口径与盲区

- “1/2”只回答源码包是否包含可重编实现，不回答许可证、厂商签名、发布
  权限或组织流程是否允许重发；这些输入 `NOT_AVAILABLE`。
- 对三个源码可构建包没有在本任务实际构建，因此构建成功性和产物等价性
  `NOT_OBSERVED`。
- `pen-wave` 若另有未提供的私有源码，本语料无法观测；就本次提供的 source
  RPM 而言是确定的 `PRECOMPILED_ONLY`。
