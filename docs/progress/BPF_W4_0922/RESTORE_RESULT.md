# 四份误改副本恢复结果

结论：4/4 恢复到 `BPF_W3R_0922/INCIDENT.md` 登记的改前 SHA256；四份事故文件均原地改名保留为 `bpftrace.objcopy-modified-0922`，未删除。原 RPM、板测 static ELF、平台源码不变。

## 逐份核验

路径前缀均为工作区 `tmp/`。完整 RPM 路径、RPM SHA、解出 SHA、改前/事故 SHA、复制后 SHA 见 `RESTORE_RECORDS.json`，不以缩写替代机器可核验记录。

| 顺序 | 原副本（相对 tmp） | RPM SHA256 | 解出 SHA256 = 改前 SHA256 = 恢复后 SHA256 | 结果 |
|---|---|---|---|---|
| 1 | BPF_W1_0921/payload-bpf-armv7l-libcxx/bpftrace/usr/bin/bpftrace | bfb90dfc923d0e9e45fcac9e2e6a611ebe7c63758b33a68a1d86ccf511d32d29 | 046115d23a6cda0bda5a38ca31bba8ed9f495fcdc7c2ae42f615d0d24dc7f3f4 | PASS |
| 2 | BPF_STATIC_0922/payload-bpf-armv7l-libcxx-verified/bpftrace/usr/bin/bpftrace | d6dffdeca9961d54cdeb5ceda4c1475b3ba232bedd2553604650202f5a4378bf | 44b1b04faa5e75e3a7274b651fc65d0333c0bebeeff4a0ac08d8058a74598b8b | PASS |
| 3 | BPF_W1_0921/payload-bpf-aarch64-libcxx/bpftrace/usr/bin/bpftrace | df465538e7635dfcf359a8ab759e8be7bed8f8ed949981938e2a3aec4f5c4fd0 | 7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec | PASS |
| 4 | BPF_STATIC_0922/payload-bpf-aarch64-libcxx-verified/bpftrace/usr/bin/bpftrace | 80720dbc175b45677d27d48b65dcc6c8c990f13c6109ac0e3e8287ac94059493 | 7093e7b92aacf750b0614ee0400ccbf98e983eef77e26bf38a5d293a8c9a68ec | PASS |

每份先核当前事故 SHA 与对应 RPM_INVENTORY 登记的 RPM SHA，再原地 `mv --no-clobber` 留存，`rpm2cpio` 到新的独立目录，`cpio` 仅解主包 ELF；核对通过才用 `cp -a --no-clobber` 回原路径。四组 rename/extract/copy 均退出 0。完整 argv、命令原文和退出码在 `restore_commands.json`，stdout/stderr 在同目录 `restore-*`；新解包副本在 `tmp/BPF_W4_0922/restore/{1,2,3,4}/`。

## 防止再次原位转换

修正后的脚本为本目录 `build_audit.py`，差异为 `BUILD_AUDIT_FIX.diff`。旧 W3R 脚本保留为事故历史，**不得再执行旧版本**。

修正命令明确提供第三个位置参数作为新输出 ELF：

```text
llvm-objcopy --dump-section .text=<新路径> <输入ELF> <新输出ELF>
```

新输出目录要求不存在，两个输出路径要求不存在且不等于输入；操作后断言输入字节及 inode/size/mtime/ctime 未变。今后所有提取/转换必须写到新路径，不能原位改写已有文件。

`FIX_SMOKE.json`：只对新建的一次性 `/usr/bin/true` 副本试验同形命令，退出 0，输入内容及上述元数据完全相同；没有重新转换四份已恢复主包，未运行构建。此测试证明修正命令不改该测试输入，不把它冒充全脚本重跑。

技术性错误申报：首次 `restore.py` 有一个多余右括号，raw/005 为 Python 语法错误，解析阶段尚未执行任何恢复操作；修正后 raw/006 才实际执行并完成 4/4。`cp -n` 的可移植性警告原样保留，命令退出均为 0。事故本身不因恢复而抹去。
