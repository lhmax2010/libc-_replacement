from common import *
import json,datetime
paths=['src/public/delegate/LWEWebContainerDelegate.cpp','src/core/page/WebView.cpp','src/core/modules/renderer/Renderer.cpp','src/core/modules/renderer/Renderer.h','src/core/modules/renderer/RendererSoftware.cpp','inc/LWEWebView.h']
rows=[]
for name in paths:
    p=SOURCE/name;dest=OUT/'W0/source'/name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes())
    rows.append(dict(path=str(p),snapshot=str(dest.relative_to(OUT)),sha256=digest(p)))
save(OUT/'W0/source_manifest.json',rows)
checks=[]
for arch,file in [('x86_64','x86_64_no-render.json'),('armv7l','armv7l.json')]:
    matrix=json.loads((OUT/'W0'/file).read_text());gnu=next(r for r in matrix if r['library']=='gnu')
    assert len(gnu['runs'])==5
    for r in gnu['runs']:
        s=pathlib.Path(str(ROOT/r['raw'])+'.stdout').read_text()
        assert r['exit']==0 and 'callback_value size=1 hex=33' in s and 'callback_calls=1' in s and 'destroy_completed=1' in s
        assert 'provider_mapped=1 gnu_mapped=1' in s
        checks.append(dict(arch=arch,raw=r['raw'],exit=0,value_hex='33'))
start=json.loads((OUT/'raw/001_resource_gate.time.json').read_text())['start'];end=datetime.datetime.now(datetime.timezone.utc).isoformat()
save(OUT/'W0/SELF_CHECK.json',dict(runs=checks,start=start,end=end,elapsed_seconds=(datetime.datetime.fromisoformat(end)-datetime.datetime.fromisoformat(start)).total_seconds(),gnu_control='FIXED',overall_matrix='PARTIAL',diagnostic_cleanup='PENDING_PERMISSION'))
manifest=[]
for p in sorted(OUT.rglob('*')):
    if not p.is_file() or '__pycache__' in p.parts or p.name=='SHA256.json':continue
    if '/raw/' in str(p):
        suffix=next((s for s in ['.command.txt','.stdout','.stderr','.exitcode','.time.json'] if p.name.endswith(s)),None)
        if suffix and not pathlib.Path(str(p)[:-len(suffix)]+'.time.json').is_file():continue
    manifest.append(dict(path=str(p.relative_to(OUT)),sha256=digest(p)))
save(OUT/'W0/SHA256.json',manifest)
print('verified GNU control runs:',len(checks))
