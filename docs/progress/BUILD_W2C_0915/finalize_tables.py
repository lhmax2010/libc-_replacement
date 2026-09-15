"""Summarize completed checks without approving the unresolved prebuilt scope."""
import csv
import json
from pathlib import Path

p=Path('progress/BUILD_W2C_0915')
bcc=json.loads((p/'bcc_gate.json').read_text())
assert bcc['status']=='SIX_CELLS_PASS'
rows=[]
for item in bcc['matrix']:
    rows.append(dict(package='bcc-tools',arch=item['arch'],mode=item['mode'],build='PASS',
        compiler='Clang 22.1.8' if item['mode']=='libcxx' else 'GNU 14.2.0',
        standard_library='libc++.so.1' if item['mode']=='libcxx' else 'libstdc++.so.6',
        boundary='PASS',checks=item['repeated_checks'],reused=item['reused'],
        scope='runtime ELF; six-cell package gate closed',evidence=';'.join(item['evidence'])))
summaries=[]
for arch in ['armv7l','aarch64']:
    for mode in ['libcxx','gcc']:
        path=p/f'checks/bpftrace_{arch}_{mode}/summary.json'
        item=json.loads(path.read_text())
        assert item['status']=='SOURCE_PROGRAMS_AND_PROVIDERS_PASS'
        assert item['prebuilt']['byte_identical']
        summaries.append(item)
        rows.append(dict(package='bpftrace',arch=arch,mode=mode,build='PASS',
            compiler=item['compiler'],standard_library='libc++.so.1' if mode=='libcxx' else 'libstdc++.so.6',
            boundary='PASS',checks=1,reused=False,
            scope='source-built bpftrace and bpftrace-aotrt; prebuilt static scope pending',evidence=str(path)))
with (p/'MATRIX.tsv').open('w') as stream:
    writer=csv.DictWriter(stream,delimiter='\t',fieldnames=list(rows[0]))
    writer.writeheader(); writer.writerows(rows)
md=['# 构建及产物核查矩阵','','| 包 | 架构 | 路径 | 编译器 | 构建 | 标准库依赖 | 核查 | 来源 |',
    '| --- | --- | --- | --- | --- | --- | --- | --- |']
for row in rows:
    md.append(f"| {row['package']} | {row['arch']} | {row['mode']} | {row['compiler']} | PASS | {row['standard_library']} | {row['checks']}次 | {'沿用' if row['reused'] else '本轮'} |")
md += ['', 'bcc-tools 六格全部闭合，已推送。bpftrace 四格中的 PASS 指完整构建、',
       '两个源码构建程序及实际 LLVM/BCC 提供方的标准库检查；不包含对预编译',
       'static 子包内嵌标准库的确认，不能据此将整包推送门禁写为已闭合。',
       'bpftrace x86_64 不在 ExclusiveArch，未列入矩阵，不算失败格。',
       'bcc-tools armv7l 每格的五次为同一批 RPM 的重复核查，不是五次独立构建。',
       '本轮未使用开发板，未进行 bcc/bpftrace 内核跟踪功能实测。','']
(p/'MATRIX.md').write_text('\n'.join(md))

with (p/'PACKAGE_STATUS.tsv').open() as stream:
    packages=list(csv.DictReader(stream,delimiter='\t'))
for row in packages:
    if row['package']=='bpftrace':
        row['validation']='armv7l/aarch64 × libc++/GCC 四格完整构建、源码程序及LLVM/BCC提供方检查通过；static预编译子包内嵌标准库NOT_OBSERVED；x86_64不适用'
        row['current_status']='PARTIAL：static子包推送门禁范围待人工确认；未提交、未推送'
assert len(packages)==11 and sum(row['current_status']=='已完成并推送' for row in packages)==10
with (p/'PACKAGE_STATUS.tsv').open('w') as stream:
    writer=csv.DictWriter(stream,delimiter='\t',fieldnames=list(packages[0]))
    writer.writeheader(); writer.writerows(packages)
table=['# 11 包实施状态','','以下提交为钉住的实施与验证对象，不冒充本轮重新查询的所有远端 HEAD。','',
       '| 包 | 分支 | 实施提交 | 验证 | 当前状态 |','| --- | --- | --- | --- | --- |']
for row in packages:
    table.append('| '+' | '.join(row[key] for key in ['package','branch','adaptation_commit','validation','current_status'])+' |')
(p/'PACKAGE_STATUS.md').write_text('\n'.join(table)+'\n')
(p/'bpftrace_source_matrix.json').write_text(json.dumps(dict(source_cells=4,
    source_cells_status='PASS',prebuilt_scope_status='WAITING_FOR_HUMAN_CONFIRMATION',
    package_pushed=False,cells=summaries),ensure_ascii=False,indent=2)+'\n')
print('PARTIAL: bcc-tools six cells closed/pushed; bpftrace four source cells passed; prebuilt scope not approved')
