import hashlib,json,re,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); root=Path('tmp/NIGHT_0917/builds'); results={}; commands=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True); commands.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)); assert r.returncode==0,r.stderr; return r.stdout
for arch in ['x86_64','aarch64']:
    d=root/f'libclang-{arch}-libcxx'; a=d/'lib64/libclang.a'; assert a.is_file()
    s=run(['strings',str(a)]); names=run(['nm','-g',str(a)])
    row=dict(path=str(a),bytes=a.stat().st_size(),sha256=hashlib.sha256(a.read_bytes()).hexdigest(),exact_std1_lines=sum('_ZNSt3__1' in l for l in s.splitlines()),exact_cxx11_lines=sum('_ZNSt7__cxx11' in l for l in s.splitlines()),broad_cxx11_lines=sum('__cxx11' in l for l in names.splitlines()))
    assert row['exact_std1_lines']>0 and row['exact_cxx11_lines']==0 and row['broad_cxx11_lines']==0
    cache=(d/'CMakeCache.txt').read_text(); assert 'LIBCLANG_BUILD_STATIC:BOOL=ON' in cache
    assert '-stdlib=libc++' in (d/'build.ninja').read_text(); results[arch+'-libcxx']=row
d=root/'libclang-x86_64-gcc'; assert not (d/'lib64/libclang.a').exists()
libs=[x for x in (d/'lib64').glob('libclang.so*') if x.is_file() and not x.is_symlink()]; assert len(libs)==1,libs
s=run(['readelf','-d',str(libs[0])]); needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',s)
assert 'libstdc++.so.6' in needed and 'libc++.so.1' not in needed and 'libc++abi.so.1' not in needed
flags=(d/'build.ninja').read_text(); assert '-stdlib=libc++' not in flags and '-lc++abi' not in flags
assert 'LIBCLANG_BUILD_STATIC:BOOL=OFF' in (d/'CMakeCache.txt').read_text()
results['x86_64-gcc']=dict(path=str(libs[0]),sha256=hashlib.sha256(libs[0].read_bytes()).hexdigest(),needed=needed,no_libclang_a=True,no_forbidden_flags=True)
(p/'libclang_verification_commands.json').write_text(json.dumps(commands,indent=2))
(p/'libclang_result.json').write_text(json.dumps(results,indent=2)); print(json.dumps(results,indent=2))
