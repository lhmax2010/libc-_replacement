# NOT_OBSERVED / NOT_AVAILABLE

- 调试器附加：宿主 `ptrace_scope` 拒绝 attach；由独立状态探针替代。
- std::async 内部线程 `pthread_join`：libc++ 将该线程 detach，调用方无合法 join 所有权，记 NOT_AVAILABLE。
- R54 future 并发子集：R54 并发用例不含 future 设施，分母为 0，记 NOT_AVAILABLE。
- aarch64、LTO、混合运行时帧穿越：未执行。
- 补丁作者身份：人工 name/email 未指定，记 NOT_AVAILABLE，待 amend。
