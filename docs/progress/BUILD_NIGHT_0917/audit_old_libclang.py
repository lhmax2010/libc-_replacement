import hashlib,json,re,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); previous=Path('progress/BUILD_NIGHT_0916/static_packages_v2.json')
old=json.loads(previous.read_text()); rows=[]; commands=[]
def run(cmd,allowed=(0,)):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace'); commands.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)); assert r.returncode in allowed,r.stderr; return r.stdout
for x in old['members']:
    if 'local_path' not in x: continue
    f=Path(x['local_path']); digest=hashlib.sha256(f.read_bytes()).hexdigest(); assert digest==x['sha256']
    members=run(['ar','t',str(f)]).splitlines(); comment=run(['readelf','-p','.comment',str(f)])
    symbols=run(['nm','-u',str(f)]); header=run(['readelf','-h',str(f)])
    row=dict(path=x['path'],local_path=str(f),bytes=f.stat().st_size,sha256=digest,members=members,member_count=len(members),gcc_comment_lines=sum('GCC: (Tizen GCC 14.2.0 20240801 2.1) 14.2.0' in s for s in comment.splitlines()),comment_values=sorted(set(s.strip() for s in comment.splitlines() if 'GCC:' in s or 'clang version' in s)),undefined_cxx11_lines=sum('__cxx11' in s for s in symbols.splitlines()),undefined_std1_lines=sum('St3__1' in s for s in symbols.splitlines()),machine=sorted(set(s.strip() for s in header.splitlines() if 'Machine:' in s)),version_basis='archive filename plus introduction commit description')
    assert row['gcc_comment_lines']==len(members) and row['undefined_cxx11_lines']>0 and row['undefined_std1_lines']==0
    rows.append(row)
repo='tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'
# Verify the exact blob recorded on both the introducing and inspected commits.
blobs={}
for rev in ['84440faa935cc2650bf84e3204550016d7d21db8','624f68d5f67a673083e5ad08ea205742613d6e53']:
    cmd=['git','-C',repo,'show',rev+':packaging/prebuilt.tar.gz']
    r=subprocess.run(cmd,capture_output=True); assert r.returncode==0
    digest=hashlib.sha256(r.stdout).hexdigest(); assert digest==old['prebuilt_tar_sha256']; blobs[rev]=digest
    commands.append(dict(command=cmd,exitcode=r.returncode,stdout_binary_sha256=digest,stdout_bytes=len(r.stdout),stderr=r.stderr.decode(errors='replace')))
(p/'old_libclang_commands.json').write_text(json.dumps(commands,indent=2))
(p/'old_libclang_result.json').write_text(json.dumps(dict(records=rows,tar_revision_hashes=blobs,source_of_reused_metadata=str(previous),producer_recipe_candidate='llvm sandbox/hoegeun/llvm-static @ 46df8cd0d7cb564af6e1e482f3c48d04dba3079b',exact_recipe_to_payload_link='NOT_OBSERVED'),indent=2))
print(json.dumps(rows,indent=2))
