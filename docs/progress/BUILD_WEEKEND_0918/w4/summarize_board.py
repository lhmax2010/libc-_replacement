"""Derive board tables from actual commands and repetitions."""
import csv,hashlib,json,re,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'
data=json.loads((out/'board_cancel/results.json').read_text())
assert len(data)==40
with (out/'BOARD_CANCEL_MATRIX.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t'); w.writerow(['platform','link','mode','runs','passes','remote_exitcodes'])
    for link in ('shared-gcc','static-gcc'):
        for mode in ('wait','system_clock','steady_clock','custom_clock'):
            subset=[r for r in data if r['link']==link and r['mode']==mode]
            assert sorted(r['repeat'] for r in subset)==[1,2,3,4,5]
            w.writerow(['物理板',link,mode,len(subset),sum(r['assertions_pass'] for r in subset),','.join(str(r['remote_exitcode']) for r in subset)])
with (out/'BOARD_ROOT_OPERATIONS.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t'); w.writerow(['record','index','root_mode_before','command','transport_exitcode','remote_exitcode'])
    for location in ('board_cancel','board-install-original-only'):
        root=False
        for i,row in enumerate(json.loads((out/location/'commands.json').read_text())):
            cmd=row['command']; change=bool(re.search(r'\broot (on|off)$',cmd))
            if root or change:
                text=row.get('stdout','')
                if location=='board-install-original-only' and text: text=(out/location/text).read_text(errors='replace')
                codes=re.findall(r'^TASK_REMOTE_RC=(\d+)$',text.replace('\r\n','\n'),re.M)
                w.writerow([location,i,root,cmd,row['exitcode'],codes[0] if len(codes)==1 else 'NOT_APPLICABLE_OR_NOT_OBSERVED'])
            if change and row['exitcode']==0: root=cmd.endswith(' on')
        assert not root
dest=out/'board-probe-evidence'; dest.mkdir(exist_ok=False)
commands=[]
for mode in ('shared-gcc','static-gcc'):
    binary=p/'tmp/WEEKEND_0918/board-probes'/('cancel-'+mode)
    for label,cmd in [('symbols',['readelf','-Ws',str(binary)]),('dynamic',['readelf','-dW',str(binary)])]:
        r=subprocess.run(cmd,capture_output=True)
        (dest/(mode+'-'+label+'.txt')).write_bytes(r.stdout)
        commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stderr':r.stderr.decode(errors='replace')})
        assert r.returncode==0
    source=binary.with_suffix('.map'); (dest/(mode+'.map')).write_bytes(source.read_bytes())
source=p/'docs/progress/BUILD_STATIC_0917/inputs/condition_cancel_probe.cpp'
(dest/source.name).write_bytes(source.read_bytes())
(dest/'commands.json').write_text(json.dumps(commands,indent=2))
(dest/'SHA256.json').write_text(json.dumps({x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in dest.iterdir() if x.is_file()},indent=2))
print('40 repetitions; root-operation ledger; exact probe source, maps, ELF evidence saved')
