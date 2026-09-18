import hashlib,json,shutil
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; task=p/'tmp/WEEKEND_0918'
(task/'specs').mkdir(parents=True,exist_ok=True)
records=[]
for name in ('bpftrace.spec','libcxx-runtimes.spec','llvm.spec'):
    old=p/'tmp/STATIC_0917B/specs'/name; new=task/'specs'/name
    assert not new.exists(); shutil.copyfile(old,new)
    records.append({'source':str(old),'destination':str(new),'sha256':hashlib.sha256(new.read_bytes()).hexdigest()})
(out/'spec_input_identity.json').write_text(json.dumps(records,indent=2))
old=json.loads((p/'progress/BUILD_STATIC_0917B/resume_0917/cell_commands/runtime-arm-libcxx.json').read_text())
old=[x.replace(str(p/'tmp/STATIC_0917B'),str(task)) for x in old]
for d in ('BUILD','BUILDROOT','RPMS','SRPMS','TMP'):
    (task/'rpm-arm-runtime'/d).mkdir(parents=True,exist_ok=True)
(out/'cell_commands/runtime-arm-libcxx.json').write_text(json.dumps(old,indent=2))
print(json.dumps(records,indent=2))
