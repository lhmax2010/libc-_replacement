#!/usr/bin/env python3
"""将 SDK 配方、包候选和头文件形态相交，产出待确认范围；不作阶段二判断。"""
import collections
import csv
import fnmatch
import gzip
import json
import pathlib
import re

OUT=pathlib.Path('docs/progress/P7_0909')
def load(name):
    p=OUT/'tables'/name
    stream=p.open() if p.exists() else gzip.open(str(p)+'.gz','rt')
    with stream:return list(csv.DictReader(stream,delimiter='\t'))
def write(name,fields,rows):
    with (OUT/'tables'/name).open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n',extrasaction='ignore');w.writeheader();w.writerows(rows)

packages=load('packages.tsv');headers=load('headers.tsv');recipes=load('sdk_recipes.tsv')
matches=load('sdk_recipe_package_matches.tsv');patterns=load('sdk_recipe_required_paths.tsv')
header_by_name=collections.defaultdict(list)
header_by_rpm=collections.defaultdict(list)
for h in headers:
    if h['scope']=='INCLUDE_TREE':
        header_by_name[h['name']].append(h)
        header_by_rpm[h['rpm_sha256']].append(h)
recipe_rows=[];matched_paths=[]
public_groups=collections.defaultdict(set);internal_groups=collections.defaultdict(set)
public_header_keys=set();internal_header_keys=set()
def path_match(path,pattern):
    return fnmatch.fnmatchcase(path,pattern) or (not any(c in pattern for c in '*?[') and path.startswith(pattern.rstrip('/')+'/'))
for recipe in recipes:
    kind=recipe['kind'];group=recipe['group'];snapshot=recipe['snapshot']
    names=sorted({r['name'] for r in matches if r['snapshot']==snapshot})
    needed=[r['pattern'] for r in patterns if r['snapshot']==snapshot]
    for name in names:
        if kind=='PUBLIC':public_groups[name].add(group)
        elif kind=='INTERNAL':internal_groups[name].add(group)
    found=[]
    for name in names:
        for h in header_by_name[name]:
            candidates=[p for p in needed if path_match(h['path'],p)]
            if not candidates:continue
            found.append(h)
            key=(h['rpm_sha256'],h['path'])
            if kind=='PUBLIC':public_header_keys.add(key)
            elif kind=='INTERNAL':internal_header_keys.add(key)
            matched_paths.append(dict(kind=kind,group=group,snapshot=snapshot,name=name,arch=h['arch'],rpm_sha256=h['rpm_sha256'],path=h['path'],form=h['form'],cpp_signal=h['cpp_signal'],evidence_line=h['evidence_line'],file_sha256=h['file_sha256'],patterns=';'.join(candidates)))
    forms=collections.Counter(h['form'] for h in found)
    recipe_rows.append({**recipe,'matched_binary_names':';'.join(names),'matched_include_paths':len(found),'c_shape_headers':forms['C_DECLARATION_SHAPE'],'cpp_signal_headers':forms['CPP_DECLARATION_SIGNAL'],'unresolved_headers':forms['UNRESOLVED_OR_INCLUDE_MACRO_ONLY'],'unavailable_headers':forms['NOT_AVAILABLE'],'interpretation':'UNION_OF_RECIPE_CONDITIONS_NOT_EXECUTED'})
write('sdk_api_groups.tsv',list(recipe_rows[0]),recipe_rows)
write('sdk_header_path_matches.tsv',['kind','group','snapshot','name','arch','rpm_sha256','path','form','cpp_signal','evidence_line','file_sha256','patterns'],matched_paths)

