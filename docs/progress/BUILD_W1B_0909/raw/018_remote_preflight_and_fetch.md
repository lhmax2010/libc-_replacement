# 目标分支预检与 fetch 技术性中止

远端只读查询：

```bash
git -C tmp/BUILD_W1_0908/llvm-worktree ls-remote origin \
  refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
```

输出与退出码：

```text
c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13 refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport
EXIT=0
```

首次常规 fetch：

```bash
git fetch origin sandbox/lhmax2025/libcxx-ehabi-backport
```

服务器要求发送 `6810150` 个对象；接收约 160 MiB 时为避免占满磁盘发送
Ctrl-C，中止退出码 `1`。中止留下两个明确的未完成临时包：

```text
200082822 .../.git/objects/pack/tmp_pack_VI9vBH
175514697 .../.git/objects/pack/tmp_pack_lek3VC
```

只删除这两个 `tmp_pack_*` 后，改用单分支浅 fetch：

```bash
mkdir -p tmp/BUILD_W1BC_0911/target-fetch
git -C tmp/BUILD_W1BC_0911/target-fetch init
git -C tmp/BUILD_W1BC_0911/target-fetch remote add origin \
  ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm
git -C tmp/BUILD_W1BC_0911/target-fetch fetch --depth=1 --no-tags origin \
  sandbox/lhmax2025/libcxx-ehabi-backport
```

输出终态：

```text
remote: Total 177323 (delta 33318), reused 115874 (delta 33318)
From ssh://review.tizen.org:29418/platform/upstream/llvm
 * branch sandbox/lhmax2025/libcxx-ehabi-backport -> FETCH_HEAD
EXIT=0
```

目标分支与验证基线的 `packaging/llvm.spec` SHA256 均为：

```text
7962d176484920de6138d70c7e8a8e1764dfada6655abf0332e9748167e889fd
```
