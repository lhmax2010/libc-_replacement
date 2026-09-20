import csv,hashlib,json,pathlib,re,shutil
b=pathlib.Path('progress/ALIGN_0920');e=b/'evidence';e.mkdir(exist_ok=True)
sources={'failure0917':'progress/BUILD_STATIC_0917B/resume_0917/cells/llvm-arm-rpm-serial/build.log','failure0918':'progress/BUILD_WEEKEND_0918/cells/llvm-arm-whole-retry/build.log','success_diag':'progress/ARM_RPM_DIAG/cells/llvm-arm-repro/build.log'}
orders={}
for label,path in sources.items():
    rows=[];excerpt=[]
    for n,line in enumerate(pathlib.Path(path).open(errors='replace'),1):
        if line.startswith('Wrote: '):rows.append((line.strip().split('/')[-1],n));excerpt.append(f'{path}:{n}: {line.rstrip()}')
        if 'create archive failed:' in line:excerpt.append(f'{path}:{n}: {line.rstrip()}')
    orders[label]=rows;(e/(label+'-packaging-lines.txt')).write_text('\n'.join(excerpt)+'\n')
assert len(orders['failure0917'])==len(orders['failure0918'])==18
assert [r[0] for r in orders['failure0917']]==[r[0] for r in orders['failure0918']]==[r[0] for r in orders['success_diag']][:18]
with (b/'rpm-order-comparison.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['order','rpm','09-17_failure_line','09-18_W2_failure_line','diagnostic_success_line'])
    for i,(name,line) in enumerate(orders['success_diag']):w.writerow([i+1,name,*[orders[k][i][1] if i<len(orders[k]) else 'NOT_WRITTEN' for k in ('failure0917','failure0918')],line])
roots={'llvm_aarch64_complete':'tmp/STATIC_0917B/rpm-aarch64/RPMS','llvm_arm_approved_partial':'tmp/STATIC_0917B/rpm-arm-llvm/RPMS','llvm_arm_w2_partial':'tmp/WEEKEND_0918/rpm-arm-llvm/RPMS','llvm_arm_diagnostic_complete':'tmp/ARM_RPM_DIAG/rpm-arm-llvm/RPMS','runtime_arm':'tmp/WEEKEND_0918/rpm-arm-runtime/RPMS'}
for arch in ('armv7l','aarch64'):
    for mode in ('libcxx','gcc','undefined'):roots['bpf_'+arch+'_'+mode]=f'tmp/WEEKEND_0918/rpm-bpf-{arch}-{mode}/RPMS'
inventory=[]
for label,path in roots.items():
    p=pathlib.Path(path);files=sorted(p.rglob('*.rpm')) if p.exists() else []
    inventory.append({'label':label,'root':path,'root_exists':p.exists(),'rpm_count':len(files),'files':[{'path':str(f),'bytes':f.stat().st_size} for f in files]})
(b/'current-rpm-inventory.json').write_text(json.dumps(inventory,indent=2)+'\n')
provenance={
'published-source':'tmp/BUILD_W1BC_0911/target-fetch/packaging/SOURCE_PROVENANCE',
'codes-readonly':'codes/R104/llvm/packaging/SOURCE_PROVENANCE',
'arm-installed-input':'tmp/WEEKEND_0918/input-armv7l-v3/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE',
'arm-runtime-build':'tmp/WEEKEND_0918/rpm-arm-runtime/BUILD/llvm-22.1.8/SOURCE_PROVENANCE',
'aarch64-installed-input':'tmp/WEEKEND_0918/input-aarch64-v3/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE'}
prov=[]
for label,path in provenance.items():
    p=pathlib.Path(path);target=e/('SOURCE_PROVENANCE-'+label+'.txt');shutil.copy2(p,target);data=p.read_bytes()
    prov.append({'label':label,'source':path,'snapshot':str(target),'sha256':hashlib.sha256(data).hexdigest(),'bytes':len(data)})
(b/'source-provenance-identities.json').write_text(json.dumps(prov,indent=2)+'\n')
print(json.dumps({'inventory':[{k:r[k] for k in ('label','root_exists','rpm_count')} for r in inventory],'provenance':prov,'first_18_names_same_order':True,'success_total':len(orders['success_diag'])},indent=2))
