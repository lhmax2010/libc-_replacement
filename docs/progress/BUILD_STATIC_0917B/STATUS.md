# 开工核查（首次记录，后续状态见 README.md）

本轮只完成资源门禁与输入的只读核查，未进入构建、RPM 安装或板上测试。未调用 sudo，未修改 codes/、候选 spec 或 Source1002；未提交、未推送。

## 门禁与资源

`tools/resource_gate.sh --level medium` 实际退出码 0，PASS；load1=0.60，MemAvailable=8424972 KiB，磁盘可用 267728880 KiB。记录器固定 PATH，使用 nice 19、ionice class 3、ulimit -v 16148878 KiB（机器 MemTotal 的 50%；这是虚拟地址空间限制，不是 RSS/cgroup 限制）。尚未启动构建。

## 需要人工确认的输入冲突

1. Source1002 的 tar SHA256 为 `3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123`。实际只有 arm 与 aarch64 两个二进制，没有 x86_64 原资产。这个 SHA256 是 tar 的哈希，不是内部二进制的哈希。
2. 原 spec 和 tmp 候选 spec 第 17 行均为 `ExclusiveArch: armv7l aarch64`。因此本任务要求的 x86_64 完整三路径 RPM 不是现有打包支持范围，且 GCC/undefined 路径没有可供逐字节对照的 x86_64 原资产。
3. 两份原资产均无 libgcc_s.so.1 的 DT_NEEDED；nm 报 no symbols，动态符号表未匹配到指定展开函数的定义。strings 均出现 `_Unwind_GetGR`，但字符串不等于函数定义，不能据此确定展开器来源或重建原链接参数。原资产实际取消行为仍为 NOT_OBSERVED，不能用另一份探针的结果替代。
4. 当前没有已确认的 PC sudo 安全输入通道。未寻找或读取任何密码文件，未尝试 sudo。密码不得发到聊天、文件、命令行或日志；复制步骤需人工在本机提供交互式 stdin，或由人工执行经核对的复制和 chown 命令。

## 待答复

- 是否将三路径完整 RPM 限定为现有支持的 armv7l/aarch64，x86_64 保留独立候选与探针验证？若仍要求 x86_64 完整 RPM，需要确认允许扩展 tmp 候选 spec 的架构范围，以及提供真正的 x86_64 原资产或明确该架构回退路径的验收口径。
- sudo 复制步骤采用哪种安全方式：人工本机交互输入，还是人工执行已核对的复制/chown？本轮暂不调用 sudo。

## 证据

- raw/001_resource_gate.*：门禁原命令、输出、退出码及资源上限。
- raw/002_input_identity.*：分支、HEAD、Source1002 哈希和完整 tar 清单、两份 spec 的文件行号。
- raw/003_original_elf.*：两份原资产的哈希、readelf、nm、strings；各检索退出码已记录，rg 的 1 表示无匹配，不是把核查结果补为通过。

原 arm 二进制 SHA256：`ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b`。
原 aarch64 二进制 SHA256：`4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1`。

本轮自行判断仅限：发现打包范围与任务所需输入不一致后，不擅自扩架构、不把自建候选改称原资产；不足以还原的链接参数不作猜测。板子连通性、板上残留、内核能力均尚未检查（NOT_OBSERVED），不把用户告知的恢复状态写成此次实测。
