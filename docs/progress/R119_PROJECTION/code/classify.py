#!/usr/bin/env python3
"""逐处结果；只有实测或可追溯的固定结果类型规则才能关闭。"""
from common import *
from expressions import request
from collections import Counter
import re

def bool_rule(name,rules):
    entry=rules.get(name)
    if not entry or entry['kind']!='BOOL_VALUE':return False
    needle=name.split('::')[-1]
    for lib,rows in entry['definitions'].items():
        relevant=[r for r in rows if re.search(r'^\s*(struct|class|using)\b',r['preprocessed']) and re.search(r'\b'+needle+r'\b',r['preprocessed'])]
        # 不把“函数名叫 is_*”当证据：必须在两侧实际声明片段中看到布尔常量基类。
        if not any(re.search(r'\b(?:__bool_constant|_BoolConstant|bool_constant|true_type|false_type)\b|integral_constant\s*<\s*bool',r['source']) for r in relevant):return False
    return True

def classify(h,rules):
    if h.get('foreign_qualification'):return dict(result='RESULT_UNRESOLVED',reason='限定名还有外层命名空间；不是已确认的全局 std',result_type='NOT_OBSERVED',basis='STATIC')
    if h.get('parse_status')!='BALANCED_LEXICAL_ONLY':return dict(result='RESULT_UNRESOLVED',reason=h.get('reason',h.get('parse_status','')),result_type='NOT_OBSERVED',basis='LEXICAL')
    origin=h['references'][0]['cache_path']
    if '/cuda/std/' in origin:return dict(result='RESULT_UNRESOLVED',reason='CUDA 内嵌标准实现的命名空间/目标配置未闭合，不用宿主 std 测量替代',result_type='NOT_OBSERVED',basis='STATIC')
    name=h['candidate'];args=h['template_arguments'];member=h['suffix_member']
    if member=='value' and bool_rule(name,rules):
        return dict(result='RESULT_NO_OBJECT',reason='两侧该 trait 的实际定义继承布尔 integral_constant；此处 ::value 的类型固定为 const bool，未知的是谓词值而非结果类型。未选择任意 T 代填。',result_type='const bool；谓词值 NOT_OBSERVED',basis='STATIC_DEFINITION_CHAIN',definition='STATIC_RULE_DEFINITIONS.json#'+name,actual_projection=h['template_expression']+'::value')
    expr,why=request(h)
    if not expr:return dict(result='RESULT_UNRESOLVED',reason=why,result_type='NOT_OBSERVED',basis='CONTEXT_REQUIRED')
    key=hashlib.sha256(expr.encode()).hexdigest()[:24];measurements=[]
    for lib in ['gnu','cxx']:
        p=OUT/'measurements'/f'{key}_{lib}.json'
        if not p.exists():return dict(result='RESULT_UNRESOLVED',reason='具体表达式尚未测量',result_type='NOT_OBSERVED',requested_expression=expr,basis='NOT_OBSERVED')
        m=json.loads(p.read_text());measurements.append(m)
    refs=[f'measurements/{key}_{lib}.json' for lib in ['gnu','cxx']]
    if any(m['status']!='MEASURED' for m in measurements):
        return dict(result='RESULT_UNRESOLVED',reason='真实表达式未在两套 C++17 标准头环境均编译通过；见原始错误，未替换实参',result_type='NOT_OBSERVED',measurement=refs,requested_expression=expr,basis='COMPILE_FAILURE')
    texts=[m['result'] for m in measurements]
    types=[re.search(r'TYPE=.*?\[T = (.*?)\]',s)[1] for s in texts]
    bases=[re.search(r'BASE=.*?\[T = (.*?)\]',s)[1] for s in texts]
    common=dict(result_type={'gnu':types[0],'cxx':types[1]},base_type={'gnu':bases[0],'cxx':bases[1]},measurement=refs,requested_expression=expr,basis='MEASURED_BOTH',projection_rule=why or 'EXACT_ACTUAL_PROJECTION',condition=(args[0] if why.startswith('CONDITIONAL_ENABLE_IF') else ''),availability=('RESULT_TYPE_FIXED_IF_SUBSTITUTION_SUCCEEDS; actual instantiation NOT_OBSERVED' if why else 'type expression measured; platform call not executed'))
    if all('kind=void' in s or 'kind=builtin' in s or 'empty=1' in s for s in texts):
        return dict(common,result='RESULT_NO_OBJECT',reason='实际结果类型两侧为内建/void/空类型；若为约束投影，其存在性不等于已实例化。指针/引用的对象基底同时检查。')
    if any((b.startswith('std::') or b.startswith('const std::')) and 'empty=0' in s and ('class=1' in s or 'union=1' in s) for b,s in zip(bases,texts)):
        return dict(common,result='RESULT_STDLIB_OBJECT',reason='实际结果/指向对象基底在 std 命名空间且测得非空类/联合体；成员布局见对应 layout_record，是否跨包另查。')
    return dict(common,result='RESULT_UNRESOLVED',reason='具体类型已测得，但属于枚举、函数或其他非 std 状态类型；未将它混入内建/void/空类。分类口径/对象链待核。')

