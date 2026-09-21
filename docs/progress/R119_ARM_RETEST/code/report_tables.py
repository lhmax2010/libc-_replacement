#!/usr/bin/env python3
from common import *
rows=list(csv.DictReader((OUT/'ARMV7L_TYPES.tsv').open(),delimiter='\t'))
lines=['# 13 项 armv7l 逐项结果','','环境：交叉编译 + QEMU 用户态，GNU / libc++ 各五轮。完整 TOKEN 两侧均 `9TypeTokenIxE`；DIRECT 两侧均 `x`。','', '| 项 | 类型 | GNU 实际类型 | libc++ 实际类型 | GNU/CXX 编码 | size | align | 结果 |','| --- | --- | --- | --- | --- | --- | --- | --- |']
for r in rows:lines.append(f'| {r["id"]} | `{r["expression"]}` | {r["gnu_type"]} | {r["cxx_type"]} | `{r["gnu_direct"]}` / `{r["cxx_direct"]}` | {r["gnu_size"]} / {r["cxx_size"]} | {r["gnu_align"]} / {r["cxx_align"]} | {r["result"]} |')
(OUT/'ARMV7L_TYPES.md').write_text('\n'.join(lines)+'\n')
actual=json.loads((OUT/'ACTUAL_ARM_EDGES.json').read_text());edge_rows=[]
for e in actual:
 p,c=e['provider'],e['consumer'];kind=e['interface'];names=('dali2-ui-foundation','dali2') if kind=='dali' else ('zypper','libzypp')
 edge_rows.append(dict(id='arm_retest_'+kind,consumer_package=names[0],provider_package=names[1],candidate_count=1,candidates='std::chrono::steady_clock::rep' if kind=='dali' else 'std::streamoff',ranks='T0369' if kind=='dali' else 'T0001',arches='armv7l',evidence='ACTUAL_ARM_EDGES.json#'+kind,consumer_elf='/'+str(pathlib.Path(c['path']).relative_to(c['package']['extracted'])),provider_elf='/'+str(pathlib.Path(p['path']).relative_to(p['package']['extracted'])),consumer_sha256=c['sha256'],provider_sha256=p['sha256'],raw_symbol=e['symbol'],category_discrepancy='NOT_PRESENT_IN_TESTED_CONFIGURATION',boundary_present='YES'))
tsv(OUT/'ARM_CONFIRMED_EDGES.tsv',edge_rows)
save(OUT/'COUNTING_SCOPE.json',dict(x86_64_prior=dict(package_pairs=18,symbol_edges=23,evidence='../R119_DIVERGENT/CROSS_SYMBOL_EDGES_UPDATED.tsv',unchanged=True),armv7l=dict(selected_package_pairs_examined=2,selected_symbol_edges_examined=2,selected_edges_present=2,selected_edges_with_builtin_type_divergence=0,whole_platform_package_pairs='NOT_OBSERVED',whole_platform_symbol_edges='NOT_OBSERVED'),rollback_to_16_21='NOT_APPLIED: mismatch category disappearing does not erase measured package edges; prior denominator not an armv7l audit'))
print('ARM table 13; actual selected edges 2; no aggregate cross-architecture extrapolation')
