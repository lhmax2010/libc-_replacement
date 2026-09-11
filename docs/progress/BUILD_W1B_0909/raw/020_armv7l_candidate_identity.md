# armv7l 保留候选身份核对

保留候选为成功日志在 2026-09-09 写出的 12 个 release 109.3 RPM。日志显示：

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-DCMAKE_CXX_COMPILER=armv7l-tizen-linux-gnueabi-g++
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
+ tar -C mlgo_verify_assets -xzf .../mlgo_arm_model.tar.gz
[7745s] toolchain-2026 finished "build llvm.spec"
```

`-stdlib=libc++`、`-lc++abi`、`libc++-devel`、`libc++abi-devel` 的日志
命中数均为 0。仓地址是本次 `gbs.conf` 的 Tizen-Base 与 Tizen-Unified
reference 仓。

核对命令：

```bash
sha256sum tmp/BUILD_W1_0908/llvm-worktree/packaging/llvm.spec \
  tmp/GBS-ROOT/BUILD-W1-0908-llvm-armv7l/local/sources/tizen_unified_standard/llvm-22.1.8-1/llvm.spec
diff -u <exported-spec> <candidate-spec>
```

diff 只有 GBS 导出时插入的一行：

```diff
-VCS:       platform/upstream/llvm#2d23367d74afbf2bb1e9e4013fce072b3a154109-dirty
```

除此以外 spec 内容相同，因此把这 12 个 RPM 隔离复制到
`tmp/BUILD_W1BC_0911/rpms/armv7l_candidate/` 用作候选。
