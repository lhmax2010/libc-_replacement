# R38 代码交付索引说明

- `tests/s1_rethrow_probe.cpp`：逐字复制 R34-B 用例，SHA256 为 `2e04b02c7a2b307d84faa482d6bb594e54b7e55e3cfd04e1f475e13ffb703cf3`。
- `ehabi_field_map_after_headers.h`：仅用于使同一用例源码中的 Itanium 字段名在 ARM EHABI 结构上可编译；映射为 `unwinder_cache.reserved1`，不改用例控制流或判据。
- `implementation/s1_complete_vs_platform.diff`：沿用的 R34-B 三处改动完整 diff。
- `implementation/current/`：三处被改文件的完整当前内容。
- `build/cmake/CMakeCache.unpatched-armv7l.txt`：未打补丁对照库的完整 CMakeCache。
- `build/tools/`：实际交叉编译包装器。
- `../tools/`：资源门禁、构建监控、部署、矩阵和清理脚本的完整源码。
- `../commands/`：所有编译、部署、运行及退出码原文；矩阵完整输出为 `040_board_matrix.log`。

R38 没有新增 libc++abi 功能改动；实验版沿用 R34-B 三处改动，另构建未打补丁对照库。
