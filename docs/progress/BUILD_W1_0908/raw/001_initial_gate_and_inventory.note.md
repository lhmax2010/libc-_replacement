# 初始资源门禁与盘点记录

第一次盘点命令包含两处命令文本拼接错误：无效路径 `.issional`，以及 LLVM
仓库段被错误拼入 `printf` 参数。有效部分实际执行了 medium 资源门禁并得到：

```text
load1=3.61
mem_available_kib=23412196
disk_available_kib=147254672
gate_result=PASS
RESOURCE_GATE_EXIT=0
```

该命令整体退出码为 `0`。错误仅影响首次磁盘附加探针与 LLVM 身份输出；随后
用明确命令重跑，确认可用空间约 141 GiB，`codes/llvm` 为干净的
`tizen_base@5ed6c77278dfa7a470667cf1a137723d3c96fe60`。

第二次读取旧报告时，末尾的宽范围 `find tmp` 在执行工具等待时间内未给出
最终退出码，记为 `NOT_OBSERVED`；其输出不用于完整性判定。随后只在明确的
`tmp/BUILD_W1_0905/generated/` 路径重跑，退出码为 `0`。

