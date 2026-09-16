import hashlib, json, subprocess
from pathlib import Path
from probes import p, run

summary=[]
for arch in ['aarch64','armv7l']:
    binary=Path(f'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/{arch}/usr/bin/bpftrace')
    row=dict(arch=arch, path=str(binary),sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),size=binary.stat().st_size)
    for tool,args in [('file',[]),('readelf',['-d']),('readelf',['-Ws']),('nm',[])]:
        r=run('original-'+arch+'-'+tool+('-'+args[0].lstrip('-') if args else ''),[tool]+args+[str(binary)])
    cmd=['strings','-a',str(binary)]
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=90)
    needles=['_Unwind_ForcedUnwind','_Unwind_SetGR','__gcc_personality_v0','__gxx_personality_v0','libgcc_s','_ZNSt3__1','_ZNSt7__cxx11','clang version','bpftrace v','pthread_cancel','pthread_setcanceltype']
    hits={q:[x for x in r.stdout.splitlines() if q in x] for q in needles}
    (p/('original-'+arch+'-strings.json')).write_text(json.dumps(dict(command=cmd,exitcode=r.returncode,matching_lines=hits,stderr=r.stderr),indent=2))
    row['string_match_counts']={q:len(x) for q,x in hits.items()}
    summary.append(row)
(p/'original_summary.json').write_text(json.dumps(summary,indent=2))
for label,root in [('bpftrace','tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'),('bcc','codes/R104/bcc-tools'),('llvm','tmp/BUILD_W1BC_0911/target-fetch')]:
    run(label+'-source-identity',['git','-C',root,'rev-parse','HEAD'])
    run(label+'-source-status',['git','-C',root,'status','--short'])
    # Search text source, build scripts and tests, not archives or Git object databases.
    run(label+'-cancel-search',['rg','-n','--glob','!.git/**','--glob','!*.patch','pthread_cancel|pthread_setcanceltype',root],timeout=180)
