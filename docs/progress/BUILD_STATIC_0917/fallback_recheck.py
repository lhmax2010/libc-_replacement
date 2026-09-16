import hashlib,json,shlex
from pathlib import Path
from probes import p,t,base,run,inside

config=Path('codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config')
macro=next(x[1:] for x in config.read_text().splitlines() if x.startswith('%toolchain_is() '))
result=[]
for mode in ['gcc','undefined']:
    texts={}
    for version,file in [('baseline','bpftrace.baseline.spec'),('candidate','bpftrace.spec')]:
        c=base+['--','rpmspec','-P','--target','aarch64','--define',macro,'--define','_smp_mflags -j1']
        if mode!='undefined': c+=['--define','_toolchain '+mode]
        c+=['/tmp/night/specs/'+file]
        r=run('fallback-'+mode+'-'+version+'-expansion',c)
        assert r['exitcode']==0
        texts[version]=r['stdout']
    normalize=lambda s:'\n'.join(x for x in s.splitlines() if x.strip())
    assert normalize(texts['baseline'])==normalize(texts['candidate'])
    assert '-stdlib=libc++' not in texts['candidate'] and '-lc++abi' not in texts['candidate']
    installs=[x for x in texts['candidate'].splitlines() if x.startswith('install -m 0755 prebuilt-static-bpftrace/')]
    assert len(installs)==1
    args=shlex.split(installs[0]); source=args[-2]
    assert source=='prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace'
    dest=t/('fallback-'+mode); dest.mkdir(exist_ok=True)
    r=run('fallback-'+mode+'-install',base+['--','install','-m','0755','/tmp/night/original_assets/'+source,inside+'/'+dest.name+'/bpftrace-static'])
    assert r['exitcode']==0
    original=Path('tmp/NIGHT_0917/original_assets')/source
    previous=Path('tmp/NIGHT_0917')/('fallback-'+mode)/'usr/bin/bpftrace-static'
    installed=dest/'bpftrace-static'
    assert installed.read_bytes()==original.read_bytes()==previous.read_bytes()
    result.append(dict(mode=mode,expanded_specs_equal=True,forbidden_flags=0,new_install_sha256=hashlib.sha256(installed.read_bytes()).hexdigest(),source1002_asset_equal=True,previous_verified_install_equal=True,rebuild_this_task=False,previous_build_record='docs/progress/BUILD_NIGHT_0917/fallback_result.json'))
(p/'fallback_recheck.json').write_text(json.dumps(result,indent=2))
