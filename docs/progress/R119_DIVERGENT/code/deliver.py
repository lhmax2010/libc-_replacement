#!/usr/bin/env python3
from common import *
phase=sys.argv[1];gate('delivery_gate_'+phase)
save(OUT/'SCRIPT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in (OUT/'code').glob('*.py')})
rc,branch,_=record('branch_'+phase,['git','branch','--show-current'],False);assert rc==0 and branch.strip()=='codex/runtime-validation'
rc,staged,_=record('staged_before_'+phase,['git','diff','--cached','--name-only'],False);assert rc==0 and all(p.startswith('docs/progress/R119_DIVERGENT/') for p in staged.splitlines())
steps=[('add',['git','add','--','docs/progress/R119_DIVERGENT',':(exclude)docs/progress/R119_DIVERGENT/code/__pycache__']),('format',['git','diff','--cached','--check','--','docs/progress/R119_DIVERGENT',':(exclude)docs/progress/R119_DIVERGENT/raw']),('commit',['git','commit','-m','docs(runtime): R119 divergent '+phase]),('push',['git','push','origin','codex/runtime-validation'])]
for tag,args in steps:
 rc,out,ref=record(tag+'_'+phase,args,False);print(out[:800]);print((ROOT/(ref+'.stderr')).read_text());assert rc==0
rc,local,_=record('local_'+phase,['git','rev-parse','HEAD'],False);assert rc==0
rc,remote,_=record('remote_'+phase,['git','ls-remote','--heads','origin','refs/heads/codex/runtime-validation'],False);assert rc==0 and remote.split()[0]==local.strip()
print('REMOTE_EQUALS_LOCAL',local.strip())
