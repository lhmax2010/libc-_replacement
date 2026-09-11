#!/usr/bin/env python3
"""建立真实库的同侧加载前提；候选依赖有产品变体时不自行选择。"""
import csv, pathlib, json, subprocess, sys, re, hashlib
out=pathlib.Path('docs/progress/API_0911/W1');tmp=pathlib.Path('tmp/API_0911/W1').resolve()
cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
rows=[x for x in csv.DictReader(open('/home/toolchain/development/libc++_replacement/progress/R11/tables/elf_inventory.tsv'),delimiter='\t') if x['arch']=='x86_64' and x['path'].startswith('/usr/lib64/') and x['debug_payload']=='NO']
targets={'ewk':tmp/'chromium_image/lib/libchromium-impl.so'}
providers=json.loads((out/'provider_paths.json').read_text())
for name,package,filename in [('binder','dali2-csharp-binder','libdali2-csharp-binder.so.2.0.0'),('native_control','capi-appfw-app-control','libcapi-appfw-app-control.so.0.26.2')]:
    targets[name]=pathlib.Path(next(x['local_path'] for x in providers if x['name']==package and x['path'].endswith('/'+filename)))
results=[]
for name,provider in targets.items():
    stage=tmp/('deps_'+name);stage.mkdir(exist_ok=True)
    for attempt in range(1,41):
        stem=f'load_{name}_{attempt}'
        code='import ctypes,pathlib; ctypes.CDLL('+repr(str(provider))+'); print(pathlib.Path("/proc/self/maps").read_text()); print("LOAD_ONLY_PASS; no interface or value tested")'
        cmd=['env','LD_LIBRARY_PATH='+str(stage),'python3','-c',code]
        rc=subprocess.run([sys.executable,'docs/progress/API_0911/record.py',str(out/'raw'/stem),'nice','-n','15','ionice','-c','3','timeout','20',*cmd]).returncode
        err=(out/'raw'/(stem+'.stderr')).read_text()
        print(name,attempt,rc,err[-300:],flush=True)
        if rc==0: results.append(dict(target=name,status='LOAD_ONLY_PASS',evidence=stem));break
        missing=re.search(r'OSError: ([^ /:]+\.so[^ :]*): cannot open shared object file',err)
        if not missing:
            results.append(dict(target=name,status='LOAD_FAILED',evidence=stem,detail=err));break
        soname=missing[1]
        candidates=[x for x in rows if pathlib.Path(x['path']).name==soname or pathlib.Path(x['path']).name.startswith(soname+'.')]
        # 文件名更短的无版本 .so 不替代目标 SONAME；只用表中真实 ELF。
        if len(candidates)!=1:
            results.append(dict(target=name,status='DEPENDENCY_UNRESOLVED',evidence=stem,soname=soname,candidates=candidates));break
        x=candidates[0];h=x['rpm_sha256'];p=cache/h[:2]/h/x['path'].lstrip('/')
        if not p.exists():
            results.append(dict(target=name,status='FILE_NOT_AVAILABLE',evidence=stem,path=str(p)));break
        link=stage/soname
        if link.exists():
            results.append(dict(target=name,status='LOAD_FAILED_WITH_DEPENDENCY',evidence=stem));break
        link.symlink_to(p)
        print('STAGED_REAL_DEPENDENCY',soname,str(p),hashlib.sha256(p.read_bytes()).hexdigest(),flush=True)
    else: results.append(dict(target=name,status='DEPENDENCY_ATTEMPT_LIMIT',limit=40))
(out/'load_results.json').write_text(json.dumps(results,indent=2)+'\n')
