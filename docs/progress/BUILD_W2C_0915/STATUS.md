# 低资源续跑状态

开工：2026-09-15 15:08 +08；硬截止：2026-09-16 03:08 +08。

状态：PARTIAL，等待人工确认恢复后的重编范围；无新增通过格，无包提交或包推送。

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
