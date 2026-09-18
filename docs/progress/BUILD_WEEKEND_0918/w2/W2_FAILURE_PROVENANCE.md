# armv7l 整轮写包失败：既有证据与待验证事项

本页是 W2 开始前对历史记录的核对，不表示本轮重试已执行。

## 两个不同错误，不合并归因

1. 旧 LLVM 整轮写包失败：`progress/BUILD_STATIC_0917B/resume_0917/cells/llvm-arm-rpm-serial/build.log:7560` 的实际输出是：

   ```text
   error: create archive failed: cpio: write failed - Function not implemented
   ```

   前一行是 `clang-devel-debuginfo` 的 Wrote 记录，但日志没有指出下一份失败包的文件名，不能据顺序推定失败包身份。整轮退出 1 与 18 份单独通过核验的 RPM 是两种不同事实。该失败不能改写为 rsync EPERM；当时具体 syscall 及其原因未观测。

2. 后续复制失败：`progress/BUILD_STATIC_0917B/resume_0917/copy_rsync.strace:155`–156，针对 `usr/bin/llvm-opt-report`，`openat(... O_RDONLY|O_NOFOLLOW)` 成功，随后 `read(3, ..., 40896) = -1 EPERM`；同一受控诊断的 rsync 退出 23。

   同目录 `copy_read_diagnostic.json` 记录 cp 退出 0，复制件及来源 SHA256 均为 `55629940b16d1ca7c100f17a41f3e1677f7ec3726449d9d8fa0f462f9b941a35`，大小 40,896 字节。具体为何出现进程间读取差异仍未查明；不据此认定文件损坏，也不把工具退出非零忽略掉。

## 本轮准备的验证范围

按人工批准使用 cp -a，逐文件核验；W1 输入仍标记 `USABLE_BUT_BUILD_INCOMPLETE`。W2 将在新的输出目录重试完整 `rpmbuild -bb`，复用已批准的构建树，不伪称从空目录重新编译全部 LLVM。计划记录失败的文件、read/pread/write/pwrite、lseek、ioctl、copy_file_range/sendfile 系统调用，并给文件描述符加路径注释；不能仅凭 cpio 的文字预判底层一定是 write 失败。脚本存在不代表已取得 trace。只有整轮退出 0 且全部新 RPM 核验通过，才给新一轮标完整。

本轮核对命令和实际摘录见 raw/308、309。raw/307 曾给出一个不存在的历史交付路径，退出 2；308 已通过实际文件索引定位到 `docs/progress/BUILD_STATIC_0917B/resume_0917/resume_stop_1909/FINAL_RESULT.md`，不补写不存在的原记录。
