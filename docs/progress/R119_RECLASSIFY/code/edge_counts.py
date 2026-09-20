#!/usr/bin/env python3
"""从未改动的阳性证据投影三个去重层级，不把候选名当独立边。"""
from common import *
from collections import defaultdict
start=time.time(); gate('edge_gate'); mappings=[]; proofs={}
for item in inputs():
    row=item['row'];rank=item['rank']
    if row['result']!='CROSS_PACKAGE':continue
    for rel in row['evidence'].split(';'):
        if '/positive/' not in rel or not rel.endswith('.json') or rel.endswith('_include_chain.json'):continue
        p=(item['base']/rel).resolve();obj=json.loads(p.read_text());proofs[str(p.relative_to(ROOT))]=sha(p)
        for m in obj['elf_matches']:
            c=m['consumer']; v=m['provider']; ci=c['inventory'];vi=v['inventory'];sym=m['symbol'];raw=sym['raw_symbol']
            if isinstance(raw,str):raw=[raw]
            assert ci['arch']==vi['arch']
            for symbol in raw:
                mappings.append(dict(rank=rank,candidate=row['candidate'],consumer_package=row['consumer_package'],provider_package=row['provider_package'],consumer_sourcerpm=ci['sourcerpm'],provider_sourcerpm=vi['sourcerpm'],arch=ci['arch'],consumer_elf=ci['path'],provider_elf=vi['path'],consumer_sha256=c['sha256'],provider_sha256=v['sha256'],raw_symbol=symbol,demangled=sym['demangled'],shape=row['shape'],ownership=row['ownership'],proof=str(p.relative_to(ROOT))))
# 同一份证据在一个候选中重复提及时去重。
mappings=list({(r['rank'],r['arch'],r['consumer_sha256'],r['provider_sha256'],r['raw_symbol']):r for r in mappings}.values())
assert len({r['candidate'] for r in mappings})==44
tsv(OUT/'CROSS_CANDIDATE_EDGE_MAP.tsv',mappings)
units={
 'package_pairs':['consumer_package','provider_package'],
 'elf_links':['arch','consumer_sha256','provider_sha256'],
 'symbol_edges':['arch','consumer_sha256','provider_sha256','raw_symbol']
}
counts={}; groups={}
for name,fields in units.items():
    grouped=defaultdict(list)
    for r in mappings: grouped[tuple(r[f] for f in fields)].append(r)
    counts[name]=len(grouped);rows=[]
    for index,(key,items) in enumerate(sorted(grouped.items()),1):
        r=items[0]
        row=dict(id=f'{name}_{index:02}',consumer_package=r['consumer_package'],provider_package=r['provider_package'],candidate_count=len({x['candidate'] for x in items}),candidates=';'.join(sorted({x['candidate'] for x in items})),ranks=','.join(map(str,sorted({x['rank'] for x in items}))),arches=','.join(sorted({x['arch'] for x in items})),evidence=';'.join(sorted({x['proof'] for x in items})))
        if name!='package_pairs':row.update(consumer_elf=r['consumer_elf'],provider_elf=r['provider_elf'],consumer_sha256=r['consumer_sha256'],provider_sha256=r['provider_sha256'])
        if name=='symbol_edges':row.update(raw_symbol=r['raw_symbol'],demangled=r['demangled'])
        rows.append(row)
    tsv(OUT/f'CROSS_{name.upper()}.tsv',rows);groups[name]=rows
save(OUT/'CROSS_COUNTS.json',dict(candidates=44,**counts,candidate_symbol_incidence=len(mappings),architectures=sorted({r['arch'] for r in mappings}),proof_sha256=proofs,elapsed_seconds=time.time()-start))
(OUT/'CROSS_SURFACE.md').write_text('# 已确认跨包面的计数单位\n\n'+f"- 候选：44 个（第一批 34、第二批 10），原阳性结论均保留。\n- 有向源码包对：{counts['package_pairs']} 条，键为消费包 → 提供包。\n- 实际 ELF 依赖边：{counts['elf_links']} 条，键为架构、消费 ELF SHA256、提供 ELF SHA256。\n- 原始符号边：{counts['symbol_edges']} 条，在 ELF 边键上再加原始导入/导出符号。构造/析构不同原始名字若存在会分别计数。\n- 候选—原始符号关联：{len(mappings)} 行，不是独立边数。\n\n"+'换算不是按固定比例乘除：`CROSS_CANDIDATE_EDGE_MAP.tsv` 的每行把一个候选映射到实际证实的边；去掉候选列后，分别按上述键去重。一个 vector 边可证明 string 元素、allocator 子对象等多个候选；同一个包对也可以有多个接口与 ELF。同一候选若有多条已选证据边可关联多行。不得把 44 加到包边数上。\n\n'+'这些数是当前选定阳性证明所覆盖的面，不是所有平台包边的全集，也不是不兼容次数。gtest 消费方含真实 RPM 的测试 ELF，不宣称均为业务生产调用。没有在本轮追加跨包运行实验。\n\n| 消费包 → 提供包 | 关联候选数 | 候选 |\n|---|---:|---|\n'+''.join(f"| {r['consumer_package']} → {r['provider_package']} | {r['candidate_count']} | {r['candidates']} |\n" for r in groups['package_pairs'])+'\n新增 NO_STDLIB_OBJECT 仅用于原 156 项未定的结构定界；它不否认标签或空子对象能够真实跨包。已有 less/hash/allocator 等实际存储链不重判、不删边。\n')
print(json.dumps({k:v for k,v in json.loads((OUT/'CROSS_COUNTS.json').read_text()).items() if k!='proof_sha256'},ensure_ascii=False))
