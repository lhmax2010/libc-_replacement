# LLVM 候选与远端只读核验

命令原文：

```bash
git -C tmp/BUILD_W1_0908/llvm-worktree status --short
git -C tmp/BUILD_W1_0908/llvm-worktree rev-parse HEAD
git -C tmp/BUILD_W1_0908/llvm-worktree ls-remote --heads origin tizen_base sandbox/lhmax2025/libcxx-migration
```

退出码：`0`。

构建完成时 HEAD 仍为：

```text
2d23367d74afbf2bb1e9e4013fce072b3a154109
```

远端关键分支：

```text
2d23367d74afbf2bb1e9e4013fce072b3a154109  refs/heads/sandbox/lhmax2025/libcxx-migration
2d23367d74afbf2bb1e9e4013fce072b3a154109  refs/heads/tizen_base
```

候选工作树只有 `packaging/llvm.spec` 与三份 TF 2.18 资产构成拟议改动。
GBS 再次生成的根目录 `llvm.manifest` 已在收口时删除，并核验未纳入拟议
提交。本轮未创建 LLVM commit，未执行 package 仓 push。
