#!/usr/bin/env python3
"""读取冻结 x86_64 ELF 与 pkg-config 元数据，不加载/执行目标库。"""
import csv,pathlib,hashlib,subprocess,sys,json
P=pathlib.Path('docs/progress/P7_0909');O=P/'stage2';C=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
    return h.hexdigest()
def run(stem,cmd):
    subprocess.run([sys.executable,str(P/'record.py'),str(O/'raw'/stem),*cmd],check=True)
    return (O/'raw'/f'{stem}.stdout').read_text()
rows=list(csv.DictReader((P/'inputs/binary_package_records.tsv').open(),delimiter='\t'))
wanted=set(sys.argv[1:]) or {'dali2','dali2-toolkit','dali2-csharp-binder-profile_common','libscl-ui-nui','lightweight-web-engine-profile_mobile','webapi-plugins-profile_common'}
result=[]
for r in rows:
    if r['arch']!='x86_64' or r['name'] not in wanted:continue
    rpm=C/'rpms'/r['repo_id']/r['location'];assert rpm.exists(),rpm;assert sha(rpm)==r['checksum']
    root=C/'extracted'/r['checksum'][:2]/r['checksum'];assert root.is_dir(),root
    metadata=run('elf_'+r['name']+'_rpm',['rpm','-qp','--qf','[%{FILENAMES}\t%{FILEDIGESTS}\n]',str(rpm)])
    expected={line.split('\t')[0]:line.split('\t')[1] for line in metadata.splitlines() if '\t' in line}
    found=0
    for p in root.rglob('*'):
        if not p.is_file() or p.is_symlink():continue
        with p.open('rb') as f:magic=f.read(4)
        if magic!=b'\x7fELF':continue
        if r['name']=='webapi-plugins-profile_common' and 'application' not in p.name:continue
        path='/'+str(p.relative_to(root));digest=sha(p);assert digest==expected[path],(path,digest,expected[path])
        stem='elf_'+r['name']+'_'+p.name
        run(stem+'_dynamic',['readelf','-dW',str(p)]);run(stem+'_symbols',['readelf','--dyn-syms','-W',str(p)])
        result.append(dict(package=r['name'],arch=r['arch'],rpm_sha256=r['checksum'],path=path,sha256=digest,symbol_record='raw/'+stem+'_symbols.stdout',dynamic_record='raw/'+stem+'_dynamic.stdout'));found+=1
    print(r['name'],'ELFs',found)
with (O/'tables'/('elf_identity_additional.tsv' if sys.argv[1:] else 'elf_identity.tsv')).open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(result[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(result)
if sys.argv[1:]:sys.exit(0)
packages=list(csv.DictReader((O/'tables/packages.tsv').open(),delimiter='\t'));pcs=[]
for r in packages:
    if r['group'] not in {'DALI_NUI','WEB'}:continue
    root=C/'extracted'/r['checksum'][:2]/r['checksum']
    for p in root.rglob('*.pc'):
        raw=p.read_bytes();dest=O/'snapshots/pkgconfig'/r['name']/p.name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(raw)
        pcs.append(dict(package=r['name'],family=r['family'],rpm_sha256=r['checksum'],path='/'+str(p.relative_to(root)),sha256=sha(p),snapshot=str(dest.relative_to(O))))
with (O/'tables/pkgconfig.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(pcs[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(pcs)
print(json.dumps(dict(elfs=len(result),pkgconfig_files=len(pcs))))
