#!/usr/bin/env python3
"""把重复发布版本归并为待确认的源码包族；仍保留全部版本身份。"""
import collections
import csv
import pathlib

OUT=pathlib.Path('docs/progress/P7_0909')
def read(path):return list(csv.DictReader(path.open(),delimiter='\t'))
groups=collections.defaultdict(list)
for r in read(OUT/'tables/proposed_source_groups_named.tsv'):groups[r['source_name']].append(r)
managed=[r for r in read(OUT/'tables/logical_packages.tsv') if r['sourcerpm'].startswith('csapi-tizenfx-')]
assert len({r['sourcerpm'] for r in managed})==1
groups['csapi-tizenfx']=[dict(source_rpm=managed[0]['sourcerpm'],binary_names=';'.join(sorted(r['name'] for r in managed)),public_required_cpp_headers='0',nonprivate_cpp_headers='0')]
rows=[]
for name,variants in sorted(groups.items()):
    if name.startswith('dali2') or name in {'csapi-tizenfx','libscl-ui-nui'}:
        group='DALI_NUI';reason='用户指定；分开原生 public/devel/integration、C# 程序集及绑定层，确认真实应用入口'
    elif name in {'chromium-efl','lightweight-web-engine','webapi-plugins'}:
        group='WEB';reason='用户指定；区分 EWK、Node/V8、LWE 原生层与 Web 插件/脚本侧'
    elif name in {'glib2','harfbuzz','icu','vulkan-headers'}:
        group='COMMON_PUBLIC_SDK';reason='public 配方有 C++ 条件声明或便利封装，先区分纯头包装与真实跨库接口'
    else:
        group='NATIVE_APP_MIDDLEWARE';reason='public 配方必要路径有 C++ 信号，需按头文件/接口组核验，不能按 CAPI/服务包名判纯 C'
    rows.append(dict(group=group,source_name=name,source_rpm_records=';'.join(sorted({r['source_rpm'] for r in variants})),binary_names=';'.join(sorted(set(';'.join(r['binary_names'] for r in variants).split(';')))),source_versions=len(variants),reason=reason,status='PROPOSED_NOT_STARTED'))
with (OUT/'tables/STAGE2_RECOMMENDED.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
print('recommended_source_families',len(rows),'source_rpm_versions',sum(r['source_versions'] for r in rows),'groups',dict(collections.Counter(r['group'] for r in rows)))
