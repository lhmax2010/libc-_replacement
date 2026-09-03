# 两个未定包的证据缺口与临时处理

## gmp

- R100 判定：`UNDETERMINED`，本任务不变。
- 已知：三架构 `gmp-devel` 均提供 `libgmp.a` 和 `libgmpxx.a`；只有
  `libgmpxx.a` 命中 C++，每架构有 11 个可解码全局 C++ 符号，包括 iostream、
  `std::__cxx11::basic_string` 和流运算符。
- 候选 consumer：`eigen`、`python-pycrypto` 有 BuildRequires 关系。
- 缺口：BuildRequires 只说明文件进入 buildroot，未证明 consumer 链接
  `libgmpxx.a`；当前没有真实链接命令、link map 或最终 ELF 中的归档成员归属。
- 闭合方法：取得两包同一构建轮次的完整 verbose link command；若含
  `-lgmpxx` 或归档路径，再用 link map、`nm -uC`/`readelf` 和 archive member
  provenance 确认进入哪个最终 ELF，并对其跨边界 API 分类。

## tensorflow2

- R100 判定：`UNDETERMINED`，本任务不变。
- 已知：三架构均提供带大量全局 C++ 符号的 `libtf_xla_runtime.a` 和
  `libtensorflow2-lite.a`；前者每架构约 4.67 万个、后者约 0.99–1.00 万个。
  同时安装大规模 C++ 头文件。
- 候选 consumer：`inference-engine-tflite`、`nnstreamer`、`nntrainer` 有
  BuildRequires 关系。
- 缺口：尚未证明哪一个 consumer 的最终 ELF 实际吸收了哪些归档成员，也没有
  闭合由此形成的 C++ 类型、异常和所有权边。
- 闭合方法：保存三包 verbose link command 与 link map；检查是否出现
  `libtensorflow2-lite.a` / `libtf_xla_runtime.a`；对最终 ELF 的定义/未定义符号
  和对象归属做交叉核对，再按 R100 的布局敏感/C++ ABI/纯 C 规则分类。

## 定下来之前如何处理

建议在实施计划和验证预算中按“可能需要改”预留，且不允许把二者当成“不需要
改”的安全项；但在真实链接证据闭合前，不直接修改其 spec，也不把它们加入已
确认的 10 包集合。

若最终实际链接成立而暂不迁移，风险是静态归档把 libstdc++ 布局或异常语义复制
进 libc++ consumer，链接可能成功但产生静默数据错误。若最终没有链接却提前迁移，
主要代价是无必要的大包重编、依赖扩散和额外验证，并可能改变独立工具行为。
