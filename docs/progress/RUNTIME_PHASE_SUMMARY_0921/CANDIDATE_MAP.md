# Candidate-to-edge mapping

[Evidence: base-map](EVIDENCE.md#base-map)

| Candidate | Edge row | Consumer → provider |
| --- | --- | --- |
| std::shared_ptr | 1 | appcore-agent → app-core |
| std::string | 2 | amd → bundle |
| std::vector | 2 | amd → bundle |
| std::basic_string | 2 | amd → bundle |
| std::allocator | 2 | amd → bundle |
| std::pair | 3 | amd → bundle |
| std::initializer_list | 3 | amd → bundle |
| std::wstring | 4 | ise-engine-anthy → isf |
| std::deque | 5 | xwalk-extensions-common → jsoncpp |
| std::stack | 5 | xwalk-extensions-common → jsoncpp |
| std::atomic | 6 | grpc → abseil-cpp |
| std::unordered_map | 7 | com.samsung.dali-demo → dali2-adaptor |
| std::tuple | 7 | com.samsung.dali-demo → dali2-adaptor |
| std::array | 7 | com.samsung.dali-demo → dali2-adaptor |
| std::hash | 7 | com.samsung.dali-demo → dali2-adaptor |
| std::equal_to | 7 | com.samsung.dali-demo → dali2-adaptor |
| std::unique_ptr | 8 | dali2-adaptor → dali2 |
| std::default_delete | 8 | dali2-adaptor → dali2 |
| std::stringstream | 9 | enlightenment → gtest |
| std::streambuf | 9 | enlightenment → gtest |
| std::ios_base::openmode | 9 | enlightenment → gtest |
| std::filesystem::path | 10 | app-installers → manifest-parser |
| std::list | 11 | app-installers → cert-svc |
| std::function | 12 | com.samsung.dali-demo → dali2-toolkit |
| std::string_view | 13 | dali2-toolkit → dali2 |
| std::basic_string_view | 13 | dali2-toolkit → dali2 |
| std::chrono::duration | 14 | dali2-toolkit → dali2 |
| std::chrono::time_point | 14 | dali2-toolkit → dali2 |
| std::chrono::steady_clock::time_point | 14 | dali2-toolkit → dali2 |
| std::chrono::nanoseconds | 14 | dali2-toolkit → dali2 |
| std::chrono::steady_clock::duration | 14 | dali2-toolkit → dali2 |
| std::type_info | 15 | dali2-toolkit → dali2 |
| std::map | 16 | capi-media-vision → inference-engine-interface |
| std::less | 16 | capi-media-vision → inference-engine-interface |
| std::ostream | 17 | app-core → gtest |
| std::ios | 17 | app-core → gtest |
| std::ios_base | 17 | app-core → gtest |
| std::locale | 17 | app-core → gtest |
| std::basic_ostream | 17 | app-core → gtest |
| std::ios_base::fmtflags | 17 | app-core → gtest |
| std::multimap | 18 | data-provider-master → notification |
| std::unordered_set | 19 | libzypp-bindings → libzypp |
| std::set | 20 | libzypp-bindings → libzypp |
| std::istream | 21 | libzypp-bindings → libzypp |
