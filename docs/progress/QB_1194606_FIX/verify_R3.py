"""Verify real GBS outputs before any sandbox push; retain all commands and results."""
import difflib, hashlib, json, pathlib, shutil, subprocess, sys, shlex
ROOT=pathlib.Path(__file__).resolve().parents[3]
E=ROOT/'docs/progress/QB_1194606_FIX';T=ROOT/'tmp/QB_1194606_FIX'
pkg=sys.argv[1];out=E/('verify-'+pkg+'_R3');out.mkdir(exist_ok=False)
counter=0
def run(argv):
    global counter
    counter+=1;p=out/f'{counter:03d}'
    p.with_suffix('.argv.json').write_text(json.dumps(argv,indent=2)+'\n')
    r=subprocess.run(argv,capture_output=True)
    p.with_suffix('.stdout').write_bytes(r.stdout);p.with_suffix('.stderr').write_bytes(r.stderr)
    p.with_suffix('.exitcode').write_text(str(r.returncode)+'\n')
    assert r.returncode==0,(argv,r.stderr.decode(errors='replace'))
    return r.stdout.decode()
def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
repo=T/'GBS-ROOT/local/repos/qb1194606/x86_64/RPMS'
rpms=[p for p in repo.rglob('*.rpm') if not p.name.endswith('.src.rpm') and ((pkg=='cereal' and p.name.startswith('cereal-')) or (pkg=='icu' and (p.name.startswith('libicu-') or p.name.startswith('icu-'))) or (pkg=='qemu-accel' and 'accel-armv7l-' in p.name))]
assert rpms, str(repo)
records=[]
for p in sorted(rpms):
    run(['rpm','-K','--nosignature',str(p)])
    files=run(['rpm','-qlp',str(p)]).splitlines()
    records.append(dict(path=str(p),sha256=sha(p),bytes=p.stat().st_size,files=files))
(out/'RPM_INVENTORY.json').write_text(json.dumps(records,indent=2)+'\n')
result={}
buildroot=T/'GBS-ROOT/local/BUILD-ROOTS/scratch.x86_64.0'
if pkg=='cereal':
    perf=list((buildroot/'home/abuild/rpmbuild/BUILD').glob('cereal-*/build/unittests/performance'))
    if not perf:perf=[p for p in (buildroot/'home/abuild/rpmbuild/BUILD').rglob('performance') if p.is_file()]
    assert len(perf)==1,perf
    dst=T/'verified/cereal/performance';dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(perf[0],dst)
    dyn=run(['readelf','-d',str(dst)])
    assert '[libc++.so.1]' in dyn and '[libstdc++.so.6]' not in dyn
    candidate=next(r for r in records if pathlib.Path(r['path']).name.startswith('cereal-devel-'))
    old=run(['rpm','-qlp',str(T/'preQB-cereal-devel.rpm')]).splitlines()
    before=sorted(x for x in old if x.startswith('/usr/include/'))
    after=sorted(x for x in candidate['files'] if x.startswith('/usr/include/'))
    (out/'headers.diff').write_text(''.join(difflib.unified_diff(before,after,lineterm='\n')))
    assert before==after and before
    result.update(performance_path=str(dst),performance_sha256=sha(dst),headers_count=len(after),headers_equal=True)
elif pkg=='icu':
    log=(E/'build-icu_R3/build.log').read_text(errors='replace')
    lines=[x for x in log.splitlines() if 'tz2icu.cpp' in x and '-iquote' in x and not x.lstrip().startswith('+ echo')]
    assert lines
    (out/'tz2icu-command.txt').write_text('\n'.join(lines)+'\n')
    library_rpm=next(p for p in rpms if run(['rpm','-qp','--qf','%{NAME}',str(p)])=='libicu')
    extract=T/'verified/icu';extract.mkdir(parents=True,exist_ok=False)
    run(['bash','-o','pipefail','-c','cd '+shlex.quote(str(extract))+' && rpm2cpio '+shlex.quote(str(library_rpm))+' | cpio -idm --quiet --no-absolute-filenames'])
    libs=[p for p in (extract/'usr/lib64').glob('libicu*.so.*') if not p.is_symlink()]
    assert libs
    checks=[]
    for p in libs:
        dyn=run(['readelf','-d',str(p)])
        checks.append(dict(file=str(p),sha256=sha(p),libcxx='[libc++.so.1]' in dyn,libstdcxx='[libstdc++.so.6]' in dyn))
    # Data-only shared objects may have no C++ runtime dependency.
    assert any(x['libcxx'] for x in checks) and not any(x['libstdcxx'] for x in checks)
    result['libraries']=checks
else:
    record=next(r for r in records if pathlib.Path(r['path']).name.startswith('clang-accel-armv7l-'))
    required=['/emul/usr/lib64/libc++.so.1','/emul/usr/lib64/libc++abi.so.1']
    assert all(x in record['files'] for x in required)
    log=(E/'build-qemu-accel_R3/build.log').read_text(errors='replace')
    assert 'Installed (but unpackaged) file(s) found' not in log
    result.update(required_files=required,unpackaged_error=False)
result['result']='PASS'
(out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
