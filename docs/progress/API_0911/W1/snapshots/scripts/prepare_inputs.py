#!/usr/bin/env python3
"""以冻结 ELF 表定位真实依赖，记录候选歧义，不修改系统搜索路径。"""
import csv, pathlib, json, subprocess, sys, hashlib
base=pathlib.Path('/home/toolchain/development/libc++_replacement')
out=pathlib.Path('docs/progress/API_0911/W1'); tmp=pathlib.Path('tmp/API_0911/W1');tmp.mkdir(parents=True,exist_ok=True)
rows=list(csv.DictReader((base/'progress/R11/tables/elf_inventory.tsv').open(),delimiter='\t'))
selected=[x for x in rows if x['arch']=='x86_64' and x['name'] in {'libdlog','dali2','dali2-adaptor','dali2-toolkit','dali2-csharp-binder','capi-appfw-app-control','capi-appfw-app-common','appcore-agent','capi-appfw-application'} and x['path'].startswith('/usr/lib64/')]
for x in selected:
    h=x['rpm_sha256'];x['local_path']=str(base/'tmp/R11/extracted'/h[:2]/h/x['path'].lstrip('/'))
(out/'provider_paths.json').write_text(json.dumps(selected,indent=2)+'\n')
for x in selected: print(x['name'],x['path'],x['local_path'])
for name in ('chromium-efl-squashfs','chromium-efl-tpk','chromium-efl-browser'):
    r=next(x for x in json.loads((out/'catalog.json').read_text()) if x['name']==name)
    subprocess.run([sys.executable,'docs/progress/API_0911/record.py',str(out/'raw'/('rpm_files_'+name)),'rpm','-qpl',r['rpm']],check=True)
