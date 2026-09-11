#!/usr/bin/env python3
"""仅在隔离目录选用获批 elementary-tizen，续查真实目标依赖。"""
import csv, pathlib, json, subprocess, sys, re, hashlib, time, os
root=pathlib.Path.cwd();out=root/'docs/progress/API_0911/W1_CONT';tmp=root/'tmp/API_0911/W1_CONT';tmp.mkdir(parents=True,exist_ok=True)
cache=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
rows=[x for x in csv.DictReader(open('/home/toolchain/development/libc++_replacement/progress/R11/tables/elf_inventory.tsv'),delimiter='\t') if x['arch']=='x86_64' and x['path'].startswith('/usr/lib64/') and x['debug_payload']=='NO']
old=root/'tmp/API_0911/W1'
targets={'ewk':old/'chromium_image/lib/libchromium-impl.so','binder':pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted/38/38113041d61d0b68d0a65af809402ff283177071a8fa0b76bb23c188dfa86dcd/usr/lib64/libdali2-csharp-binder.so.2.0.0')}
results=[];staged=[];tag=os.environ.get('API_LOAD_ATTEMPT','')
for name,provider in targets.items():
    stage=tmp/('deps_'+name);stage.mkdir(exist_ok=True)
    for p in (old/('deps_'+name)).iterdir():
        if p.is_symlink() and not (stage/p.name).exists():(stage/p.name).symlink_to(p.resolve())
    for attempt in range(1,161):
        stem=f'load_{tag}{name}_{attempt}'
        code='import ctypes,pathlib; ctypes.CDLL('+repr(str(provider))+'); print(pathlib.Path("/proc/self/maps").read_text()); print("LOAD_ONLY_PASS; target values not tested")'
        rc=subprocess.run([sys.executable,str(root/'docs/progress/API_0911/record.py'),str(out/'raw'/stem),'nice','-n','15','ionice','-c','3','timeout','20','env','LD_LIBRARY_PATH='+str(stage),'python3','-c',code]).returncode
        err=(out/'raw'/(stem+'.stderr')).read_text();print(name,attempt,rc,err[-300:],flush=True)
        if rc==0:results.append(dict(target=name,status='LOAD_ONLY_PASS',evidence=stem));break
        missing=re.search(r'OSError: ([^ /:]+\.so[^ :]*): cannot open shared object file',err)
        if not missing:results.append(dict(target=name,status='LOAD_FAILED',evidence=stem,detail=err));break
        soname=missing[1]
        candidates=[x for x in rows if pathlib.Path(x['path']).name==soname or pathlib.Path(x['path']).name.startswith(soname+'.')]
        if not candidates:
            for x in rows:
                if pathlib.Path(x['path']).name.split('.so')[0]!=soname.split('.so')[0]:continue
                h=x['rpm_sha256'];p=cache/h[:2]/h/x['path'].lstrip('/');alias=p.parent/soname
                if alias.exists() and alias.resolve()==p.resolve():candidates.append(x)
        if soname=='libelementary.so.1':candidates=[x for x in candidates if x['name']=='elementary-tizen']
        if len(candidates)!=1:results.append(dict(target=name,status='DEPENDENCY_UNRESOLVED',evidence=stem,soname=soname,candidates=candidates));break
        x=candidates[0];h=x['rpm_sha256'];p=cache/h[:2]/h/x['path'].lstrip('/')
        if not p.exists():results.append(dict(target=name,status='FILE_NOT_AVAILABLE',evidence=stem,path=str(p)));break
        link=stage/soname
        if link.exists():results.append(dict(target=name,status='LOAD_FAILED_WITH_DEPENDENCY',evidence=stem));break
        link.symlink_to(p)
        row=dict(target=name,soname=soname,package=x['name'],path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest());staged.append(row);print('REAL_DEPENDENCY',json.dumps(row),flush=True)
    else:results.append(dict(target=name,status='DIAGNOSTIC_BATCH_LIMIT',limit=160))
(out/'load_results.json').write_text(json.dumps(results,indent=2)+'\n')
(out/'staged_dependencies.json').write_text(json.dumps(staged,indent=2)+'\n')
