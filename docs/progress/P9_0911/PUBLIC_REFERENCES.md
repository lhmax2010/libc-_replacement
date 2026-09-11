# 公开依据与检索边界 / Public references and search boundaries

检索日：2026-09-11。这里只把原始作者、标准草案或维护者的文字用作设计／政策依据。网页是检索日可见版本；它们不替代本包中固定 revision 的布局源码。浏览器工具没有 shell 退出码；HTTP 检查的命令及退出码另在 `raw/038*`、`raw/041*`、`raw/045*`，结果在 [URL 检查](PUBLIC_URL_CHECKS.json)及 [GNU 重试](GNU_URL_RETRY.json)。

Search date: 2026-09-11. Rationale/policy is attributed only to original source authors, standard wording or maintainers. Web documents are the versions accessible on that date, not substitutes for the frozen layout sources. Browser tools do not have shell exit codes. HTTP-check commands and exit codes are recorded separately.

| 来源 / Source | 本说明使用范围 / Supported claim | 不能推出 / Not established |
|---|---|---|
| [N4861 objects.within.classes §16.4.2.4](https://timsong-cpp.github.io/cppwp/n4861/objects.within.classes) | 这些库条款不指定类表示；所需成员与等价行为实现有自由度 / Latitude for private representation and equivalent behavior | 所有标准库类型无任何布局限制 / No representation constraints anywhere in the library |
| [N4861 derivation](https://timsong-cpp.github.io/cppwp/n4861/derivation)、[member.functions](https://timsong-cpp.github.io/cppwp/n4861/member.functions)、[structure.specifications](https://timsong-cpp.github.io/cppwp/n4861/structure.specifications) | 内部基类、函数声明余地及语义／复杂度约束 / Bases, member-declaration latitude and semantic/complexity obligations | 接口语义可随意改 / Arbitrary behavior changes |
| [GNU ABI policy](https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html) | 自身 ABI 版本和兼容政策 / Its own ABI/versioning policy | 与 libc++ 互通承诺 / libc++ interoperability promise |
| [GNU Dual ABI](https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html) | C++11 字符串改变、旧用户兼容及命名空间 / String transition and compatibility | 精确 32／24 字节布局的初始动机 / Initial rationale for exact size choice |
| [Wakely function-buffer reply, 2016-07-30](https://gcc.gnu.org/pipermail/libstdc++/2016-July/044437.html) | 16→24 缓冲区扩大因 ABI 被拒 / ABI-based rejection of enlargement | 对全部可能改造的永久裁决 / Permanent ruling on every redesign |
| [libc++ ABI guarantees](https://libcxx.llvm.org/ABIGuarantees.html)、[ABI versioning](https://libcxx.llvm.org/DesignDocs/ABIVersioning.html) | 稳定性、可选破坏性布局变化 / Stability and opt-in ABI changes | 所列新宏已用于原探针 / Every currently documented flag was active in the original probe |
| [LLVM D128285](https://reviews.llvm.org/D128285) | AIX 字符串位域后兼容性修复 / AIX bit-field ABI repair | x86_64 原始短字符串容量的原因 / Original x86_64 short capacity rationale |
| [GNU tree source, gcc-14.2.0](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-14.2.0/libstdc++-v3/include/bits/stl_tree.h) | 左右极值缓存与删除重连的源码说明 / Header caching and deletion relinking comments | 作者未写出的全部树布局动机 / Undocumented complete history |
| [GNU deque source, gcc-14.2.0](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-14.2.0/libstdc++-v3/include/bits/stl_deque.h) | 块大小效率说明、迭代器不变式及 ABI 警告 / Block efficiency, invariants, ABI warning | 为什么恰好选 512 / Exact reason for 512 |
| [GNU hash source, gcc-14.2.0](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-14.2.0/libstdc++-v3/include/bits/hashtable.h) | 单桶支撑无分配的移动后状态 / Embedded bucket supporting non-allocating moved-from state | LLVM 未写明的设计动机 / Undocumented LLVM motive |
| [N4861 any.class](https://timsong-cpp.github.io/cppwp/n4861/any.class)、[futures.state](https://timsong-cpp.github.io/cppwp/n4861/futures.state) | 小对象优化限制、共享状态同步和生命周期 / Small-object constraint, shared-state synchronization/lifetime | 特定实现必须使用某种指针数量 / Required implementation pointer count |
| [N4861 deque.modifiers](https://timsong-cpp.github.io/cppwp/n4861/deque.modifiers)、[associative.reqmts](https://timsong-cpp.github.io/cppwp/n4861/associative.reqmts)、[unord.req](https://timsong-cpp.github.io/cppwp/n4861/unord.req) | 两侧仍要遵守的容器行为约束 / Common container obligations | 已穷举验证两套实现 / Exhaustive conformance test results |
| [AMD64 ABI 0.99.6 §3.1.2](https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf)、[Itanium C++ ABI §2](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#data-layout) | 基本类型尺寸、对齐、C++ 对象规则 / Primitive sizes, alignment and C++ object rules | C++ 库类型的固定私有成员清单 / Fixed private STL fields |

GNU 三个官网 HTML 地址在本机 urllib 返回 403，浏览器工具在同日均成功取得正文；并未把 403 写成成功。其余本次脚本检查成功的 URL 为 HTTP 200。GNU 公开源码注释与平台注释的关键文字也作了双端检查，结果记录在 JSON；原始平台源码的文件和行号以报告实际摘录为准。

The three GNU HTML pages returned HTTP 403 through local urllib, while the browser tool successfully retrieved their contents on the same date. A local 403 is not recorded as success. Other successful scripted checks returned HTTP 200. GNU public source comment phrases were checked against the platform copies; exact platform file/line evidence is in the reports.

## 公开动机未找到的检索记录 / Searches without a specific rationale

实际使用的查询包括以下原文；搜索结果只用于定位，结论回到原始来源。

```
site.libcxx.llvm.org ABI guarantees any function layout
site:gcc.gnu.org libstdc++ manual ABI dual ABI string
site:gcc.gnu.org "std::any" "buffer" ABI
site:lists.llvm.org "any" "small" buffer sizeof
site:gcc.gnu.org "std::function" "ABI" buffer size
site:libcxx.llvm.org "future" "promise" design
site:lists.llvm.org libcxx "any" "three pointers"
site:gcc.gnu.org libstdc++ future shared_ptr rationale promise result allocation
site:reviews.llvm.org D128285 string bitfields
site:eel.is/c++draft "representation" "library" unspecified
```

| 范围 / Scope | 找到 / Found | 仍缺 / Missing |
|---|---|---|
| any | 标准小对象要求及两侧实现 / Standard constraint and code | 1／3 指针缓冲容量的初始设计解释 / Original capacity rationale |
| function | GNU 扩容拒绝邮件；LLVM ABI 文档 / GNU rejection, LLVM ABI docs | 两侧最初完整表示的理由 / Initial complete design history |
| deque | GNU 和 LLVM 的局部源码说明 / Local source explanations | 精确块常数与最初成员数量决策 / Exact constants and original field-count decision |
| map、set | 缓存极值、重连删除和最小哨兵契约 / Caches, deletion, sentinel contract | 完整布局历史 / Complete representation history |
| unordered_map、unordered_set | GNU 单桶、缓存及策略注释 / GNU bucket/cache/policy comments | LLVM 组织字段、固定缓存的公开动机 / LLVM field organization and hash-cache rationale |
| string | 双 ABI、容量位编码、AIX 修复 / Dual ABI, encoding, AIX repair | 15／22 短态容量及原布局的历史选择 / Original short capacity/layout choice |
| future、promise | 共享状态要求、两侧状态和设置路径 / Standard state requirements and code | 分拆／合并所有权、结果、状态的公开动机 / Original split/combined ownership/result/state rationale |

这里的缺口统一为 `NOT_AVAILABLE`。未做上游全历史遍历，不能宣称不存在任何相关提交／讨论。除了已引用的 `function` 邮件，没有找到针对本报告整套布局改造的上游接受意见。

These gaps are `NOT_AVAILABLE`. No exhaustive traversal of upstream history was performed; absence of a found rationale does not prove no discussion exists. Apart from the cited function-buffer exchange, no upstream acceptance decision for the proposed full-layout changes was found.
