#!/usr/bin/env python3
"""从原始矩阵生成可审阅计数；不将诊断、重跑、表示探针混进目标接口格。"""
import pathlib,json,collections,csv,hashlib,re
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';t=r/'tmp/API_0911/W1_CONT'
def read(name):return json.loads((o/name).read_text())
groups=collections.defaultdict(list)
for x in read('ewk_matrix.json'):
 groups[('x86_64 native','ewk_'+x['consumer']+'_to_gnu')].append(dict(exit=x['returncode'],maps=x['maps'],exact=x['exact'],raw=x['raw']))
for x in read('dali_matrix_final.json'):
 groups[('x86_64 native','dali_'+x['scope']+'_'+x['consumer']+'_to_'+x['provider'])].append(dict(exit=x['returncode'],maps=x['maps'],exact=x['exact'],raw=x['raw']))
for x in read('arm_board_matrix.json'):
 groups[('armv7l physical rpi4',x['target'])].append(dict(exit=x['target_exit'],maps=x['maps'],exact=x['exact'],raw=x['raw']))
summary=[]
for (arch,case),items in sorted(groups.items()):
 assert len(items)==5,(arch,case,len(items))
 summary.append(dict(architecture=arch,case=case,repeats=len(items),exits=dict(collections.Counter(str(x['exit']) for x in items)),maps=sum(x['maps'] for x in items),exact=sum(x['exact'] for x in items),raw=[x['raw'] for x in items]))
assert sum(x['repeats'] for x in summary)==80
assert sum(x['exact'] for x in summary)==60
assert len(summary)==16
lines=['# 目标接口主矩阵（每格 5 次）','','只统计最终目标运行格：不计初次 Dali 重跑、加载诊断、表示探针、shim 或前置环境试跑。加载失败格的 maps=0 表示完整 binder 未能与 provider 同时加载，不是缺少检查。','','| 架构 | 测试范围与方向 | 实际次数 | 退出分布 | 完整映射满足 | 具体值符合 | 原始记录 |','| --- | --- | ---: | --- | ---: | ---: | --- |']
for x in summary:
 links=' / '.join('[%s](raw/%s.stdout)'%(i+1,n) for i,n in enumerate(x['raw']))
 lines.append('| '+ ' | '.join([x['architecture'],x['case'],str(x['repeats']),str(x['exits']),str(x['maps']),str(x['exact']),links])+' |')
lines+=['','80 次是已执行目标格的尝试数，不是完整需求覆盖分母。每架构的 EWK libc++ provider 两格、真实 SetSyncBinaryReply 四格，以及完整 Dali 图形/虚调用格仍有未观测项，详见报告。']
(o/'MATRIX.md').write_text('\n'.join(lines)+'\n');(o/'matrix_summary.json').write_text(json.dumps(summary,indent=2)+'\n')
sources=[
 ('EWK implementation',t/'chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc',[(1,100)]),
 ('Browser callback and receiver',t/'chromium_extension_source/chromium-efl-1.1.144/tizen_src/chromium_impl/components/xwalk_extensions/browser/xwalk_extension.cc',[(115,132),(250,269),(330,345),(395,408),(470,503)]),
 ('Service callback is not the browser implementation',t/'chromium_source/chromium-efl-1.1.144/wrt/src/service/node/xwalk_extension_node.cc',[(134,146)])]
facts=['# 真实源码的核查摘录','','源文件未修改。完整归档/成员身份见 source_members.json、extension_source_members.json；此处保留原始行号，便于独立复核。']
for label,path,ranges in sources:
 data=path.read_bytes();ls=data.decode(errors='replace').splitlines();facts+=['','## '+label,'','源路径：`'+str(path)+'`；SHA256：`'+hashlib.sha256(data).hexdigest()+'`。','','```text']
 for start,end in ranges:
  facts.extend(f'{i}: {ls[i-1]}'.rstrip() for i in range(start,min(end,len(ls))+1))
 facts+=['```']
(o/'SOURCE_FACTS.md').write_text('\n'.join(facts)+'\n')
paths=[r/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so',t/'ewk_gnu',t/'ewk_cxx',t/'ewk_shim.so',t/'ewk_shim_probe',t/'dali_core_cxx/libdali2-core.so.2.0.0',t/'arm/dali_core_cxx/libdali2-core.so.2.0.0',r/'tmp/IMPL_0908/build-native/lib/libc++.so.1.0',r/'tmp/IMPL_0908/build-native/lib/libc++abi.so.1.0']
(o/'supplement_binary_identity.json').write_text(json.dumps([dict(path=str(p),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in paths],indent=2)+'\n')
print('TARGET_PRIMARY_CELLS',len(summary),'RUNS',80,'EXACT_VALUE_RUNS',60)
