# 只读纪律违反：llvm-objcopy改写既有解包副本

## 事实与责任

本轮 `build_audit.py` 为取得.text，执行了以下形态：

```sh
/usr/bin/llvm-objcopy --dump-section .text=<本轮新输出> <已有主包ELF>
```

遗漏独立的输出ELF参数，工具原位改写了输入。**这是我的命令错误，违反第二部分只读要求**，不能记为正常只读检查。准确四条命令及退出码在 `build-audit/commands.json`；本地发现于 `raw/030_input_identity_after.*` 的末次SHA核验（退出1）。发现后已告知用户，停止继续使用受影响副本，未擅自恢复；仅追加原RPM完整性及内存只读解析核对。

## 影响范围（均为主包，不是板测static）

工作区前缀 `/home/toolchain/development/libc++_replacement/`：

| 受影响路径 | 改前SHA256 | 改后SHA256 |
|---|---|---|
| tmp/BPF_W1_0921/payload-bpf-armv7l-libcxx/bpftrace/usr/bin/bpftrace | 046115d23a6cda0bda5a38ca31bba8ed9f495fcdc7c2ae42f615d0d24dc7f3f4 | 97ff881269249f489e95f8049d39bba11329197cc26f6bdf4e755e5f9ef1cde7 |
| tmp/BPF_STATIC_0922/payload-bpf-armv7l-libcxx-verified/bpftrace/usr/bin/bpftrace | 44b1b04faa5e75e3a7274b651fc65d0333c0bebeeff4a0ac08d8058a74598b8b | 376b92e60cd88566eb55a13b8063a0297e82a98bfe840a207cf91f8ff86810da |
| tmp/BPF_W1_0921/payload-bpf-aarch64-libcxx/bpftrace/usr/bin/bpftrace | 7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec | c9d09f266f0904ac0f2b4519f649da7bc6ac292c20a7edfc052f42b172679529 |
| tmp/BPF_STATIC_0922/payload-bpf-aarch64-libcxx-verified/bpftrace/usr/bin/bpftrace | 7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec | c9d09f266f0904ac0f2b4519f649da7bc6ac292c20a7edfc052f42b172679529 |

原RPM、spec、平台源码未作为该命令输出。本轮板测从static RPM在板端新解出的ELF，不在此四份之内；其身份与测试记录独立保留。

## 只读取证，不修复

`rpm_readonly_check.py` 对四份原main RPM重新算SHA，均与此前验收表一致。随后用rpm2cpio的stdout在内存解析cpio/ELF，**没有写回任何输入，也没有替换解包副本**。复核四个原始ELF SHA和.text SHA，均与误改前读取/首次section导出的值相符，故报告中“ARM两轮不同、aarch64两轮相同”的数据仍得到原RPM支撑。具体为 `RPM_READONLY_CHECK.json`、raw/031。

这份核查**不抵消违规**，只划清数据影响范围；不得把旧路径下目前已改写的文件继续当作原始ELF。本轮不归因字节重排细节，不以“功能可能不变”为理由隐去改写。

## 待人工裁决

是否允许从已验证未变的原RPM恢复这四份解包副本？恢复方式应当是先解到新的独立目录、核对原SHA，再按批准范围恢复。**本轮未实施。** 其余任务材料可审阅，但只读合规状态为VIOLATION。
