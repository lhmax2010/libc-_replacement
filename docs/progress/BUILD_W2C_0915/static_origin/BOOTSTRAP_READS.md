# 包装器建立前的只读定位记录

以下为工具调用记录恢复的原命令与退出码，不冒充新包装器自动生成。
后续正式核查使用本目录run_logged.sh，各子进程记录在JSONL中。

```text
pwd; rg --files -g AGENTS.md -g '!tmp/**' -g '!codes/**' -g '!docs/**' -g '!progress/**'; git status --short --untracked-files=no; ls codes/qemu-accel
整体退出码：0（早期组合命令未逐子命令记录，不补造各子命令退出码）

sed -n '1,140p' progress/BUILD_W2C_0915/run_logged.sh; rg --files codes/qemu-accel; rg --files docs/progress/R95 docs/progress/R98 docs/progress/R100 | rg -i 'image|manifest|report|input'
整体退出码：0（同上）
```

raw/002、004等早期组合核查保留整体退出码；逐ref搜索及镜像/资产校验
随后由inspect_evidence.py重新执行并逐命令记录退出码，不依赖组合命令的
末条退出码判定全部成功。RPM查询首试错误、修正后成功分别保留。
读取本目录已有stdout/json的展示命令不改变输入或分析结果；原始核查结果
已存入对应记录。未手工补记不存在的成功退出码。