providers=[]
for p in packages:
    hs=header_by_rpm[p['checksum']]
    eligible=[h for h in hs if h['private_path']!='YES']
    cpp=[h for h in eligible if h['form']=='CPP_DECLARATION_SIGNAL']
    public_h=[h for h in hs if (h['rpm_sha256'],h['path']) in public_header_keys]
    public_cpp=[h for h in public_h if h['form']=='CPP_DECLARATION_SIGNAL']
    aggregate=p['name'].startswith('app-rootstrap')
    role='SDK_AGGREGATE' if aggregate else ('INCLUDE_PROVIDER' if hs else ('NON_INCLUDE_HEADERS' if int(p['header_paths']) else 'DEVEL_WITHOUT_HEADERS'))
    if aggregate:
        proposal='SEPARATE_SDK_REPLICA';reason='开发根聚合副本，不作为独立接口提供方重复分析'
    elif p['name'].startswith('dali2') or p['name']=='libscl-ui-nui-devel':
        proposal='PROPOSE_FOCUS';reason='用户指定 UI 重点；需分 public/devel/integration/C# 绑定，不先认定应用直接可用'
    elif p['category']=='WEB_ENGINE_RELATED' and hs:
        proposal='PROPOSE_FOCUS';reason='用户指定 Web/Chromium 重点；公开 EWK 与其它安装头文件必须区分'
    elif public_cpp and not re.match(r'(gcc|glibc|kernel|linux|llvm|clang)',p['sourcerpm']):
        proposal='PROPOSE_PUBLIC_CPP';reason='public 配方必要路径内有 C++ 声明信号；须核查是否为实际应用边界'
    elif public_cpp:
        proposal='CONFIRM_COMMON_CPP_SCOPE';reason='公开配方通用/标准库 C++ 头文件；是否逐组纳入由人工确认'
    elif cpp:
        proposal='CONFIRM_APPLICATION_EXPOSURE';reason='安装头文件有 C++，但没有相应 public 必要路径交集证据；先确认应用使用范围'
    elif hs and p.get('unavailable_headers') not in ('','0'):
        proposal='RESOLVE_HEADER_GAP';reason='头文件不可读/符号链接等尚未闭合，不能归为纯 C'
    elif hs:
        proposal='DEFER_C_SHAPE_OR_UNRESOLVED';reason='非内部候选头未发现所列 C++ 强特征；可暂缓对外 C++ 深查，不豁免 C ABI/生命周期/异常核验，也不抹去内部 C++ 命中'
    else:
        proposal='DEFER_NO_INCLUDE_API';reason='没有本轮识别的 include 头文件；不是证明不存在应用接口'
    providers.append({**p,'role':role,'public_recipe_groups':';'.join(sorted(public_groups[p['name']])),'internal_recipe_groups':';'.join(sorted(internal_groups[p['name']])),'public_required_headers':len(public_h),'public_required_cpp_headers':len(public_cpp),'nonprivate_cpp_headers':len(cpp),'proposal':proposal,'proposal_reason':reason})
write('provider_scope.tsv',list(providers[0]),providers)
bycat=[]
for cat in ['NATIVE_CAPI_CANDIDATE','UI_FRAMEWORK','WEB_ENGINE_RELATED','SERVICE_MIDDLEWARE_CANDIDATE','OTHER']:
    ps=[p for p in providers if p['category']==cat and p['role']=='INCLUDE_PROVIDER']
    names={p['name'] for p in ps};hs=[h for h in headers if h['category']==cat and h['name'] in names and h['scope']=='INCLUDE_TREE'];forms=collections.Counter(h['form'] for h in hs)
    bycat.append(dict(category=cat,provider_binary_groups=len(ps),source_rpm_records=len({p['sourcerpm'] for p in ps}),packages_with_cpp_signal=sum(int(p['cpp_signal_headers'])>0 for p in ps),packages_c_shape_without_cpp=sum(int(p['c_shape_headers'])>0 and int(p['cpp_signal_headers'])==0 and int(p['unavailable_headers'])==0 for p in ps),header_paths=len(hs),c_shape_headers=forms['C_DECLARATION_SHAPE'],cpp_signal_headers=forms['CPP_DECLARATION_SIGNAL'],unresolved_headers=forms['UNRESOLVED_OR_INCLUDE_MACRO_ONLY'],unavailable_headers=forms['NOT_AVAILABLE']))
write('provider_category_summary.tsv',list(bycat[0]),bycat)
source_groups=collections.defaultdict(list)
for p in providers:
    if p['proposal'] in {'PROPOSE_FOCUS','PROPOSE_PUBLIC_CPP'}:source_groups[p['sourcerpm']].append(p)
scopes=[]
for source,ps in sorted(source_groups.items()):
    scopes.append(dict(source_rpm=source,binary_names=';'.join(p['name'] for p in ps),categories=';'.join(sorted({p['category'] for p in ps})),nonprivate_cpp_headers=sum(p['nonprivate_cpp_headers'] for p in ps),public_required_cpp_headers=sum(p['public_required_cpp_headers'] for p in ps),proposal_reasons=';'.join(sorted({p['proposal_reason'] for p in ps}))))
write('proposed_source_groups.tsv',list(scopes[0]) if scopes else ['source_rpm'],scopes)
summary=dict(provider_roles=dict(collections.Counter(p['role'] for p in providers)),proposals=dict(collections.Counter(p['proposal'] for p in providers)),proposed_source_groups=len(scopes),public_recipes=len([r for r in recipe_rows if r['kind']=='PUBLIC']),public_recipes_with_matched_headers=sum(r['kind']=='PUBLIC' and r['matched_include_paths']>0 for r in recipe_rows),public_recipes_with_cpp=sum(r['kind']=='PUBLIC' and r['cpp_signal_headers']>0 for r in recipe_rows),public_header_membership_paths=len(public_header_keys),internal_header_membership_paths=len(internal_header_keys))
(OUT/'SCOPE_SUMMARY.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary))
for r in scopes:print(r['source_rpm'],r['binary_names'])
