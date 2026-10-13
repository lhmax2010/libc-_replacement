"""Preserve failed cereal evidence and publish only remotely observed R5 entries."""
import csv,hashlib,json,pathlib,shutil,subprocess,sys
R=pathlib.Path.cwd();E=R/'docs/progress/QB_1194606_FIX';T=R/'tmp/QB_1194606_FIX'
out=E/'failure-cereal_R3';out.mkdir(exist_ok=False)
log=E/'build-cereal-attempt2_R3/build.log'
lines=log.read_text().splitlines()
first=next(i for i,x in enumerate(lines) if 'error: static assertion failed' in x)
(out/'FIRST_ERROR.txt').write_text('\n'.join(f'{i+1}: {lines[i]}' for i in range(max(0,first-40),min(len(lines),first+41)))+'\n')
b=T/'GBS-ROOT/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/BUILD/cereal-1.3.2'
files=[b/'sandbox/performance.cpp',b/'build/CMakeCache.txt',b/'build/sandbox/CMakeFiles/performance.dir/flags.make',b/'build/sandbox/CMakeFiles/performance.dir/link.txt',T/'repos/cereal/packaging/cereal.spec',T/'gbs.conf',T/'repomd.xml']
identities=[]
for p in files:
    data=p.read_bytes(); dest=out/p.name;assert not dest.exists();dest.write_bytes(data)
    identities.append(dict(source=str(p),copy=str(dest),sha256=hashlib.sha256(data).hexdigest(),size=len(data),mtime_ns=p.stat().st_mtime_ns))
(out/'IDENTITIES.json').write_text(json.dumps(identities,indent=2)+'\n')
rows=list(csv.DictReader((R/'docs/progress/LLVM_W4_0923/QUICKBUILD_BRANCHES_R4.tsv').open(),delimiter='\t'))
results=[]
for i,row in enumerate(rows):
    argv=['git','ls-remote','ssh://lhmax2025@review.tizen.org:29418/'+row['gerrit_repository'],'refs/heads/'+row['branch']]
    label=f'R3_branch_{i+1:02d}'
    r=subprocess.run([sys.executable,str(E/'run.py'),label,*argv],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    print(r.stdout.decode(),flush=True)
    raw=(E/'raw'/f'{label}.stdout.txt').read_text().strip()
    actual=raw.split()[0] if r.returncode==0 and raw else 'NOT_AVAILABLE'
    results.append(dict(package=row['obs_package'],expected=row['remote_sha'],actual=actual,result='MATCH' if actual==row['remote_sha'] else 'MISMATCH'))
    if actual!=row['remote_sha']:raise RuntimeError(results[-1])
with (E/'QUICKBUILD_BRANCHES_R5.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['package','repo','branch','commit'])
    for row in rows:w.writerow([row['obs_package'],row['gerrit_repository'],row['branch'],row['remote_sha']])
(E/'BRANCH_CHECK_R3.json').write_text(json.dumps(results,indent=2)+'\n')
print('R5 retains 12 verified remote rows; no failed/unvalidated candidate added.')
