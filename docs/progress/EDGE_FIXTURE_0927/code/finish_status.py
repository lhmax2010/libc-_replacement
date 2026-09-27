from common import *
a=json.loads((OUT/'AUDIT.json').read_text());assert a['result']=='PASS'
c=json.loads((OUT/'COUNTS.json').read_text());assert c['baseline_edges']==16
start=datetime.datetime.fromisoformat(a['task_first_record_utc']);end=datetime.datetime.now(datetime.timezone.utc)
minutes=(end-start).total_seconds()/60
text=f'''# 夜间夹具与 GNU 基线：收尾状态

**PARTIAL。** 分析/小夹具工作结束，停止等待审阅；提交推送结果与远端 SHA 见 DELIVERY.md。

| 阶段 | 状态 | 产物 | 备注 |
| --- | --- | --- | --- |
| 开场/输入冻结/资源 | 完成 | INPUTS.json、EDGES.tsv、NEXT_STAGE.tsv、raw/ | 已读备案并复述十项；23 边唯一来源保持不变 |
| 当前 GNU 包获取 | 完成 | SNAPSHOTS.json、RPMS.json、RPM_PROVENANCE.tsv | 262 个主输入 RPM；另四个测试数据包独立解包，不安装、不构建 |
| 公开声明/前置/夹具 | PARTIAL | DECLARATIONS.md、DETAILS.json、fixtures/ | 19 个边夹具编译；4 条未构造运行夹具，不用模拟件 |
| GNU 对照 | 16/23 条有效 | SELECTED_RUNS.json、RESULTS.tsv、runs/、raw/ | 16×5=80 有效轮次；最终采用 95 次尝试；其余 15 次不算通过 |
| 七项未完成 | NOT_AVAILABLE | FINAL_RESULT.md、DETAILS.json、DATA_FOLLOWUP.md | 边 7/8/10/11/12/16/18，原因逐项列明 |
| libc++ 交接/重放 | 清单与参数入口完成，运行未测 | LIBCXX_INPUTS.tsv、README.md、code/ | 不生成或推断 libc++ 结果；待输入、同侧控制与授权 |
| 自检/备案 | 完成 | AUDIT.json、BINDING_AUDIT.json、SHA256SUMS、docs/LINE_STATUS*.md | 精确绑定目标、源码 SHA、输入一致、串行和资源检查 |

首条操作记录：{start.isoformat()}；收尾状态生成：{end.isoformat()}。
本次墙钟耗时约 {minutes:.1f} 分钟（含阅读、下载、失败纠正、补查和汇编，不等于纯测试运行时间）；精确各命令起点/时长见 raw/*.time.json。
硬截止：2026-09-28 08:30 +08:00；本次未延长截止。

资源：x86_64 本机、串行、nice 19、ionice 3、light 闸门，RLIMIT_AS={MEM*30//100} 字节（实际内存 30%）。截至自检闸门失败 0 次；失败时等待十分钟、连续六次保存停止的规则写入执行器。小夹具编译/运行时间区间已核无重叠。夹具内部通知/争用线程不等于并行调度。Git 保存采用普通 I/O 优先级。

不用开发板、不构建平台包、不修改平台源码/配置、不推 Gerrit/包仓、无 force。原有两份其他任务日志修改不纳入本次提交；其余无关未跟踪材料同样不纳入。下载库和二进制保留在本任务隔离 tmp 目录供复核，不进入 git；源码、运行记录与 SHA 入库。

全量 raw/runs 保留失败尝试。最终证据选择按每条边最后一组，不用旧成功掩盖新失败。公有 API 夹具仅证明被测输入及生命周期断言，未声称内部全部无泄漏、原应用端到端通过或跨架构兼容。
'''
(OUT/'STATUS.md').write_text(text)
print('status_minutes',round(minutes,1))
