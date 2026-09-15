"""Produce explicit phase outcomes and check evidence without changing prior task records."""
import csv
import datetime as dt
import hashlib
import json
from pathlib import Path
import subprocess

p=Path('progress/BUILD_NIGHT_0916')
inventory=json.loads((p/'static_archive_inventory.json').read_text())
repo=json.loads((p/'repository_static_packages.json').read_text())
abi=json.loads((p/'available_archive_abi.json').read_text())
alternate=json.loads((p/'static_packages_v2.json').read_text())
rows=list(csv.DictReader((p/'PACKAGE_STATUS.tsv').open(),delimiter='\t'))
assert len(rows)==11 and next(x for x in rows if x['package']=='bpftrace')['adaptation_commit']=='f895f8c0373d224847fc7d3ecbeaac3bf926a1a1'
assert len(repo['records'])==15 and all(x['status']=='RPM_PAYLOAD_LIST_VERIFIED' for x in repo['records'])
assert len(abi['records'])==12 and all(x['std1_lines']>0 and x['stdcxx11_lines']==0 for x in abi['records'])
for arch in ['armv7l','aarch64','x86_64']:
 for library in ['libc++','libc++abi','Clang C API']:
  row=next(x for x in inventory['records'] if x['arch']==arch and x['library']==library)
  assert row['count']==0
alternatives=[x for x in alternate['members'] if 'stdcxx11_lines' in x]
assert len(alternatives)==2 and sorted(x['stdcxx11_lines'] for x in alternatives)==[30,31]
assert (p/'raw/031_original_aarch64_info.exitcode').read_text().strip()=='1'
error=(p/'raw/031_original_aarch64_info.stderr.txt').read_text()+(p/'raw/031_original_aarch64_info.stdout.txt').read_text()
assert 'only supports running as the root user' in error
assert (p/'raw/036_original_aarch64_version.exitcode').read_text().strip()=='0'
assert (p/'raw/037_original_aarch64_help.exitcode').read_text().strip()=='0'
assert hashlib.sha256(Path('codes/R104/bpftrace/packaging/bpftrace.spec').read_bytes()).hexdigest()=='f471f98b0d48dd769de286fbeaa8d7292230fbf316c95b06e03e2b631a86cdd3'

def stamp(label,suffix):
 return dt.datetime.fromisoformat((p/'raw'/f'{label}.{suffix}.txt').read_text().strip())
start1=stamp('001_resource_gate','started')
end1=stamp('021_push_w1','finished')
start2=stamp('022_w2_resource_gate','started')
now=dt.datetime.now().astimezone()
result=dict(W1='COMPLETE_SOURCE_ADAPTATION_PUSHED_STATIC_EXCEPTED',W2='EVALUATED_WITH_BLOCKERS',
 W3='NOT_STARTED',W1_seconds=(end1-start1).total_seconds(),W2_seconds=(now-start2).total_seconds(),
 W1_start=start1.isoformat(),W1_finish=end1.isoformat(),W2_start=start2.isoformat(),W2_evaluation_finish=now.isoformat(),
 timing_scope='W1 measured from resource gate to project evidence push; preliminary reading before gate is not included. W2 evaluated at report finalization, archive/push follows.',
 blocked_inputs=['matching libclang.a','libc++.a','libc++abi.a'],
 additional_gaps=['complete static transitive dependency closure','privileged original --info and probe baseline'],
 new_builds=0,static_spec_changes=0,static_replacement_pushed=False,
 armv7l_board='NOT_OBSERVED_BOARD_OFFLINE')
(p/'w2_result.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')

