#!/usr/bin/env python3
"""只对有本次结构结果或逐项头上下文结论者重判，其他不动。"""
from common import *
import gzip, re
start=time.time();gate('ab_gate')
cases=json.loads((OUT/'A_CASES.json').read_text());results=json.loads((OUT/'A_RESULTS.json').read_text())
defs={lib:json.loads((OUT/f'A_DEFINITIONS_{lib}.json').read_text()) for lib in ['gnu','cxx']}
table={x['rank']:x for x in inputs()}; decisions=[]
for case in cases:
    rank=case['rank'];item=table[rank];name=case['candidate'];key=str(rank)
    assert all(defs[lib][key] for lib in defs),(rank,'missing actual definition')
    pair={r['library']:r['results'][key] for r in results}
    if case['subtype']=='BUILTIN_ALIAS':
        actual=[re.search(r'\[T = (.*?)\]',text)[1] for text in pair.values()];assert len(set(actual))==1
        reason='两侧实际展开一致：'+actual[0]+'；仅限 x86_64 本次头配置。'
    elif case['subtype']=='EMPTY_TYPE':
        assert all('empty=1' in text and 'size=1' in text for text in pair.values())
        reason='两侧指定实例 is_empty=1、sizeof=1；继承的空基类计入空性检查。空对象可有地址/大小，不等于任意接口无条件兼容。'
    else:
        reason='本次实际定义为类型投影或静态/编译期查询；平台样本使用该投影/查询，不承载该元设施的独立运行时表示。结果类型的对象另计，不能抹去其存储与跨包风险。'
    # 绑定实际平台样本和其当前内容摘要，不能只给概括性 old note。
    base=item['base']; path=base/'W1/entity_full'/(name.replace('::','_')+('.json.gz' if item['rank']<=100 else '.json'))
    if path.exists():
        with (gzip.open(path,'rt') if path.suffix=='.gz' else path.open()) as f: samples=json.load(f)
    else:samples=item['dossier'].get('header_samples',[])
    if isinstance(samples,dict):samples=samples.get('contexts',samples.get('items',samples.get('samples',[])))
    verified=[]
    for sample in samples[:3]:
        p=pathlib.Path(sample.get('cache_path',sample.get('metadata',{}).get('cache_path','')))
        if not p.is_file():continue
        verified.append(dict(source=sample,current_sha256=sha(p)))
    if not verified:raise RuntimeError(f'missing actual platform sample {rank}: {path}')
    detail=dict(case=case,observed=pair,definition_files={lib:f'A_DEFINITIONS_{lib}.json#{rank}' for lib in defs},platform_samples=verified,source_dossier=str(item['dossier_path'].relative_to(ROOT)),scope='S_A: x86_64 C++17 两侧实际标准实现及所列平台样本；不覆盖用户自定义特化与投影结果对象的新边界。')
    save(OUT/'items'/f'{rank:03}.json',detail)
    decisions.append(dict(rank=rank,candidate=name,original='NOT_OBSERVED',new='NO_STDLIB_OBJECT',subtype=case['subtype'],scope='S_A',reason=reason,evidence=f'items/{rank:03}.json'))
save(OUT/'A_RECLASSIFIED.json',decisions)
save(OUT/'A_VALIDATION_TIME.json',dict(elapsed_seconds=time.time()-start,candidates=len(cases),scope='证据验证及逐项报告生成，不含先前共享探针时间'))
print('A verified',len(decisions))
