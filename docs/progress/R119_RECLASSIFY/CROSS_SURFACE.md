# 已确认跨包面的计数单位

- 候选：44 个（第一批 34、第二批 10），原阳性结论均保留。
- 有向源码包对：16 条，键为消费包 → 提供包。
- 实际 ELF 依赖边：16 条，键为架构、消费 ELF SHA256、提供 ELF SHA256。
- 原始符号边：21 条，在 ELF 边键上再加原始导入/导出符号。构造/析构不同原始名字若存在会分别计数。
- 候选—原始符号关联：44 行，不是独立边数。

换算不是按固定比例乘除：`CROSS_CANDIDATE_EDGE_MAP.tsv` 的每行把一个候选映射到实际证实的边；去掉候选列后，分别按上述键去重。一个 vector 边可证明 string 元素、allocator 子对象等多个候选；同一个包对也可以有多个接口与 ELF。同一候选若有多条已选证据边可关联多行。不得把 44 加到包边数上。

这些数是当前选定阳性证明所覆盖的面，不是所有平台包边的全集，也不是不兼容次数。gtest 消费方含真实 RPM 的测试 ELF，不宣称均为业务生产调用。没有在本轮追加跨包运行实验。

| 消费包 → 提供包 | 关联候选数 | 候选 |
|---|---:|---|
| amd → bundle | 6 | std::allocator;std::basic_string;std::initializer_list;std::pair;std::string;std::vector |
| app-core → gtest | 6 | std::basic_ostream;std::ios;std::ios_base;std::ios_base::fmtflags;std::locale;std::ostream |
| app-installers → cert-svc | 1 | std::list |
| app-installers → manifest-parser | 1 | std::filesystem::path |
| appcore-agent → app-core | 1 | std::shared_ptr |
| capi-media-vision → inference-engine-interface | 2 | std::less;std::map |
| com.samsung.dali-demo → dali2-adaptor | 5 | std::array;std::equal_to;std::hash;std::tuple;std::unordered_map |
| com.samsung.dali-demo → dali2-toolkit | 1 | std::function |
| dali2-adaptor → dali2 | 2 | std::default_delete;std::unique_ptr |
| dali2-toolkit → dali2 | 8 | std::basic_string_view;std::chrono::duration;std::chrono::nanoseconds;std::chrono::steady_clock::duration;std::chrono::steady_clock::time_point;std::chrono::time_point;std::string_view;std::type_info |
| data-provider-master → notification | 1 | std::multimap |
| enlightenment → gtest | 3 | std::ios_base::openmode;std::streambuf;std::stringstream |
| grpc → abseil-cpp | 1 | std::atomic |
| ise-engine-anthy → isf | 1 | std::wstring |
| libzypp-bindings → libzypp | 3 | std::istream;std::set;std::unordered_set |
| xwalk-extensions-common → jsoncpp | 2 | std::deque;std::stack |

新增 NO_STDLIB_OBJECT 仅用于原 156 项未定的结构定界；它不否认标签或空子对象能够真实跨包。已有 less/hash/allocator 等实际存储链不重判、不删边。
