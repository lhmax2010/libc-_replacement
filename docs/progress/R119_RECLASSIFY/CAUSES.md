# 未定项成因分组（重判前）

共 156 项；主因互斥，辅因可重叠。A/B 是待本轮证据检查的处理通道，不是已通过的结论。

| 主因 | 数量 | 样例 |
|---|---:|---|
| A | 89 | std::size_t、std::numeric_limits、std::is_same、std::streamsize、std::enable_if、std::true_type、std::false_type、std::is_base_of、std::nullptr_t、std::ptrdiff_t、std::re |
| B | 5 | std::launch、std::smatch、std::filesystem::directory_iterator、std::future_status、std::wstring_convert |
| C | 23 | std::exception、std::runtime_error、std::invalid_argument、std::bad_alloc、std::chrono::milliseconds、std::out_of_range、std::chrono::seconds、std::regex、std::chrono:: |
| D | 39 | std::ifstream、std::mutex、std::ostringstream、std::lock_guard、std::unique_lock、std::thread、std::ofstream、std::istringstream、std::condition_variable、std::queue、std |

逐项原观察、实际缺口、本线可解性、来源摘要见 CAUSES_156.tsv。未修改两个原定界表。已有 allocator/hash/less 成员存储链、duration 与特化的区分保持不动。
