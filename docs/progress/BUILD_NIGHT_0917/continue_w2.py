"""Sequential continuation only after the active x86 static build succeeds."""
import datetime,subprocess,time,json
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); deadline=datetime.datetime.fromisoformat((p/'raw/043_w2_resource_gate.started.txt').read_text().strip()).timestamp()+5*3600
first=p/'raw/051_build_libclang_x86.exitcode'
while not first.exists():
    if time.time()>=deadline: raise SystemExit(124)
    time.sleep(1)
assert first.read_text().strip()=='0','x86 static prerequisite failed'
steps=[
 ('058_libclang_x86_archive',['python3',str(p/'verify_archives.py'),'tmp/NIGHT_0917/builds/libclang-x86_64-libcxx/lib64/libclang.a']),
 ('059_configure_libclang_gcc',['python3',str(p/'run_libclang_cell.py'),'x86_64-gcc','configure']),
 ('060_build_libclang_gcc',['python3',str(p/'run_libclang_cell.py'),'x86_64-gcc','build']),
 ('061_configure_libclang_aarch',['python3',str(p/'run_libclang_cell.py'),'aarch64-libcxx','configure']),
 ('062_build_libclang_aarch',['python3',str(p/'run_libclang_cell.py'),'aarch64-libcxx','build']),
 ('063_libclang_aarch_archive',['python3',str(p/'verify_archives.py'),'tmp/NIGHT_0917/builds/libclang-aarch64-libcxx/lib64/libclang.a'])]
for label,cmd in steps:
    if time.time()>=deadline: raise SystemExit(124)
    rc=subprocess.call(['bash',str(p/'run_logged.sh'),label,*cmd])
    if rc: print('STOP prerequisite or cell failed:',label,rc,flush=True); raise SystemExit(rc)
print('W2 sequence finished; final verification still required.',flush=True)