# Provide an updated availability view without changing the first root-only inventory.
summary=[]
mapping={'libbpf':'libbpf-static','zlib':'zlib-devel-static','LZMA':'liblzma-static','libffi':'libffi-devel-static'}
for entry in inventory['records']:
 row={k:entry[k] for k in ['arch','library','role','count']}
 if entry['count']:
  row.update(availability='PRESENT_IN_ROOT',evidence='static_archive_inventory.json')
 elif entry['library'] in ['BCC','BCC runtime','BCC loader']:
  row.update(availability='AVAILABLE_LOCAL_LIBCXX_RPM',evidence='static_packages_v2.json; available_archive_abi.json')
 elif entry['library'] in mapping:
  found=next(x for x in repo['records'] if x['arch']==entry['arch'] and x['package']==mapping[entry['library']])
  assert found['archives']
  row.update(availability='AVAILABLE_REFERENCE_RPM',evidence=found['url'])
 elif entry['library'] in ['Clang C API','libc++','libc++abi']:
  row.update(availability='MATCHING_STATIC_INPUT_NOT_AVAILABLE',evidence='W2_REPORT.md sections 3–4')
 else:
  row.update(availability='NOT_OBSERVED_IN_ROOT_PLATFORM_EXHAUSTIVENESS_NOT_CLAIMED',evidence='W2_REPORT.md dependency scope')
 summary.append(row)
with (p/'static_availability_summary.tsv').open('w') as f:
 w=csv.DictWriter(f,delimiter='\t',fieldnames=summary[0].keys());w.writeheader();w.writerows(summary)

status=f'''# 夜间任务状态：源码适配已推送，static 构建未启动

| 任务 | 状态 | 用时 | 产物路径 | 备注 |
| --- | --- | --- | --- | --- |
| W1 推送 bpftrace | COMPLETE | {result['W1_seconds']:.0f} 秒（资源门禁至项目材料推送；更早阅读不计） | W1_REPORT.md、PACKAGE_STATUS.*、w1_result.json | f895f8c 已推；源码适配 11/11，static 单列 |
| W2 static 可行性评估 | EVALUATED_WITH_BLOCKERS | {result['W2_seconds']:.0f} 秒（至评估收口） | W2_REPORT.md、static_availability_summary.tsv | 匹配的核心静态输入缺失；完整闭包与可靠估时未取得 |
| W3 static 构建 | NOT_STARTED | 0 | W3_NOT_STARTED.md | 按 W2 第 11.b 条，不启动、不改 spec、不推候选 |

时间戳以 w2_result.json 与 raw 为准，归档/推送在评估收口后继续进行。
未达到任何阶段时限。全任务不足一小时，没有虚构小时心跳或构建目标进度。

资源：并行度 1；nice=19；ionice class=3；ulimit -v 16148878 KiB，为实测
MemTotal 32297756 KiB 的 50%，约 15.40 GiB/进程虚拟地址空间，不是累计 RSS。
两次 medium 门禁均 0；开工磁盘约 264 GiB；4.5 MiB 读写/fsync 探测约 0.105 秒。
本轮没有构建，所以“每 500 目标”与 I/O 暂停机制未触发，不能声称暂停流程已实测。
只清理了当次探测新建的 4.5 MiB 临时文件及其空目录；未删除任何既有资料。
开发板离线，armv7l 板上项目均 NOT_OBSERVED_BOARD_OFFLINE；无连接或等待。

阶段记录：

- {start1.isoformat()}：W1 资源门禁开始，之后四格/11 spec 复核通过。
- {end1.isoformat()}：W1 项目材料推送完成；包的推送时间在 w1_result.json。
- {start2.isoformat()}：W2 资源门禁开始。
- {now.isoformat()}：W2 评估收口，W3 NOT_STARTED；随后提交推送 W2/W3 记录。

需后续补齐：匹配版本与 libc++ ABI 的 libclang.a、libc++.a、libc++abi.a，
传递静态依赖闭包，以及有权限环境下原资产的 --info/实际 probe 基准。
这不是永久不可构建结论，也不是新静态构建失败；本轮没有启动新构建。
'''
(p/'STATUS.md').write_text(status)
print(json.dumps(result,ensure_ascii=False,indent=2))
