import json
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; task=p/'tmp/BPF_W1_0921'; old=p/'tmp/WEEKEND_0918'
(out/'cell_commands').mkdir(exist_ok=True)
(out/'deadlines.json').write_text(json.dumps({'W1':'2026-09-22T05:01:00+08:00'}))
changes=[]
for arch in ('armv7l','aarch64'):
    (out/'cell_commands'/f'prepare-{arch}.json').write_text(json.dumps(['/usr/bin/python3',str(out/'prepare.py'),arch],indent=2))
    for mode in ('libcxx','gcc','undefined'):
        label=f'bpf-{arch}-{mode}'
        suffix='-checkers' if mode=='libcxx' and arch=='aarch64' else '-linkerfixed' if mode=='gcc' else '-harness' if mode=='undefined' else ''
        src=p/f'progress/BUILD_WEEKEND_0918/cell_commands/{label}{suffix}.json'
        before=json.loads(src.read_text()); after=[x.replace(str(old),str(task)).replace('_binary_payload w5T1.xzdio','_binary_payload w5.xzdio') for x in before]
        assert '_binary_payload w5.xzdio' in after
        for name in ('BUILD','BUILDROOT','RPMS','SOURCES','SPECS','SRPMS','TMP'):
            (task/f'rpm-{label}'/name).mkdir(parents=True,exist_ok=False)
        (out/'cell_commands'/f'{label}.json').write_text(json.dumps(after,indent=2))
        changes.append(dict(label=label,source=str(src),diff=[dict(index=i,before=a,after=b) for i,(a,b) in enumerate(zip(before,after)) if a!=b]))
(out/'argv-differences.json').write_text(json.dumps(changes,indent=2))
print('six commands; private task paths and payload only; unchanged specs will be copied')
