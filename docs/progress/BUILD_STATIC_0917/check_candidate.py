import difflib, hashlib,json,re,shutil,subprocess
from pathlib import Path
from probes import base,run,p,t,inside

exe=t/'bpftrace-dynamic-libgcc'
old=Path('tmp/NIGHT_0917/builds/bpftrace-x86_64-static/src/bpftrace')
needed=run('candidate-needed',base+['--','readelf','-d',inside+'/'+exe.name])
deps=re.findall(r'Shared library: \[(.*?)\]',needed['stdout'])
assert 'libgcc_s.so.1' in deps
assert not any(x.startswith(('libc++.','libc++abi.','libstdc++.','libLLVM','libclang','libbcc')) for x in deps)
assert set(deps)<=set(['libgcc_s.so.1','libc.so.6','libm.so.6','libpthread.so.0','libdl.so.2','librt.so.1','ld-linux-x86-64.so.2'])
comparisons=[]
for option in ['--version','--help']:
    oldr=run('old-candidate-'+option[2:],base+['--','/tmp/night/builds/bpftrace-x86_64-static/src/bpftrace',option])
    newr=run('candidate-'+option[2:],base+['--',inside+'/'+exe.name,option])
    comparisons.append(dict(option=option,old_exitcode=oldr['exitcode'],new_exitcode=newr['exitcode'],stdout_equal=oldr['stdout']==newr['stdout'],stderr_equal=oldr['stderr']==newr['stderr'],diff=list(difflib.unified_diff(oldr['stdout'].splitlines(),newr['stdout'].splitlines()))))
    assert newr['exitcode']==oldr['exitcode']==0
nm=run('candidate-unwind-symbols',['nm',str(exe)])
nm_path=p/'candidate-unwind-symbols.json'
# Full nm is evidence retained locally; a compact indexed derivative is published.
matches={q:[x for x in nm['stdout'].splitlines() if q in x] for q in ['_Unwind_ForcedUnwind','_Unwind_SetGR','__gxx_personality_v0','__gcc_personality_v0','__forced_unwind','pthread_cancel','pthread_setcanceltype']}
cmd=['strings','-a',str(exe)]
r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=90)
counts={q:sum(q in x for x in r.stdout.splitlines()) for q in ['_ZNSt3__1','_ZNSt7__cxx11']}
(p/'candidate-string-counts.json').write_text(json.dumps(dict(command=cmd,exitcode=r.returncode,counts=counts),indent=2))
assert counts['_ZNSt3__1']>0 and counts['_ZNSt7__cxx11']==0
mapfile=t/'bpftrace-dynamic-libgcc.map'
lines=mapfile.read_text(errors='replace').splitlines()
hits=[x for x in lines if any(y in x for y in ['libgcc_eh','libgcc_s','libunwind','libc++abi.a(cxa_personality','libomp','liblldb'])]
(p/'candidate-map-excerpts.txt').write_text('\n'.join(hits)+'\n')
assert not any('libgcc_eh.a(' in x for x in lines)
assert not any('libunwind.a(' in x for x in lines)
assert any('libc++abi.a(cxa_personality' in x for x in lines)
stripped=t/'bpftrace-dynamic-libgcc.stripped'
shutil.copy2(exe,stripped)
s=run('candidate-strip',base+['--','eu-strip','--remove-comment','-f',inside+'/'+stripped.name+'.debug',inside+'/'+stripped.name],180)
assert s['exitcode']==0
run('candidate-stripped-version',base+['--',inside+'/'+stripped.name,'--version'])
sizes=dict(unstripped_bytes=exe.stat().st_size,stripped_bytes=stripped.stat().st_size,previous_unstripped_bytes=old.stat().st_size,previous_stripped_bytes=Path('tmp/NIGHT_0917/finish_validation/bpftrace-x86_64.stripped').stat().st_size)
(p/'candidate_checks.json').write_text(json.dumps(dict(needed=deps,cli_comparisons=comparisons,unwind_symbols=matches,sizes=sizes,sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),stripped_sha256=hashlib.sha256(stripped.read_bytes()).hexdigest(),old_candidate_sha256=hashlib.sha256(old.read_bytes()).hexdigest(),only_semantic_link_change='remove -static-libgcc',rebuild_kind='relink existing objects, not a fresh compilation'),indent=2))
