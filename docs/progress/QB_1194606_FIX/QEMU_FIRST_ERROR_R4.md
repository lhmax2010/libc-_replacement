# qemu-accel 首个确定的验收失败（不是编译错误）

armv7l 与 aarch64 变体均在 x86_64 构建退出 0，生成 clang-accel RPM，要求的两条 .so.1 路径均存在，原 unpackaged 报错均已实测消失。

与前一版同变体 aarch64 的 clang-accel-0.4-2.1.x86_64.rpm 对照，候选新增 8 项、删除 0 项，不满足“其它清单条目只多这两条”。新增原文：

```text
/emul/usr/lib64/libLLVM.so.22.1
/emul/usr/lib64/libatomic.so.1
/emul/usr/lib64/libatomic.so.1.2.0
/emul/usr/lib64/libc++.so.1
/emul/usr/lib64/libc++.so.1.0
/emul/usr/lib64/libc++abi.so.1
/emul/usr/lib64/libc++abi.so.1.0
/emul/usr/lib64/libclang-cpp.so.22.1
```

原始清单 diff：verify-qemu-accel-aarch64_R4/clang-accel-file-list.diff；机器判定：同目录 result.json；完整命令输出：raw/R4_029_qemu_aarch64_verify.*。前版 SHA fd0ddae1d9cde87a07a3e3993768de668c3d8e698e2fd0ebc5d0f54ed3c99c45。

armv7l 没有取得对应变体的前版 RPM，因此该项为 NOT_OBSERVED；与上述 aarch64 前版的跨变体差异完整另存，不当作同变体回归证据。

按 R4“任一包验收失败，保留现场、停止该包”，在 aarch64 的确定 FAIL 后不启动 riscv64，记 NOT_OBSERVED，不把其宏展开检查冒充完整构建。qemu 候选 3b8d6274 内容不变、不推；不自行忽略新增项、不修改配方。两格根与 RPM 全部保留在 tmp/QB_1194606_FIX/GBS-R4/ 下。
