"""Post-build checks, with command/exit records and exact checker snapshots."""
import hashlib,json,shlex,subprocess,sys
from pathlib import Path
arch,mode,cell=sys.argv[1:]; out=Path('progress/BUILD_WEEKEND_0918')
dest=out/f'checks-{arch}-{mode}'; dest.mkdir(exist_ok=False); records=[]
for script,args in [('verify_bpf_rpms.py',[arch,mode]),('audit_bpf_cell.py',[arch,mode,cell]),('check_actual_hal.py',[arch,mode])]:
    f=out/script; snapshot=dest/script; snapshot.write_bytes(f.read_bytes())
    cmd=['python3',str(f),*args]
    print('COMMAND '+shlex.join(cmd),flush=True)
    r=subprocess.run(cmd)
    records.append({'command':shlex.join(cmd),'exitcode':r.returncode,'snapshot_sha256':hashlib.sha256(snapshot.read_bytes()).hexdigest()})
    (dest/'commands.json').write_text(json.dumps(records,indent=2))
    print('EXITCODE',r.returncode,flush=True)
    if r.returncode: raise SystemExit(r.returncode)
print('BUILD_PAYLOAD_AND_MACRO_CHECKS_PASS_NOT_BOARD_VALIDATED',flush=True)
