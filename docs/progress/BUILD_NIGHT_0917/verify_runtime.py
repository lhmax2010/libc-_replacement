import hashlib,json,subprocess,re,sys
from pathlib import Path
out=Path('progress/BUILD_NIGHT_0917'); arch=sys.argv[1]; modes=sys.argv[2:] or ['libcxx','gcc']
records=[]
def capture(cmd,label):
    r=subprocess.run(cmd,capture_output=True,text=True)
    (out/(label+'.txt')).write_text(r.stdout)
    with (out/'runtime_verification_commands.jsonl').open('a') as f: f.write(json.dumps(dict(command=cmd,exitcode=r.returncode,stderr=r.stderr))+'\n')
    assert r.returncode==0; return r.stdout
def symbols(p,label):
    text=capture(['readelf','--dyn-syms','--wide',str(p)],label)
    return sorted(tuple(l.split()[2:]) for l in text.splitlines() if re.match(r'\s*\d+:',l))
def needed(p,label):
    text=capture(['readelf','-d',str(p)],label)
    return sorted(re.findall(r'\(NEEDED\).*?\[(.*?)\]',text))
base=Path('tmp/NIGHT_0917/builds')/f'runtime-{arch}-baseline'/'lib'
for mode in modes:
    lib=Path('tmp/NIGHT_0917/builds')/f'runtime-{arch}-{mode}'/'lib'
    record=dict(arch=arch,mode=mode,shared=[])
    for name in ['libc++.so.1.0','libc++abi.so.1.0']:
        a=base/name; b=lib/name; tag=f'{arch}-{mode}-{name}'
        sa=symbols(a,tag+'-baseline-symbols'); sb=symbols(b,tag+'-candidate-symbols')
        da=needed(a,tag+'-baseline-needed'); db=needed(b,tag+'-candidate-needed')
        row=dict(name=name,baseline_sha256=hashlib.sha256(a.read_bytes()).hexdigest(),candidate_sha256=hashlib.sha256(b.read_bytes()).hexdigest(),dynsym_equal=sa==sb,dynsym_count=len(sa),needed_equal=da==db,needed=da)
        row['symbol_only_baseline']=[x for x in sa if x not in sb]; row['symbol_only_candidate']=[x for x in sb if x not in sa]
        record['shared'].append(row)
    record['static_present']={name:(lib/name).is_file() for name in ['libc++.a','libc++abi.a']}
    record['passed']=all(r['dynsym_equal'] and r['needed_equal'] for r in record['shared']) and all(v==(mode=='libcxx') for v in record['static_present'].values())
    records.append(record)
(out/f'{arch}_runtime_comparison.json').write_text(json.dumps(records,indent=2))
print(json.dumps(records,indent=2))
assert all(r['passed'] for r in records)
