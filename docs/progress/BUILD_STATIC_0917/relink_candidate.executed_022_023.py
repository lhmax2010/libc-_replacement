import hashlib,json,shlex,sys,time
from pathlib import Path
from probes import base,run,inside,p

build='/tmp/night/builds/bpftrace-x86_64-static'
if sys.argv[1]=='prepare':
    r=run('ninja-recorded-commands',base+['--','/tmp/night/tools/ninja','-C',build,'-t','commands','bpftrace'])
    assert r['exitcode']==0
    lines=[x for x in r['stdout'].splitlines() if ' -o src/bpftrace ' in x and '-static-libgcc' in x]
    assert len(lines)==1
    original=lines[0]
    tokens=shlex.split(original)
    # Ninja's generated shell rule is ': && <driver ...> && :'.
    assert tokens[:2]==[':', '&&'] and tokens[-2:]==['&&', ':']
    tokens=tokens[2:-2]
    assert tokens.count('-static-libgcc')==1
    revised=[x for x in tokens if x!='-static-libgcc']
    output=inside+'/bpftrace-dynamic-libgcc'
    revised[revised.index('-o')+1]=output
    maps=[i for i,x in enumerate(revised) if x.startswith('-Wl,-Map,')]
    assert len(maps)==1
    revised[maps[0]]='-Wl,-Map,'+output+'.map'
    (p/'relink_plan.json').write_text(json.dumps(dict(original_shell=original,original_tokens=tokens,new_tokens=revised,working_directory=build,changes=['remove -static-libgcc','new output and map paths only'],input_tree_read_only=True),indent=2))
    # Object identity before linking; no input compilation or recompilation is needed.
    root=Path('tmp/NIGHT_0917/builds/bpftrace-x86_64-static')
    identities=[]
    for x in tokens:
        if x.endswith(('.o','.a')) and not x.startswith('/'):
            f=root/x
            if f.is_file(): identities.append(dict(path=str(f),sha256=hashlib.sha256(f.read_bytes()).hexdigest(),bytes=f.stat().st_size))
    (p/'reused_object_identities.json').write_text(json.dumps(identities,indent=2))
elif sys.argv[1]=='build':
    plan=json.loads((p/'relink_plan.json').read_text())
    for attempt in range(4):
        r=run('link-io-before-'+str(attempt),['python3','progress/BUILD_STATIC_0917/io_probe.py'],35)
        if r['exitcode']==0: break
        if attempt==3: raise SystemExit(30)
        print('I/O waiting 600 seconds, no build running',flush=True)
        for _ in range(60): time.sleep(10)
    r=run('relink-candidate',base+['--chdir',plan['working_directory'],'--']+plan['new_tokens'],3600)
    if r['exitcode']!=0: raise SystemExit(1)
    run('link-io-after',['python3','progress/BUILD_STATIC_0917/io_probe.py'],35)
else: raise SystemExit(2)