def main(end):
    rules=json.loads((OUT/'STATIC_RULE_DEFINITIONS.json').read_text())['rules'];start=time.time();summary=[];verified={};object_rows=[]
    previous={}
    if (OUT/'CANDIDATE_RESULTS.tsv').exists() and '--refresh' not in sys.argv:
        previous={int(r['rank']):r for r in csv.DictReader((OUT/'CANDIDATE_RESULTS.tsv').open(),delimiter='\t')}
    previous_objects=json.loads((OUT/'STDLIB_OBJECT_OCCURRENCES.json').read_text()) if (OUT/'STDLIB_OBJECT_OCCURRENCES.json').exists() else []
    for ordinal,c in enumerate(candidates()[:end],1):
        rank=int(c['rank'])
        if rank in previous:
            old=previous[rank]
            for k in ['ordinal','rank','occurrences','no_object','stdlib_object','unresolved','static_no_object','measured_occurrences']:old[k]=int(old[k])
            old['elapsed_seconds']=float(old['elapsed_seconds']);summary.append(old)
            object_rows.extend(h for h in previous_objects if h['rank']==rank)
            continue
        gate(f'classify_gate_{end}_{ordinal}');begin=time.time()
        with gzip.open(OUT/f'occurrences/{rank:03}.json.gz','rt') as f:rows=json.load(f)
        out=[]
        for h in rows:
            for source in h['references']:
                path=source['cache_path']
                if path not in verified:verified[path]=sha(path)
                if verified[path]!=source['sha256']:raise RuntimeError('SOURCE_SHA_CHANGED '+path)
            decision=classify(h,rules);h['decision']=decision;out.append(h)
            if decision['result']=='RESULT_STDLIB_OBJECT':object_rows.append(h)
        target=OUT/f'results/{rank:03}.json.gz';target.parent.mkdir(parents=True,exist_ok=True)
        with gzip.open(target,'wt') as f:json.dump(out,f,ensure_ascii=False)
        counts=Counter(h['decision']['result'] for h in out)
        # 有一个未定就不能把整个候选标为已闭合。对象与未定可同时存在。
        overall='RESULT_UNRESOLVED' if counts['RESULT_UNRESOLVED'] else ('RESULT_STDLIB_OBJECT' if counts['RESULT_STDLIB_OBJECT'] else 'RESULT_NO_OBJECT')
        r=dict(ordinal=ordinal,rank=rank,candidate=c['candidate'],overall=overall,occurrences=len(out),no_object=counts['RESULT_NO_OBJECT'],stdlib_object=counts['RESULT_STDLIB_OBJECT'],unresolved=counts['RESULT_UNRESOLVED'],static_no_object=sum(h['decision']['basis']=='STATIC_DEFINITION_CHAIN' for h in out),measured_occurrences=sum(h['decision']['basis']=='MEASURED_BOTH' for h in out),elapsed_seconds=time.time()-begin,evidence=str(target.relative_to(OUT)))
        summary.append(r);print(r,flush=True)
    tsv(OUT/'CANDIDATE_RESULTS.tsv',summary);save(OUT/f'VERIFIED_HEADERS_classify_{end:02}.json',verified)
    save(OUT/'STDLIB_OBJECT_OCCURRENCES.json',object_rows)
    save(OUT/f'CHECKPOINT_{end:02}.json',dict(completed_candidates=len(summary),of_total=65,candidate_results=dict(Counter(r['overall'] for r in summary)),candidates_with_object=sum(r['stdlib_object']>0 for r in summary),occurrence_results=dict(no_object=sum(r['no_object'] for r in summary),stdlib_object=sum(r['stdlib_object'] for r in summary),unresolved=sum(r['unresolved'] for r in summary)),elapsed_this_run=time.time()-start,objects_need_boundary_check=True))
if __name__=='__main__':main(int(sys.argv[1]))
