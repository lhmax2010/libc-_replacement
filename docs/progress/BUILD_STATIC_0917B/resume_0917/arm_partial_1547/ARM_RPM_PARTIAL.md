# armv7l LLVM 写包：部分完成，整轮失败

完整命令及退出码：raw/202_arm_serial_rpm_build.*。独立日志：cells/llvm-arm-rpm-serial/build.log。15:00:14 启动，15:47:27 返回 1，约 47 分 13 秒，96 个增量目标公告。

实际错误：

```
error: create archive failed: cpio: write failed - Function not implemented
```

在此之前日志写出 18 个 RPM，最后两项为 clang-debuginfo 和 clang-devel-debuginfo。错误没有指出失败包名，故不把“下一项可能为 lldb-debuginfo”写成确认结论。未删除失败现场。

只读核查 raw/261 返回 0：18 个已写出文件逐个通过摘要校验，压缩参数均为 xz/5T1；从 llvm-static-devel 实际提取的 libclang.a 有 1062 行 `_ZNSt3__1`、0 行 `_ZNSt7__cxx11`。从 libllvm RPM 提取的 libLLVM 依赖 libc++、不依赖 libstdc++。完整文件身份、SHA256、成员与动态依赖在 verify_armv7l/。

这些事实不代表完整 rpmbuild 成功。是否可将这个单独核验的静态开发 RPM 作为后续输入已交人工确认；未自行放宽整轮门禁。aarch64 的完整 RPM 结果不受影响。

资源方面，第一次 armv7l 尝试的 XZ 多线程遗漏已停止并单列保留；本次重试使用显式 w5T1.xzdio。资源监控没有 OOM；ENOSYS 的准确原因仍未观测，不用推测补全。
