# 日志器建立及复用脚本时的只读命令

以下原文与退出码补记；原文件/脚本在仓中，未将后续内容冒充当时输出。

| 命令原文 | 退出码 |
|---|---:|
| `pwd` | 0 |
| `rg --files -g AGENTS.md -g 'CONTINUE.md' -g '*REFERENCE_INPUTS.json' -g 'resource_gate.sh' -g 'LINE_STATUS.md' -g '!codes/**' -g '!tmp/**'` | 0 |
| `sed -n '1,200p' progress/BPF_W1_0921/cell_commands/bpf-armv7l-libcxx.json` | 0 |
| `sed -n '1,240p' tmp/BPF_W1_0921/specs/bpftrace.spec` | 0 |
| `sed -n '1,180p' docs/progress/BPF_W1_0921/CONTINUE.md` | 0 |
| `sed -n '1,180p' docs/progress/BPF_W1_0921/W3_REFERENCE_INPUTS.json` | 0 |
| `ls -ld progress/BPF_STATIC_0922 tmp/BPF_STATIC_0922 docs/progress/BPF_STATIC_0922` | 2（新目录尚不存在） |
| `sed -n '1,160p' progress/BPF_LIBBPF_0922/run.py` | 0 |
| `sed -n '1,260p' progress/BPF_LIBBPF_0922/guard.py` | 0 |
| `sed -n '1,220p' progress/BPF_W1_0921/capture_failure.py` | 0 |
| `sed -n '1,260p' progress/BPF_W1_0921/verify.py` | 0 |
| `sed -n '1,220p' progress/BPF_LIBBPF_0922/publish.py` | 0 |
| `sed -n '1,220p' progress/BPF_LIBBPF_0922/check_staged.py` | 0 |

实际新脚本均经 apply_patch 建立；命令执行记录见raw与cells。这里不包含任何凭据。

收尾恢复上下文时另有以下只读命令未经过日志器，现补记实际原文及退出码（不补造原始输出）：

```sh
git status --short; sed -n '1,220p' progress/BPF_STATIC_0922/FINAL_RESULT.md
# exit 0
sed -n '1,220p' progress/BPF_STATIC_0922/publish.py; sed -n '1,220p' progress/BPF_STATIC_0922/check_staged.py; sed -n '1,150p' progress/BPF_STATIC_0922/run.py
# exit 0
rg -n '完整事实|若已生成|详细功能差异' progress/BPF_STATIC_0922/report.py; sed -n '1,180p' progress/BPF_STATIC_0922/BOOTSTRAP_READS.md; sed -n '1,100p' progress/BPF_STATIC_0922/README.md
# exit 0
```
