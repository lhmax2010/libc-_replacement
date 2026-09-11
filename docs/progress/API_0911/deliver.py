#!/usr/bin/env python3
"""逐项检查后普通提交/推送；交付日志不自包含进其自身提交。"""
import pathlib, subprocess, sys, os
phase=sys.argv[1];root=pathlib.Path('docs/progress/API_0911');paths=[str(root/phase),'docs/progress/API_STATUS_0911.md','docs/progress/API_QUESTIONS_0911.md']+sys.argv[2:]
prefix=phase+os.environ.get('API_DELIVERY_ATTEMPT','')
def run(s,cmd):
    rc=subprocess.run([sys.executable,str(root/'record.py'),str(root/'delivery'/(prefix+'_'+s)),*cmd]).returncode
    if rc:raise SystemExit(rc)
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/runtime-validation'
run('stage',['git','add',*paths]);run('check',['git','diff','--cached','--check','--','*.md','*.py','*.cpp','*.h'])
run('commit',['git','commit','-m','docs: API weekend '+phase+' evidence and scope'])
run('push',['git','push','origin','codex/runtime-validation'])
run('local',['git','rev-parse','HEAD']);run('remote',['git','ls-remote','origin','refs/heads/codex/runtime-validation'])
local=(root/'delivery'/(prefix+'_local.stdout')).read_text().strip()
remote=(root/'delivery'/(prefix+'_remote.stdout')).read_text().split()[0]
assert local==remote,(local,remote)
print('VERIFIED_REMOTE_HEAD',local)
