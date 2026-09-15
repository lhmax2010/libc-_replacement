# 最终状态：PARTIAL

本次确认后最后一格于2026-09-15 20:20:24 +08构建完成，未超十二小时。
LLVM armv7l输入已完整写包；bcc-tools六格闭合并推送
`5bd0654e24bb231035ea684d278f674aa95a2cd2`。
bpftrace四格完整构建及源码程序/实际LLVM与BCC提供方检查均通过，
但预编译static子包内嵌标准库未观测、推送门禁范围待人工裁决，未提交、未推送。
总计10包完成并推送、1包PARTIAL。未使用开发板，未进行内核跟踪功能实测。

最新结论见FINAL_REPORT.md、MATRIX.md、PACKAGE_STATUS.md。
以下为过程快照，明确保留其时间属性，不作为当前未完成项：

## 过程快照（历史）

首轮开工：2026-09-15 15:08 +08；首轮已因缓存问题停下并交审。
人工确认后重新开工：2026-09-15 15:51:49 +08；新截止：2026-09-16 03:51:49 +08。

当前状态：进行中。LLVM armv7l 已完成7146/7146并写出22个RPM，
内外层退出码均0；RPM内libLLVM的libc++/libc++abi/libgcc_s依赖检查通过，
不含libstdc++依赖。bcc-tools六格通过并推送5bd0654e24bb231035ea684d278f674aa95a2cd2；
45个远端分支仅目标sandbox改变，其他44个未变。bpftrace的armv7l/libc++格
正在构建；其static预编译子包的门禁范围另待人工确认。
续跑证据见 CONTINUATION_REPORT.md。以下保留首轮问题说明，不能视为问题未发生。

## 资源控制

- medium 门禁退出 0（raw/001）。构建 jobs=1、nice=19、ionice=3。
- systemd 用户 scope 的 MemoryMax 请求值为总内存的一半：
  16,536,451,072 字节；内核按页取整实际执行 16,536,449,024 字节。
  已检查 GBS、chroot/QEMU、rpmbuild 后代位于同一 scope。
- 每 500 个目标读写探测一次；超过 30 秒冻结整个构建 scope，等待
  600 秒后复测；三次暂停后仍不通过则停止并保存断点。
- freezer 用独立短任务校准，冻结时 cgroup.events 显示 frozen=1，
  恢复后正常退出。systemctl 的 --no-block freeze 触发客户端断言，
  已改为不带该参数；错误原文保留，不视为构建判据失败。

## 续跑技术故障（必须保留在最终报告）

原断点预检：801/7147 已完成，ninja -n 剩余 6346。
直接 rpmbuild --noprep --noclean 保留构建树，但首次启动的 PATH
使 CMake 将原 /bin 编译器路径换成 /usr/bin 路径，触发缓存重置。
重置后 LLVM_ENABLE_LIBCXX=OFF、项目集合为空、构建类型为 Release，
与目标不符；发现后立即冻结并停止该次构建。该次输出不得计入验证。
已核查 scope inactive；日志保留 raw/011–017。

已用原 /bin 编译器路径及导出 spec 的完整参数恢复构建配置，
未删除原构建树。恢复后配置值自检通过，但 Ninja 预演为 7146 个
待处理目标，不再是原来的 6346；详见 REPORT.md。未启动恢复后的编译。
配置恢复脚本首试因 set -u 遇到原 RPM 脚本中的未定义 PKG_CONFIG_PATH
而退出；改为与原脚本一致的 set -e，记录 raw/018–019。

## 包门禁

- bcc-tools：沿用 aarch64/x86_64 四格 PASS；armv7l 两格未开始。
- bpftrace：前置尚未闭合，未实施；本轮按人工确认仅需 armv7l/aarch64
  四格，x86_64 不在 ExclusiveArch 中，不列失败格。
- 未连接或占用开发板。
