# W2：armv7l LLVM 整轮补做

## 结论

**PARTIAL：整轮仍失败；未升级为完整构建。** 本次 96 个增量目标完成，但完整 `rpmbuild -bb` 于 2026-09-18 19:15:29 退出 **1**，重新写出的 18 个 RPM 单独核验通过。此前获准使用的 18 个输入仍保持 `USABLE_BUT_BUILD_INCOMPLETE`，原记录不修改。

W2 开始 18:21:21，原截止次日 00:21:21；本次没有超时、没有使用 sudo。资源预检见 raw/427；运行 scope 实际 `MemoryMax=16536457216`，nice19、idle I/O，Ninja 并行 1。使用批准的准备树增量执行完整写包命令，不宣称 clean-room 重建。

## 执行与失败点

- raw/430 准备独立输出目录；raw/431 记录完整命令；raw/432 为本次完整执行，退出 1。
- 候选 spec SHA256：`cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`；未修改平台树。
- 新目录：`tmp/WEEKEND_0918/rpm-arm-llvm/`；不覆盖旧 `tmp/STATIC_0917B/rpm-arm-llvm/`。
- `cells/llvm-arm-whole-retry/build.log:7559` 最后成功记录为 `clang-devel-debuginfo-22.1.8-1.armv7l.rpm`。
- 紧接着第 7560 行：`error: create archive failed: cpio: write failed - Function not implemented`。
- 此位置及错误文本与前次一致（此前证据见 `W2_FAILURE_PROVENANCE.md`、`previous_failure/`）。日志不能确定失败的下一份包名，记 `NOT_OBSERVED`，不根据排序猜测。

## 与 rsync EPERM 的区别

前次复制中 `rsync` 对 `llvm-opt-report` 的 `read` 返回 EPERM，`cp -a` 对同文件成功且 SHA256 相同，原 strace 与命令已保存。本次沿用已完成并核验的 cp 输入，不重新复制，不引入 rsync。故本次重新出现的是 **RPM 归档错误**，不能把它归因于同一次 rsync 失败。

本次 `strace -f -yy -s160` 记录选定的文件、读写、定位等系统调用的失败返回，完整命令见 raw/431；原始跟踪为 `llvm-arm-rpm-failed-syscalls.strace`（交付时 gzip，并记录解压后 SHA256）。raw/493 对该跟踪检索 `= -1 (ENOSYS|EPERM)`，退出 1，即在**所选宿主系统调用的失败记录**中零命中。这不证明所有层不存在该错误，也不能据此断言是内核、QEMU 或 RPM 库哪一层造成。根因仍为 `NOT_OBSERVED`；需要更细的来宾/RPM 归档层诊断，留待单独处理，不为跑完而改系统或跳过子包。

## 产物核验

raw/492 的独立核验退出 0；`llvm_retry_partial/result.json` 给出 18 份 RPM 的完整路径、大小、SHA256、包名和文件清单。18 条 `Wrote` 与 18 个实际 RPM 精确对应，全部通过 RPM digest 检查。

从实际 `llvm-static-devel` RPM 解出的 `libclang.a`：

- SHA256 `6b5e306dc7a575cb4a1b2e7ee54f9cda999af4ebe16cbdf935840c1fbbe8e96f`；
- `strings` 中 `_ZNSt3__1` 1062 行、`_ZNSt7__cxx11` 0 行；
- 包内 `libLLVM.so.22.1` 依赖 libc++、不依赖 libstdc++。

新归档 SHA256 与前次已核验输入 **不同**（前次 `9884348b…2bc374`）；不声称逐字节复现，也不将此次新产物替换 W1 已使用的输入。该差异的归因和完整功能等价本阶段未验证。符号标记只证明观察到的 ABI 痕迹。

## 下一阶段

不再盲目重试整轮写包。W1 的四个回退格结论保留；ARM libc++ 主包未写出、aarch64 static 额外依赖 libbpf 的两个缺口不因此消失。W3 可独立执行同一套已核验 ARM 静态运行库的取消探针；安装/功能等价格因缺合格候选 RPM 不得记通过。
