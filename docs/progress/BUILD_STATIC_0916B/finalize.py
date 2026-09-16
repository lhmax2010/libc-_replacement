import csv,datetime,hashlib,json,subprocess
from pathlib import Path
p=Path('progress/BUILD_STATIC_0916B'); now=datetime.datetime.now().astimezone()
rows=[]
for f in sorted(Path('tmp/NIGHT_0917/finish_validation/rpm/RPMS/aarch64').glob('*.rpm')):
    r=subprocess.run(['rpm','-qpl',str(f)],capture_output=True,text=True)
    assert r.returncode==0
    rows.append(dict(file=str(f),bytes=f.stat().st_size,sha256=hashlib.sha256(f.read_bytes()).hexdigest(),static_members=';'.join(x for x in r.stdout.splitlines() if x.endswith('.a'))))
assert len(rows)==4
assert any('/usr/lib64/libc++.a' in r['static_members'] for r in rows)
assert any('/usr/lib64/libc++abi.a' in r['static_members'] for r in rows)
with (p/'rpm_outputs.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t'); w.writeheader(); w.writerows(rows)
matrix=json.loads((p/'cancel_matrix.json').read_text())
assert len(matrix)==40 and sum(r['assertions_pass'] for r in matrix)==20
for mode in ['static','dynamic']:
    for wait in ['wait','system_clock','steady_clock','custom_clock']:
        cell=[r for r in matrix if r['mode']==mode and r['wait']==wait]
        assert len(cell)==5 and all(r['assertions_pass']==(mode=='dynamic') for r in cell)
sources={
 'tmp/NIGHT_0917/builds/bpftrace-x86_64-static/src/bpftrace':'242855842b9b07ed21d3bbb12e590601e7996b88dc29f335aa701fd3c72a5cc7',
 'tmp/NIGHT_0917/builds/bpftrace-aarch64-static/src/bpftrace':'0981193d70d2a768ecef078a1980b58185f9d5904469a6de21d6532521561836',
 'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace':'4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1'}
for name,h in sources.items(): assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==h,name
checks=[]
for repo,rev in [('tmp/BUILD_W1BC_0911/target-fetch','f203923a1508c9344f5fc6b17bd8822f011655c4'),('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push','f895f8c0373d224847fc7d3ecbeaac3bf926a1a1')]:
    for args in [['rev-parse','HEAD'],['status','--porcelain']]:
        command=['git','-C',repo,*args]; r=subprocess.run(command,capture_output=True,text=True)
        checks.append(dict(command=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
        assert r.returncode==0
        assert r.stdout.strip()==(rev if args[0]=='rev-parse' else '')
(p/'final_source_checks.json').write_text(json.dumps(checks,indent=2))
start=datetime.datetime.fromisoformat((p/'raw/001_resource_gate.started.txt').read_text().strip())
(p/'STATUS.md').write_text(f'''# bpftrace-static 收尾验证

总状态 **PARTIAL / STATIC_CANCELLATION_GATE=FAIL**。候选当前不能替换发布资产。
开始 {start.isoformat()}；本次汇总 {now.isoformat()}；墙钟 {(now-start).total_seconds():.0f} 秒。
各阶段命令起止时间在 raw/，未耗尽任何阶段时限；因静态行为失败及构建根准备受阻而收口。

| 阶段 | 结果 | 已完成 / 未完成 |
| --- | --- | --- |
| W1 功能等价 | PARTIAL | version/help 与 strip 已实测；info、probe、脚本被 root 检查阻止；内置函数实测清单未知；无 x86_64 原资产 |
| W2 完整 RPM | PARTIAL | aarch64 libcxx-runtimes 四包写出；LLVM 根复制 Permission denied 后停止；libclang RPM、三路径 bpftrace RPM、安装均未闭合 |
| W3 静态取消 | FAIL | 静态四格×5 全部 SIGABRT；动态四格×5 全通过；已得回溯、静态零初始化表与真实指令证据 |
| armv7l | NOT_OBSERVED_BOARD_OFFLINE | 未连接 |

W3 在 W2 的输入核查间隙进行，任何时刻只执行一个编译任务，不并行编译。
资源为 -j1、nice19、ionice3、ulimit -v 16148878 KiB（每进程 VA，不是聚合 RSS）。
RPM 构建监控启动及每500目标做 I/O 检查；实际记录在 runtime-aarch64-rpm.events.jsonl。
监控的小时状态在 HOURLY_STATUS.md；短于一小时的阶段由 raw/ 起止时间与本表交接。
候选和原资产 SHA256 复核未变；原源码树未改。无包仓/Gerrit 推送，无 Source1002 覆盖。
项目证据将仅提交到 codex/r5-ehabi-diagnosis。
''')
(p/'selfcheck.json').write_text(json.dumps(dict(time=now.isoformat(),rpm_count=4,cancel_rows=40,static_pass=0,dynamic_pass=20,original_candidates_unchanged=True,source_trees_clean=True,full_task_pass=False),indent=2))
print('Evidence selfcheck passed; task acceptance remains FAIL/PARTIAL, not PASS.')
