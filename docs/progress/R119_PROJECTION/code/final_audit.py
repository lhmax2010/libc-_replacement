#!/usr/bin/env python3
"""全量位置对账、逐项缺口、对象结构证据；不改变既有分类。"""
from common import *
from collections import Counter
import re
gate('final_audit_gate')
all_ids=set(); coverage=[]; gap_rows=[]; reasons=Counter(); bases=Counter(); decisions=Counter(); verified={}
for c in candidates():
    rank=int(c['rank'])
    with gzip.open(OUT/f'inventory/{rank:03}.json.gz','rt') as f: inv=json.load(f)
    with gzip.open(OUT/f'results/{rank:03}.json.gz','rt') as f: rows=json.load(f)
    expected={(r['sha256'],int(r['line'])) for r in inv}
    actual={(r['references'][0]['sha256'],r['line']) for r in rows}
    assert expected==actual,(rank,expected-actual,actual-expected)
    for r in inv:
        path=r['cache_path']
        if path not in verified:verified[path]=sha(path)
        assert verified[path]==r['sha256'],path
    counts=Counter();examples={}
    for h in rows:
        assert h['id'] not in all_ids,h['id'];all_ids.add(h['id'])
        d=h['decision'];decisions[d['result']]+=1;bases[d['basis']]+=1
        if d['result']=='RESULT_UNRESOLVED':
            reason=d['reason'];counts[reason]+=1;reasons[reason]+=1;examples.setdefault(reason,h['id'])
    for reason,n in counts.items():gap_rows.append(dict(rank=rank,candidate=c['candidate'],occurrences=n,missing_or_reason=reason,example_id=examples[reason],all_positions=f'results/{rank:03}.json.gz'))
    coverage.append(dict(rank=rank,index_rows=len(inv),unique_sha_line=len(expected),result_positions=len(rows),coverage_equal=True))
tsv(OUT/'UNRESOLVED_REASONS.tsv',gap_rows);tsv(OUT/'COVERAGE_AUDIT.tsv',coverage)
protected=json.loads((OUT/'FROZEN_INPUT_SHA256.json').read_text())
for p,digest in protected.items():assert sha(ROOT/p)==digest,p
save(OUT/'VERIFIED_HEADERS_final.json',verified)
save(OUT/'FINAL_AUDIT.json',dict(candidates=len(coverage),index_rows=sum(r['index_rows'] for r in coverage),unique_candidate_sha_line=sum(r['unique_sha_line'] for r in coverage),actual_positions=len(all_ids),all_frozen_rows_covered=True,all_position_ids_unique=True,verified_source_paths=len(verified),protected_files_unchanged=len(protected),decisions=dict(decisions),basis=dict(bases),unresolved_reason_counts=dict(reasons)))

# 保存精确对象布局，不以 is_empty=false 单独充当非静态成员证明。
objects=json.loads((OUT/'STDLIB_OBJECT_OCCURRENCES.json').read_text());proofs={}
for h in objects:
    for ref in h['decision']['measurement']:
        if ref in proofs:continue
        m=json.loads((OUT/ref).read_text());layout=ROOT/(m['layout_record']+'.stdout');text=layout.read_text();expr=m['expression']
        if 'aligned_storage' in expr:needle=r'\| union std::aligned_storage<'+('16' if 'sizeof' in expr else '8')+r', 8>::type\s*\n'
        elif 'ostream' in expr:needle=r'\| (?:class std::basic_ostream<char>|std::ostream)\s*\n'
        else:needle=r'\| (?:class std::basic_string<char>|std::string)\s*\n'
        found=[b for b in text.split('*** Dumping AST Record Layout') if re.search(needle,b)]
        assert found,(ref,needle)
        excerpt='\n'.join(found)
        field_test=r'__data' if 'aligned_storage' in expr else (r'_M_precision|__precision_' if 'ostream' in expr else r'_M_string_length|__data_')
        assert re.search(field_test,excerpt),(ref,'missing nonstatic field')
        proofs[ref]=dict(expression=expr,library=m['library'],measurement=ref,layout_record=m['layout_record'],layout_sha256=sha(layout),nonstatic_member_regex=field_test,layout_excerpt=excerpt)
save(OUT/'OBJECT_MEMBER_PROOFS.json',proofs)

boundaries=json.loads((OUT/'BOUNDARY_SEARCH.json').read_text());table=[];excluded=[]
for h in boundaries:
    p=h['installed_path'];q=h['query_result'];line=h['line']
    if p.endswith('any.hpp'):
        form='私有 storage_union::stack 子对象；外层 linb::any 的对象表示，不能当作空标签'
        missing='所查提供者相关方法42个定义无精确 UND 消费链；仍缺包含该子对象的外层公开签名与异包消费者绑定'
    elif p.endswith('event.h'):
        form='TraceEvent 私有 PayloadStorage _payload 成员；对象表示链已见，不等于跨包调用已见'
        missing='所查 TraceEvent 76个定义无精确 UND 消费链；缺外层对象在异包间传递及实际消费证据'
    else:
        form='安装头模板返回类型（条件成立时）；SDK公开承诺及实际实例化 NOT_OBSERVED'
        if p.endswith('ostreamMethods.h'):
            missing='同名重载命中不能替代此模板：唯一异源 RPM 命中为 TfToken const&，非六个容器重载；缺这些精确实例的异包消费者'
        elif p.endswith('streamOut.h'):
            missing='已见 VtStreamOut 定义与同源 RPM 引用；尚无这两处模板实例的异包消费链'
        else:missing='按函数身份未见所查提供者导出；可能内联/实例化在消费方，需消费方实例及实际跨包签名；不据零命中判不存在'
    for e in q['exact_consumer_links']:
        if e['different_source_rpm']:
            assert 'TfToken const&' in e['name'],e['name']
            excluded.append(dict(occurrence_id=h['occurrence_id'],symbol=e['name'],reason='非本投影所在容器重载',provider=e['provider'],consumer=e['consumer']))
    table.append(dict(occurrence_id=h['occurrence_id'],candidate=h['candidate'],source=f'{p}:{line}',result_type=json.dumps(h['result_type'],ensure_ascii=False),signature_or_member=form,provider_elfs=len(q['provider_elfs']),identity_definitions=len(q['provider_defined_symbols']),identity_consumer_links=len(q['exact_consumer_links']),new_cross_edge='NOT_OBSERVED',missing=missing,evidence='BOUNDARY_SEARCH.json#'+h['occurrence_id']))
tsv(OUT/'OBJECT_BOUNDARIES.tsv',table);save(OUT/'EXCLUDED_OTHER_OVERLOADS.json',excluded)
old=json.loads((OLD/'CROSS_COUNTS.json').read_text())
save(OUT/'EDGE_COUNTS_BEFORE_AFTER.json',dict(before=old,confirmed_new_package_pairs=0,confirmed_new_symbol_edges=0,after_package_pairs=old['package_pairs'],after_symbol_edges=old['symbol_edges'],meaning='已确认下界不变；不是投影结果不存在新增跨包面的证明',object_projection_positions=13,object_projection_candidates=2))
# 零新增表也沿用旧表头，不能拿空表证明未定项目不存在。
for source,target in [('CROSS_PACKAGE_PAIRS.tsv','NEW_PACKAGE_PAIRS.tsv'),('CROSS_SYMBOL_EDGES.tsv','NEW_SYMBOL_EDGES.tsv')]:
    (OUT/target).write_text((OLD/source).read_text().splitlines()[0]+'\n')
print(json.dumps(json.loads((OUT/'FINAL_AUDIT.json').read_text()),ensure_ascii=False,indent=2))
