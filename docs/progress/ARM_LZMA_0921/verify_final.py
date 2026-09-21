"""Read-only final identity/resource checks; does not launch another experiment."""
import hashlib,json,pathlib,subprocess
b=pathlib.Path('progress/ARM_LZMA_0921')
root=pathlib.Path('tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0')
expected={
 'tmp/WEEKEND_0918/specs/llvm.spec':'cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68',
 'tmp/ARM_LZMA_0921/lzma_tap.c':'8dd71b1ec006a045354290b06a2655ee2a0aeae1c28bcab3c99f92ad15735c87',
 'tmp/ARM_LZMA_0921/lzma_tap.so':'a663845ffa1e25895507de2e56b355946a443fa0bb2e281c848c4d0d21095f2f',
}
r={'identities':[],'commands':[]}
for path,wanted in expected.items():
 p=pathlib.Path(path);actual=hashlib.sha256(p.read_bytes()).hexdigest()
 r['identities'].append({'path':path,'sha256':actual,'expected':wanted,'equal':actual==wanted})
for argv in [['df','-h','.'],['git','branch','--show-current'],['git','status','--short'],['ps','-p','824601','-o','pid,ppid,ni,stat,etime,args']]:
 c=subprocess.run(argv,capture_output=True,text=True)
 r['commands'].append({'argv':argv,'exitcode':c.returncode,'stdout':c.stdout,'stderr':c.stderr})
(b/'FINAL_CHECKS.json').write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(r,ensure_ascii=False,indent=2))
assert all(x['equal'] for x in r['identities'])
