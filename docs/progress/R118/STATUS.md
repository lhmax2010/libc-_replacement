# 三类标准库设施差异验证

状态：进行中。仅记录观测结果，不把接口扫描候选视为已证实差异。

| 类型 | 状态 | 用时 | 产物 |
| --- | --- | --- | --- |
| std::string | PARTIAL；基础两架构及真实 Boost 入口已测，EWK 有缺口 | 原始 UTC 起止见 raw/*.time.json；汇总待包尾生成 | string/REPORT.md |
| std::vector | PARTIAL；两架构基础及真实 Boost 返回值对照，真实插件回调未观测 | 原始 UTC 起止见 raw/*.time.json；汇总待包尾生成 | vector/REPORT.md |
| std::function | 未开始 | — | — |

约束：平台源码只读；不推 Gerrit；并行度 1；每格 5 次；开发板连接中断即停报。原有 R115 两份脏文件不属于本任务，不修改或提交。

初始化只读命令：`pwd && git status --short --untracked-files=no && ls docs/progress/R118 && ls tmp/R118`，退出码 2；当时 R118 目录尚不存在，末项未执行。

板上普通测试与 Boost 对照上传文件已逐项删除，目录已核验不存在。EWK 尚未上传（诊断目录权限不足）。未宣称任何未完成的崩溃清理。源码代码均为新增测试/归档脚本，不修改 codes。

string 阶段提交：`b49cefd7ca0380e51f192a2e0cf55f7e871476b9`，普通推送成功且已核对远端引用。function 程序已准备、尚未执行；不得引用为实测。
