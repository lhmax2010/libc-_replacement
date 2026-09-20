#!/usr/bin/env python3
"""B 组逐项语义复核结论：依据头文件上下文，非动态符号零交集。"""
from common import *
import re
start=time.time();gate('b_gate')
base=ROOT/'docs/progress/R119_batch2';table={x['rank']:x for x in inputs()}
judgments={
 130: '25 个 SHA+行去重限定名上下文均为函数体内传给 std::async 的启动策略；未出现该枚举的包间公开形参、返回值或非静态成员。此结论不排除未展开宏/别名。',
 144: '唯一限定名样本是 Node util-inl.h:711 的函数局部 smatch；追加无前缀命中区分了 Boost/zypp 自有 smatch 和标准实现副本。没有用 smatch 的动态名字零交集作依据。',
 145: '两个限定名样本属于 libc++ 自身 ranges 特化；无前缀命中为 Boost filesystem、LLVM vfs/sys::fs 自有类型或标准实现副本，非平台公开 std::filesystem::directory_iterator 对象签名。',
 147: '限定名扫描零命中，有正向对照；无前缀命中为 Boost 自有 future_status 枚举和 CUDA 标准实现副本。结论仅及明确名字/已消歧样本，不排除自动推导返回类型。',
 186: '5 个限定名上下文均为函数体局部转换器：Boost 两处、gtest 一处、OpenVINO 两处；Windows 条件分支保留但未当作平台运行证据。无前缀补扫未发现其他平台对象签名。'
}
# 检索正向/负向对照覆盖参数、返回值、成员和注释字符串；不称其为 C++ 解析器。
controls=[]
for rank in judgments:
    name=table[rank]['row']['candidate']; token=re.escape(name)
    positive=f'{name} make(); void accept(const {name}&); struct Box {{ {name} value; }};'
    negative=f'// {name}\nconst char* s="{name}";'
    clean=lambda text:re.sub(r'//[^\n]*|"(?:\\.|[^"\\])*"',' ',text)
    a=len(re.findall(token,clean(positive))); b=len(re.findall(token,clean(negative)))
    assert a==3 and b==0
    controls.append(dict(candidate=name,positive=positive,positive_matches=a,negative=negative,negative_matches=b))
save(OUT/'B_CONTROLS.json',controls)
decisions=[]
for rank,reason in judgments.items():
    item=table[rank];name=item['row']['candidate'];path=base/'W1/entity_full'/(name.replace('::','_')+'.json')
    qualified=json.loads(path.read_text()) if path.exists() else []
    upath=base/'W1/unqualified'/(name.split('::')[-1]+'.json')
    unqualified=json.loads(upath.read_text()) if upath.exists() else []
    checks=[]
    for h in qualified:
        assert sha(h['cache_path'])==h['sha256']
        checks.append(dict(path=h['cache_path'],sha256=h['sha256'],line=h['line']))
    for h in unqualified:
        meta=h['metadata'];p=meta['cache_path']; expected=meta.get('file_sha256')
        actual=sha(p)
        if expected:assert actual==expected
        checks.append(dict(path=p,sha256=actual,line=h.get('line',h.get('lines'))))
    detail=dict(rank=rank,candidate=name,reason=reason,qualified_source=str(path.relative_to(ROOT)),qualified_source_exists=path.exists(),qualified_contexts=qualified,unqualified_source=str(upath.relative_to(ROOT)) if upath.exists() else None,verified_sources=checks,scope='S_R119_20260918',holes=['宏生成','模板实例化','静态链接','未扩扫 armv7l/aarch64 ELF','dlopen/函数指针','生成头','using/别名后的其他无限定名或自动返回类型'],not_based_on_elf_name_zero=True)
    save(OUT/'items'/f'{rank:03}.json',detail)
    decisions.append(dict(rank=rank,candidate=name,original='NOT_OBSERVED',new='INTERNAL_ONLY',subtype='',scope='S_R119_20260918',reason=reason,evidence=f'items/{rank:03}.json'))
save(OUT/'B_RECLASSIFIED.json',decisions)
# 修正初始 B 候选 cv_status：公开返回签名已直接见于冻结头，不可作阴性。
p=base/'W1/entity_full/std_cv_status.json';rows=json.loads(p.read_text());selected=[x for x in rows if x['line'] in (25,31,247)]
save(OUT/'B_TO_C_154.json',dict(rank=154,candidate='std::cv_status',initial_primary='B',final_primary='C',new='NOT_OBSERVED',reason='完整既存上下文中发现公开返回类型；旧摘要漏掉该用法。本轮只纠正成因，不展开函数身份实验。',evidence=selected,missing='tsl::internal::wait_until_system_clock / condition_variable::wait_for 的真实 consumer 与模板实例归属。'))
save(OUT/'B_EXECUTION_TIME.json',dict(elapsed_seconds=time.time()-start,candidates_considered=6,reclassified=5,scope='上下文校验及报告生成；cv_status 改归 C 包含在工作量中'))
print('B reclassified=5; cv_status remains NOT_OBSERVED, primary C')
