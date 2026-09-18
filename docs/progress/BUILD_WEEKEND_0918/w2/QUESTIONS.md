# 疑问与口径说明

1. 六格 = 两架构 × 三路径。每格至少主包和 static 子包，故实际 RPM 文件数不应强行写成六。逐文件列清单。
2. armv7l LLVM 输入按人工裁决标记 USABLE_BUT_BUILD_INCOMPLETE；W1 不抹去整轮 rpmbuild 退出 1，W2 独立追查与补做。
3. armv7l 原生工具等价抽查/切换权限仍待回复；未答复前保留 QEMU，不套用 aarch64 的既有许可。
4. W4 的机制表述将区分已测 glibc/libgcc 组合与所有平台的普遍结论；不能用源码检索或上游 musl 构建选择冒充所有 musl 版本已实测。具体证据在 W4 核对。

5. 13:40：aarch64 libc++ 完整 rpmbuild 已退出 0（076），但 static ELF 的 DT_NEEDED 额外含 `libbpf.so.1`，产物门禁失败（105）。定位到本次输入源码 `src/ast/CMakeLists.txt:86`：`target_link_libraries(ast PUBLIC ${LIBCLANG_STATIC_ABS_PATH} bpf clangIndex clangDriver clangFrontend clangCodeGen)`；其中裸 `bpf` 成为 `-lbpf`，而 FindLibBpf 已找到 `/usr/lib64/libbpf.a`。已询问人工是否允许在 tmp 候选 spec 的 libc++ 条件内仅修正该静态分支为 `${LIBBPF_LIBRARIES}`；未获答复前不实施该修正、不降低静态门禁。armv7l 运行库输入和其他独立格继续。

   这里失败的是“其余相关库静态纳入”的依赖约束；额外的 `libbpf.so.1` 不等于已观测到 libstdc++ 被错误链入。二者不混写。

6. W2 整轮再现 cpio 的 Function not implemented；所选宿主失败系统调用跟踪未显示对应 ENOSYS/EPERM，根因仍未确定。新 18 RPM 通过单独核验，但整轮仍退出 1。新 libclang.a 的 SHA256 与旧输入不同，差异未归因，不替换 W1 输入、不宣称完全复现。详见 W2_REPORT.md。
