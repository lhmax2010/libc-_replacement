# R96 关键证据索引

- 资源门禁：`raw/001_resource_gate_medium.*`，退出码 0。
- aarch64 编译器名门禁：`raw/011_aarch64_compiler_name_gate_final.*`。
  `%{_host}` 展开为 `aarch64-tizen-linux-gnu`；构建根内存在
  `aarch64-tizen-linux-gnu-clang` 与 `-clang++`，其目标为 `clang-22`；
  同构建族历史日志确认该名称实际识别为 Clang 22.1.8。
- 修改前分支、spec 与全部 sandbox 分支：`raw/012_premodify_branch_and_spec_state.*`。
- 候选完整 diff：`raw/013_spec_change_diff.*`。
- 候选工作树范围：`raw/014_candidate_worktree_cleanliness.*`。
- armv7l / GCC 环境完整构建：`raw/015_build_armv7l_gcc_env.*`，退出码 0。
- armv7l / GCC 环境产物核验：`raw/016_verify_armv7l_gcc_env.*`，退出码 0。
- armv7l / LLVM 环境完整构建：`raw/017_build_armv7l_llvm_env.*`，退出码 0。
- armv7l / LLVM 环境产物核验：`raw/018_verify_armv7l_llvm_env.*`，退出码 0。
- aarch64 / GCC 环境完整构建尝试：`raw/019_build_aarch64_gcc_env.*`，退出码 1。
- aarch64 / GCC 环境失败摘录：`raw/020_inspect_aarch64_gcc_env_failure.*`。
- 停止后的本地/远端分支与矩阵退出码核验：`raw/022_stop_audit.*`。
- 两次日志包装器技术性误用：第一次未能落盘；第二次由
  `raw/021_stop_audit_bash.*` 记录为退出码 127。两次均未执行核验主体，
  未改变 Git 或构建状态；随后使用正确调用生成 `raw/022_*`。

完整命令在每组 `.command.txt`，标准输出、标准错误和退出码分别在
`.stdout`、`.stderr`、`.exitcode`，未裁剪。
