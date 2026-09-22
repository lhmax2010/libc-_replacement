# W3 上板输入更新（未上板）

本轮两个最终 ELF 已闭合 libbpf 动态依赖；旧 RPM **不含**这些新文件。不能直接把上轮 RPM 当成本轮候选安装。

## armv7l

- 文件：`/home/toolchain/development/libc++_replacement/tmp/BPF_LIBBPF_0922/outputs/armv7l/bpftrace-static`
- 字节：90870376
- SHA256：`a68f5c8c44051769e1e4718935fc3c5e9e147c5ea56e1a91b86ab902414346d5`
- DT_NEEDED：`libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux.so.3`
- 原件对照：`/home/toolchain/development/libc++_replacement/tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/arm/usr/bin/bpftrace`
- 原件SHA256：`ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b`
- CMake改动：`armv7l-static-libbpf.patch`；完整实际链接命令在 `verify-armv7l/link-after.txt`。

## aarch64

- 文件：`/home/toolchain/development/libc++_replacement/tmp/BPF_LIBBPF_0922/outputs/aarch64/bpftrace-static`
- 字节：136139424
- SHA256：`26ecab8874f842db053a227a92b93e000fcf818ded8cd957daa0e716f36e0079`
- DT_NEEDED：`libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux-aarch64.so.1`
- 原件对照：`/home/toolchain/development/libc++_replacement/tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace`
- 原件SHA256：`4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1`
- CMake改动：`aarch64-static-libbpf.patch`；完整实际链接命令在 `verify-aarch64/link-after.txt`。

## 仍需准备/验证

- 若采用 RPM 安装路径：先将本轮 ELF 纳入新的 static RPM，保留此前主包/common，核查安装脚本和传输前后 SHA；新 RPM 本轮为 NOT_OBSERVED。已向人工询问是否在本轮追加 static-only 写包，尚未收到答复。
- 原资产、旧 RPM 与旧板端依赖清单仍引用 `docs/progress/BPF_W1_0921/W3_INPUTS.md`，不得与新 ELF 身份混用。主包仍可能需要动态 libbpf，这是原有主包设计，不是本轮 static 失败。
- ARM 物理板取消、--version/--info/probe/脚本原件对照、安装/卸载清理：本轮 NOT_OBSERVED，待 W3 授权。x86_64 probe 20/20 不能代替 ARM 板测。
- 传输前再次核对 CANDIDATES.tsv 的 SHA；不得覆盖 Source1002，不推包仓。
