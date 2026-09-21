# W3 输入交接（仅清单，不执行上板）

必须先由人工审核本轮未闭合项。以下路径只是可定位的本轮产物，不代表已获上板或发布许可。

- libcxx: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-libcxx/RPMS/armv7l/bpftrace-0.24.2-1.armv7l.rpm`；SHA256 `bfb90dfc923d0e9e45fcac9e2e6a611ebe7c63758b33a68a1d86ccf511d32d29`；1524552 字节。
- libcxx: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-libcxx/RPMS/armv7l/bpftrace-common-0.24.2-1.armv7l.rpm`；SHA256 `4d10b2895a53eaec8472025769bf707015a118450ab2de97390f5ccba740b1c2`；82924 字节。
- libcxx: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-libcxx/RPMS/armv7l/bpftrace-static-0.24.2-1.armv7l.rpm`；SHA256 `2015c3351dad45fd36e4f78412778c52d2db6e301c8bf254e179f788b72fadac`；22328608 字节。
- gcc: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-gcc/RPMS/armv7l/bpftrace-0.24.2-1.armv7l.rpm`；SHA256 `8b3297d1fe41e06a10d1ef9cd576c714282a89e9806249b1eefdc3c7347aaf99`；1677725 字节。
- gcc: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-gcc/RPMS/armv7l/bpftrace-common-0.24.2-1.armv7l.rpm`；SHA256 `3507fb81ea6b8bbf75623a736e3586c325de4135e2114bfb872ba8d229ccd95f`；82925 字节。
- gcc: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-gcc/RPMS/armv7l/bpftrace-static-0.24.2-1.armv7l.rpm`；SHA256 `2c73cf784adadeb81668cdb19a5847d2077c26615bba2f8ac1c7c1809c7abd0c`；35226353 字节。
- undefined: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-undefined/RPMS/armv7l/bpftrace-0.24.2-1.armv7l.rpm`；SHA256 `48012c531100bb23f7904f8bafce945054e39a7d09369e2e0d870d09b53b0784`；1678497 字节。
- undefined: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-undefined/RPMS/armv7l/bpftrace-common-0.24.2-1.armv7l.rpm`；SHA256 `1f3b9509a296eac55dc8361e16e7caf66ccae5912624e062ec5d350cefa6f1fc`；82933 字节。
- undefined: `/home/toolchain/development/libc++_replacement/tmp/BPF_W1_0921/rpm-bpf-armv7l-undefined/RPMS/armv7l/bpftrace-static-0.24.2-1.armv7l.rpm`；SHA256 `76832a92c379fd420c97acd3d7d3ce149ec0fa5b5cb7d88a94c031ffbc337f4d`；35227093 字节。

还需：同版本原资产对照、共享/静态展开器取消探针、板端依赖与内核/BPF 环境记录、传输前后 SHA256 对照；这些验证不在本轮执行。

## 已有参考输入与部署前提

`W3_REFERENCE_INPUTS.json` 已核对本地探针源码、共享/静态 libgcc 两份 ARM 探针与原 ARM static 二进制的完整路径和 SHA256；仅确认本地身份，没有运行探针或上板。

前轮 BUILD_WEEKEND_0918/W3_REPORT.md 记录正常 RPM 安装预检缺 `libbcc_bpf.so.0`、`libclang-cpp.so.22.1`、`libclang.so.22.1` 及相关版本符号。W3 需先确定与本轮主包标准库匹配的运行库部署清单，不能用 `--nodeps` 略过。该缺口为前轮观测，本轮板端现状 NOT_OBSERVED。

前轮 ARM 共享/静态 libgcc 取消对照各 20/20 通过，与 x86_64 静态组不同；这不是本轮实测，也不能预设下一轮静态对照必崩。后续应保留原探针和原断言，报告实际结果。

本轮 RPM 的实际依赖声明见 `RPM_REQUIRES.tsv`（生成后使用），逐项查询命令及退出码见 `RPM_REQUIRES_COMMANDS.json`。这是 RPM 声明，不表示板端已经满足；本轮未制备或安装一套板端依赖补包集合。板端是否缺依赖须在 W3 正常安装预检中再核实。
