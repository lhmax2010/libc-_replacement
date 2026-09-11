#!/usr/bin/env python3
import pathlib,sys,json,hashlib,re,datetime
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,EXT as e
o=r/'docs/progress/ADAPTOR_0912/W2'
rows=json.loads((o/'v2native_matrix.json').read_text())+json.loads((o/'board_matrix.json').read_text())
assert len(rows)==30
result=[]
for arch in ['x86_64','armv7l']:
 for path in ['direct','state','drop']:
  cell=[x for x in rows if x['arch']==arch and x['path']==path];assert len(cell)==5
  for x in cell:
   assert x['maps'] and len(x['cases'])==12 and x['summary']==[['4' if path=='drop' else '12','12']],x
   assert x['exit']==(28 if path=='drop' else 0),x
  result.append(dict(architecture=arch,environment=cell[0]['environment'],path=path,runs=5,case_observations=60,exact_case_observations=sum(c['exact'] for x in cell for c in x['cases']),destroyed_case_scopes=60))
(o/'summary.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
lines=['# 修订原型规范矩阵','', '| 架构/环境 | 路径 | 进程运行数 | 字段断言通过/用例观察数 | 消费侧对象作用域销毁完成 |','| --- | --- | --- | --- | --- |']
for x in result:lines.append(f"| {x['architecture']} / {x['environment']} | {x['path']} | 5 | {x['exact_case_observations']}/60 | 60/60 |")
lines+=['','每进程顺序执行 12 个场景；每场景每路径每架构 5 次。case 11 内另有一次准备状态的真实调用，其字段也断言，但不另算第 13 个场景。Max-Age 的时间值按同次调用前后时钟范围断言，不要求跨进程时间字节相等。','', '初版 15 次原生诊断不混入本表：GNU 直接对照 5 次完整通过，两种 adaptor 初版各 5 次 SIGSEGV。其回溯见 raw/006_debug_state.stdout。','']
(o/'MATRIX.md').write_text('\n'.join(lines))
files=[e/'tmp/corpus/extracted/dali2-2.5.26-1.src.rpm/unpacked/dali2-2.5.26.tar.gz/dali2-2.5.26/dali/public-api/object/property-value.cpp',r/'tmp/API_0911/W1_CONT/chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc',r/'tmp/API_0911/W1_CONT/chromium_extension_source/chromium-efl-1.1.144/tizen_src/chromium_impl/components/xwalk_extensions/browser/xwalk_extension.cc']
(o/'source_identity.json').write_text(json.dumps([dict(path=str(f),sha256=hashlib.sha256(f.read_bytes()).hexdigest()) for f in files],indent=2)+'\n')
cleanup=json.loads((o/'board_cleanup.json').read_text());assert cleanup['directory_absent'] and not cleanup['new_diagnostic_names']
print(json.dumps(result,ensure_ascii=False));print('BOARD_CLEAN_RELEASED',datetime.datetime.now(datetime.timezone.utc).isoformat())
