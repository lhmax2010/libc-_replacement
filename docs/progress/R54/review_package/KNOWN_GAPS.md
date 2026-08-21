# R54 NOT_AVAILABLE / NOT_OBSERVED

| 项目 | 状态 | 原因 |
|---|---|---|
| armv7l 上游 libc++ 选定源码测试 | NOT_AVAILABLE | 板上无编译器与 lit；本任务没有与 x86_64 七项同构的预编译 ARM 集合，禁止推断补全 |
| `basic_string::shrink_to_fit` 强制展开站点 | NOT_OBSERVED | 已知 `noexcept` 帧使防护不可达，本任务按任务书不重复构造 |
| `basic_ostream<char>::sentry::~sentry()` 强制展开站点 | NOT_OBSERVED | 已知隐式 `noexcept` 帧使防护不可达，本任务按任务书不重复构造 |
| aarch64 | NOT_AVAILABLE | 本任务架构范围仅 x86_64 与 armv7l |
| LTO、插件边界、混合运行时帧 | NOT_OBSERVED | 不在本任务构建/运行矩阵内 |

