# 既有材料对照

这里核对的是范围落点，不重复验证旧实验，也不把旧版报告的统计复制为当前统计。

| 原材料组 | 项 | 对应实体 | 横切维度 | 状态 |
|---|---|---|---|---|
| P9 十个类型 | any | API-02574:std::any | CROSS-11 | MAPPED |
| P9 十个类型 | function<void()> | API-04497:std::function | CROSS-11 | MAPPED |
| P9 十个类型 | deque<int> | API-03366:std::deque | CROSS-11 | MAPPED |
| P9 十个类型 | map<int,int> | API-05106:std::map | CROSS-11 | MAPPED |
| P9 十个类型 | set<int> | API-06173:std::set | CROSS-11 | MAPPED |
| P9 十个类型 | unordered_map<int,int> | API-07153:std::unordered_map | CROSS-11 | MAPPED |
| P9 十个类型 | unordered_set<int> | API-07207:std::unordered_set | CROSS-11 | MAPPED |
| P9 十个类型 | string | API-06321:std::string | CROSS-11 | MAPPED |
| P9 十个类型 | future<int> | API-04499:std::future | CROSS-11 | MAPPED |
| P9 十个类型 | promise<int> | API-05576:std::promise | CROSS-11 | MAPPED |
| R66 二十五项 | R66-001 |  | CROSS-01 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-002 |  | CROSS-02 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-003 |  | CROSS-03 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-004 |  | CROSS-04 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-005 |  | CROSS-05 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-006 |  | CROSS-06 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-025 |  | CROSS-25 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-007 |  | CROSS-07 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-008 |  | CROSS-08 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-009 |  | CROSS-09 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-010 |  | CROSS-10 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-011 |  | CROSS-11 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-012 |  | CROSS-12 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-013 |  | CROSS-13 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-014 |  | CROSS-14 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-015 |  | CROSS-15 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-016 |  | CROSS-16 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-017 |  | CROSS-17 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-018 |  | CROSS-18 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-019 |  | CROSS-19 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-020 |  | CROSS-20 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-021 |  | CROSS-21 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-022 |  | CROSS-22 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-023 |  | CROSS-23 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| R66 二十五项 | R66-024 |  | CROSS-24 | MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY |
| ABI 报告第四节 | 标记类型 | API-03451:std::exception | CROSS-01 | MAPPED |
| ABI 报告第四节 | typed catch 防护位置 | API-02828:std::basic_ostream;API-04499:std::future | CROSS-01;CROSS-05 | MAPPED |
| ABI 报告第四节 | wait 异常规格 | API-03233:std::condition_variable | CROSS-01;CROSS-08 | MAPPED |
| ABI 报告第四节 | 析构同步 | API-07284:std::wbuffer_convert | CROSS-21 | MAPPED |
| ABI 报告第四节 | 读写锁后端/状态回滚 | API-06209:std::shared_mutex;API-06214:std::shared_timed_mutex | CROSS-01;CROSS-24 | MAPPED |
| 七个问题 | 1 应用 catch-all 吞强制展开 | API-03451:std::exception;API-03452:std::exception_ptr | CROSS-01;CROSS-10;CROSS-22 | MAPPED |
| 七个问题 | 2 标准库内部 catch-all 防护 | API-02782:std::basic_ios;API-02828:std::basic_ostream;API-04499:std::future;API-05451:std::packaged_task | CROSS-01;CROSS-05 | MAPPED |
| 七个问题 | 3 noexcept/锁状态/内联重编边界 | API-03233:std::condition_variable;API-03235:std::condition_variable_any;API-07284:std::wbuffer_convert;API-06209:std::shared_mutex | CROSS-01;CROSS-08;CROSS-11 | MAPPED |
| 七个问题 | 4 八处对应关系与扩展 | API-03235:std::condition_variable_any;API-00288:__gnu_cxx::__versa_string;API-07032:std::tr2::dynamic_bitset | CROSS-06 | MAPPED |
| 七个问题 | 5 string 收缩与 ostream sentry 析构 | API-02887:std::basic_string;API-02828:std::basic_ostream | CROSS-01;CROSS-13 | MAPPED |
| 七个问题 | 6 异步取消可能绕过防护 | API-02625:std::atomic | CROSS-01;CROSS-22;CROSS-24 | MAPPED |
| 七个问题 | 7 其他二十五项风险，不再新增二十五个类型 |  | CROSS-01 至 CROSS-25 | MAPPED |
| 评审议题 | 无序容器迭代顺序 | API-07153:std::unordered_map;API-07207:std::unordered_set | CROSS-21 | MAPPED_ORIGINAL_REVIEW_QUOTE_NOT_OBSERVED |
| 评审议题 | hash 值稳定性/跨库/持久化 | API-04562:std::hash;API-07153:std::unordered_map;API-07207:std::unordered_set | CROSS-21 | MAPPED_ORIGINAL_REVIEW_QUOTE_NOT_OBSERVED |
| 评审议题 | 类型信息比较策略 | API-07085:std::type_info;API-07084:std::type_index | CROSS-10 | MAPPED_NOT_REASSESSING_OLD_EXPERIMENT_STATUS |
| 评审议题 | GNU 空字符串存储释放 | API-02887:std::basic_string | CROSS-12;CROSS-13 | MAPPED_NOT_REASSESSING_OLD_EXPERIMENT_STATUS |
| 评审议题 | libsupc++ 与展开器组合 | API-03452:std::exception_ptr | CROSS-01;CROSS-10;CROSS-22 | MAPPED_CONFIGURATION_AXIS |
| 评审议题 | veneer/薄转发层路由 | API-06321:std::string;API-07251:std::vector | CROSS-08;CROSS-17;CROSS-23 | MAPPED_DEPLOYMENT_AXIS_NOT_PUBLIC_ENTITY |

## 无法放进单一类型/函数行的项目

运行库、符号版本、编译模式、预制二进制、实际加载身份与测试发现是横切维度。它们已在 CROSSCUTTING_SCOPE.tsv 单列，不伪造为公开标准库类型。
无序容器顺序/hash 值已依本轮任务要求纳入；在已读 adaptor 评审原文中未定位其逐字引文，因此未冒称找到了原评审出处。
七个问题的原件不在当前分支 docs/hq；从同一工作区旧材料目录读取并冻结到 headers/prior_seven_issues.md。范围映射保留原件版本，并引用后续修正记录 R74/R75；不据旧件宣称当前补丁状态。
