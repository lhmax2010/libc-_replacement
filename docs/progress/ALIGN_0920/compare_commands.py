import difflib,hashlib,json,pathlib,shlex
b=pathlib.Path('progress/ALIGN_0920');out=b/'comparison';out.mkdir(exist_ok=True)
pairs=[('argv','progress/BUILD_WEEKEND_0918/cell_commands/llvm-arm-whole-retry.json','progress/ARM_RPM_DIAG/cell_commands/llvm-arm-repro.json'),('scope','progress/BUILD_WEEKEND_0918/raw/432_w2_arm_llvm_retry.command.txt','progress/ARM_RPM_DIAG/raw/045_reproduce.command.txt'),('guard','progress/BUILD_WEEKEND_0918/guard.py','progress/ARM_RPM_DIAG/guard.py'),('deadlines','progress/BUILD_WEEKEND_0918/deadlines.json','progress/ARM_RPM_DIAG/deadlines.json')]
summary=[]
for label,a,z in pairs:
    aa=pathlib.Path(a).read_bytes();zz=pathlib.Path(z).read_bytes()
    (out/(label+'-old'+pathlib.Path(a).suffix)).write_bytes(aa);(out/(label+'-new'+pathlib.Path(z).suffix)).write_bytes(zz)
    (out/(label+'.diff')).write_text(''.join(difflib.unified_diff(aa.decode().splitlines(True),zz.decode().splitlines(True),fromfile=a,tofile=z)))
    summary.append({'label':label,'old':a,'new':z,'byte_identical':aa==zz,'old_sha256':hashlib.sha256(aa).hexdigest(),'new_sha256':hashlib.sha256(zz).hexdigest()})
old=json.loads(pathlib.Path(pairs[0][1]).read_text());new=json.loads(pathlib.Path(pairs[0][2]).read_text())
for label,cmd in [('old',old),('new',new)]:
    (out/(label+'-full-command.txt')).write_text(shlex.join(cmd)+'\n')
    env={cmd[i+1]:cmd[i+2] for i,v in enumerate(cmd) if v=='--setenv'}
    defines=[cmd[i+1] for i,v in enumerate(cmd) if v=='--define']
    (out/(label+'-env-defines.json')).write_text(json.dumps({'env':env,'defines':defines},indent=2)+'\n')
ops=[]
for op,i,j,k,l in difflib.SequenceMatcher(a=old,b=new,autojunk=False).get_opcodes():
    if op!='equal':ops.append({'operation':op,'old_index':[i,j],'new_index':[k,l],'old':old[i:j],'new':new[k:l]})
(out/'argv-all-differences.json').write_text(json.dumps(ops,indent=2)+'\n')
(out/'comparison-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({'pairs':summary,'argv_differences':ops},indent=2))
