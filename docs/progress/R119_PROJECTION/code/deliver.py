#!/usr/bin/env python3
"""仅交付本任务目录；无 force、无 Gerrit、无分支切换。"""
from common import *
gate('delivery_gate')
rc,out,_=record('054_branch_guard',['git','branch','--show-current']);assert rc==0 and out.strip()=='codex/runtime-validation'
rc,out,_=record('055_staged_guard',['git','diff','--cached','--name-only']);assert rc==0
assert all(p.startswith('docs/progress/R119_PROJECTION/') for p in out.splitlines()),out
for path,digest in json.loads((OUT/'FROZEN_INPUT_SHA256.json').read_text()).items():assert sha(ROOT/path)==digest,path
rc,_,_=record('056_stage_final',['git','add','--','docs/progress/R119_PROJECTION',':(exclude)docs/progress/R119_PROJECTION/code/__pycache__']);assert rc==0
rc,out,_=record('057_staged_final_guard',['git','diff','--cached','--name-only']);assert rc==0
assert out and all(p.startswith('docs/progress/R119_PROJECTION/') and '__pycache__' not in p for p in out.splitlines()),out
# 原始 stdout 必须逐字保留，编译器布局和源码空行的空白不作格式修改。
rc,out,_=record('058_format_check',['git','diff','--cached','--check','--','docs/progress/R119_PROJECTION',':(exclude)docs/progress/R119_PROJECTION/raw']);print(out);assert rc==0
rc,out,_=record('059_commit_final',['git','commit','-m','docs(runtime): complete R119 projection coverage audit with explicit unresolved results']);print(out);assert rc==0
rc,out,ref=record('060_push_final',['git','push','origin','codex/runtime-validation']);print(out);print((ROOT/(ref+'.stderr')).read_text());assert rc==0
rc,local,_=record('061_local_final',['git','rev-parse','HEAD']);assert rc==0
rc,remote,_=record('062_remote_final',['git','ls-remote','--heads','origin','refs/heads/codex/runtime-validation']);assert rc==0
assert remote.split()[0]==local.strip(),(local,remote)
save(OUT/'DELIVERY.json',dict(local=local.strip(),remote=remote.split()[0],match=True,branch='codex/runtime-validation'))
print('VERIFIED',local.strip())
