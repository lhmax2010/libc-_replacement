"""Prepare a single preload experiment. Host successful-call capture needs approval."""
import argparse,hashlib,json,pathlib,shutil
p=argparse.ArgumentParser();p.add_argument('--record-successful-host-calls',action='store_true');o=p.parse_args()
assert not o.record_successful_host_calls, 'User confirmed original host strace filter must remain unchanged'
base=pathlib.Path('progress/ARM_LZMA_0921');work=pathlib.Path('tmp/ARM_LZMA_0921').resolve()
assert json.loads((base/'previous-preserved-manifest.json').read_text())['all_verified']
source=pathlib.Path('progress/ARM_REPRO_0920/original-argv.json')
old=json.loads(source.read_text());cmd=old.copy()
cmd[cmd.index('-o')+1]=str((base/'host-instrumented.strace').resolve())
if o.record_successful_host_calls:
    i=cmd.index('status=failed');assert cmd[i-1]=='-e';del cmd[i-1:i+1]
i=cmd.index('--')
cmd[i:i]=['--bind',str(work),'/tmp/lzma-diag']
i=cmd.index('--')+1
cmd[i:i]=['/usr/bin/env','LD_PRELOAD=/tmp/lzma-diag/lzma_tap.so','LZMA_TAP_PREFIX=/tmp/lzma-diag/rpmbuild-lzma']
assert '-vv' not in cmd and '-strace' not in cmd
check=cmd.copy();i=check.index('/usr/bin/env');del check[i:i+3]
i=check.index(str(work));assert check[i-1]=='--bind';del check[i-1:i+2]
check[check.index('-o')+1]=old[old.index('-o')+1]
if o.record_successful_host_calls:
    i=check.index('-s');check[i:i]=['-e','status=failed']
assert check==old
assert not (work/'rpmbuild-lzma.calls.tsv').exists()
spec=pathlib.Path('tmp/WEEKEND_0918/specs/llvm.spec')
assert hashlib.sha256(spec.read_bytes()).hexdigest()=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
(base/'cell_commands').mkdir(exist_ok=True)
(base/'cell_commands/preload-original.json').write_text(json.dumps(cmd,indent=2)+'\n')
(base/'deadlines.json').write_text(json.dumps({'W1':'2026-09-21T17:20:00+08:00'},indent=2)+'\n')
(base/'original-argv.json').write_bytes(source.read_bytes())
snap=base/'script-snapshots';snap.mkdir(exist_ok=True)
paths=[base/n for n in ('guard.py','capture_failure.py','prepare.py','guest.py','calibrate.py','preserve_previous.py')]
paths += [work/n for n in ('lzma_tap.c','lzma_tap.so','tap_calibration.c','tap_calibration')]
rows=[]
for src in paths:
    dst=snap/src.name;shutil.copy2(src,dst)
    rows.append({'source':str(src),'snapshot':str(dst),'sha256':hashlib.sha256(dst.read_bytes()).hexdigest()})
(snap/'SHA256.json').write_text(json.dumps(rows,indent=2)+'\n')
(base/'ARGV_DEVIATIONS.json').write_text(json.dumps({'host_successful_calls_enabled':o.record_successful_host_calls,'deviations':['host strace output redirected to current task','bind independent instrument/output directory','env launcher sets preload and output prefix']+(['remove host status=failed filter'] if o.record_successful_host_calls else []),'remaining_argv_exact':True,'shared_incremental_tree_not_reset':True,'command':cmd},indent=2)+'\n')
print(json.dumps({'prepared':True,'host_successful_calls_enabled':o.record_successful_host_calls,'remaining_argv_exact':True,'deadline':'2026-09-21T17:20:00+08:00'}))
