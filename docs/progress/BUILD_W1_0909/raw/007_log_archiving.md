# 完整日志归档

命令原文：

```bash
mkdir -p docs/progress/BUILD_W1_0909/build_logs
cp tmp/BUILD_W1_0909/armv7l_gcc_nolibcxx.log docs/progress/BUILD_W1_0909/build_logs/armv7l_gcc_nolibcxx.log
cp tmp/BUILD_W1_0909/aarch64_gcc_nolibcxx.log docs/progress/BUILD_W1_0909/build_logs/aarch64_gcc_nolibcxx_partial.log
cp tmp/BUILD_W1_0909/x86_64_gcc_nolibcxx.log docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_lock_failure.log
cp tmp/BUILD_W1_0909/x86_64_gcc_nolibcxx_retry.log docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_partial.log
gzip -n docs/progress/BUILD_W1_0909/build_logs/*.log
sha256sum docs/progress/BUILD_W1_0909/build_logs/*.gz > docs/progress/BUILD_W1_0909/build_logs/SHA256SUMS
printf 'archive_exit=0\n'
cat docs/progress/BUILD_W1_0909/build_logs/SHA256SUMS
```

输出：

```text
archive_exit=0
f2d372e030b8e7d78aabec283f27821f2c87ca8e622ac071936c75a552289561  docs/progress/BUILD_W1_0909/build_logs/aarch64_gcc_nolibcxx_partial.log.gz
886b497706a09b5bd01bcf9f04176c218d084c35c212c64d732c745799df1bd8  docs/progress/BUILD_W1_0909/build_logs/armv7l_gcc_nolibcxx.log.gz
ea225587dc81a9b205ff70e1adf05d68d9ca8a9c6ebc85bce977f14418698005  docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_lock_failure.log.gz
5b7a9e4260ba0cc459f91fce2e961275b3c7d359710e3afd475fb93f3558c211  docs/progress/BUILD_W1_0909/build_logs/x86_64_gcc_nolibcxx_partial.log.gz
```

退出码：`0`。
