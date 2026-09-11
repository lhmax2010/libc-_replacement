#!/usr/bin/env python3
"""只读复用冻结材料，生成经确认范围的路径清单与 Native 快速扫描。"""
import csv, gzip, hashlib, json, pathlib, sys, re
ROOT=pathlib.Path.cwd(); P=ROOT/'docs/progress/P7_0909'; O=P/'stage2'
sys.path.insert(0,str(P))
from scan_scope import shape
CACHE=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11/extracted')
CORPUS=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/corpus/extracted')
def read(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def write(name,rows):
    p=O/'tables'/name;p.parent.mkdir(parents=True,exist_ok=True)
    with p.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(rows)
families=read(P/'tables/STAGE2_RECOMMENDED.tsv')
srpm_family={s:r for r in families for s in r['source_rpm_records'].split(';')}
sources=[]
for r in families:
    for srpm in r['source_rpm_records'].split(';'):
        base=CORPUS/srpm
        dirs=list((base/'unpacked').iterdir()) if (base/'unpacked').is_dir() else []
        sources.append(dict(group=r['group'],family=r['source_name'],srpm=srpm,source_roots=';'.join(map(str,dirs)),payload=str(base/'payload') if (base/'payload').is_dir() else 'NOT_AVAILABLE',status='AVAILABLE' if dirs else 'NOT_AVAILABLE'))
write('source_locations.tsv',sources)
packages=[r for r in read(P/'tables/provider_scope.tsv') if r['sourcerpm'] in srpm_family]
for r in packages:
    r['family']=srpm_family[r['sourcerpm']]['source_name'];r['group']=srpm_family[r['sourcerpm']]['group']
write('packages.tsv',packages)
bysha={r['checksum']:r for r in packages}
sdk=read(P/'tables/sdk_header_path_matches.tsv');matched={}
for r in sdk:matched.setdefault((r['rpm_sha256'],r['path']),[]).append(r)
headers=[];native=[];snippets=[]
with gzip.open(P/'tables/headers.tsv.gz','rt') as f:
    for r in csv.DictReader(f,delimiter='\t'):
        keysha=r.get('rpm_sha256',r.get('checksum',''))
        if keysha not in bysha:continue
        package=bysha[keysha];path=r.get('path',r.get('installed_path',''))
        if not path.startswith('/usr/include/'):continue
        r.update(family=package['family'],group=package['group'],local_path=str(CACHE/keysha[:2]/keysha/path.lstrip('/')))
        matches=matched.get((keysha,path),[])
        r['public_recipes']=';'.join(sorted({x['group'] for x in matches if x['kind']=='PUBLIC'}))
        r['internal_recipes']=';'.join(sorted({x['group'] for x in matches if x['kind']=='INTERNAL'}))
        headers.append(r)
        if package['group']=='NATIVE_APP_MIDDLEWARE' and r['public_recipes']:
            file=pathlib.Path(r['local_path']);data=file.read_bytes()
            assert hashlib.sha256(data).hexdigest()==r['file_sha256'],path
            form=shape(data.decode(errors='replace'),path)
            assert form['form']==r['form'],(path,form,r['form'])
            snap=O/'snapshots/native'/package['name']/path.lstrip('/');snap.parent.mkdir(parents=True,exist_ok=True);snap.write_bytes(data)
            native.append(dict(family=package['family'],package=package['name'],path=path,public_recipes=r['public_recipes'],form=r['form'],cpp_signal=r['cpp_signal'],file_sha256=r['file_sha256'],snapshot=str(snap.relative_to(O))))
            hits=[]
            for n,line in enumerate(data.decode(errors='replace').splitlines(),1):
                if re.search(r'extern\s+"C"|namespace\b|\bclass\b|std::|\bthrow\b|\btemplate\s*<',line):hits.append(f'{n}: {line}')
            snippets.append(f'===== {package["family"]} {path} =====\n'+'\n'.join(hits)+'\n')
assert headers and native,'schema mismatch or empty selection'
write('headers.tsv',headers);write('native_public_scan.tsv',native)
(O/'tables/native_signal_context.txt').write_text('\n'.join(snippets))
print(json.dumps(dict(packages=len(packages),headers=len(headers),native_public_headers=len(native),native_families=len({r['family'] for r in native})),ensure_ascii=False))
