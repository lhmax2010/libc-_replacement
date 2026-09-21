"""仅交付本轮材料；Git 日志置于临时目录，避免自引用提交。"""
import json, pathlib, shlex, subprocess, time
from common import ROOT, OUT, gate

gate('delivery_gate')
logs=ROOT/'tmp/R119_ENUM_RETEST/delivery'/str(time.time_ns())
logs.mkdir(parents=True,exist_ok=True)
def git(label,*args):
    cmd=['git',*args]
    prefix=logs/label
    assert not prefix.with_suffix('.command.txt').exists(), label
    prefix.with_suffix('.command.txt').write_text(shlex.join(cmd)+'\n')
    p=subprocess.run(cmd,cwd=ROOT,capture_output=True)
    prefix.with_suffix('.stdout').write_bytes(p.stdout)
    prefix.with_suffix('.stderr').write_bytes(p.stderr)
    prefix.with_suffix('.exitcode').write_text(str(p.returncode)+'\n')
    print(label, 'exit=',p.returncode,flush=True)
    if p.stdout: print(p.stdout.decode(errors='replace'),flush=True)
    if p.stderr: print(p.stderr.decode(errors='replace'),flush=True)
    if p.returncode: raise SystemExit(p.returncode)
    return p.stdout.decode().strip()

assert git('branch','branch','--show-current')=='codex/runtime-validation'
cached=git('cached_before','diff','--cached','--name-only').splitlines()
summary='docs/progress/RUNTIME_PHASE_SUMMARY_0921/'
selected=[summary+n for n in ['SUMMARY_zh.md','SUMMARY_en.md','BACKLOG_zh.md','BACKLOG_en.md','BACKLOG.tsv','VERSION_20260921_2.md']]
task='docs/progress/R119_ENUM_RETEST/'
assert all(n.startswith(task) or n in selected for n in cached)
git('add','add','--',task,*selected)
names=git('cached_after','diff','--cached','--name-only').splitlines()
assert names and all(n.startswith(task) or n in selected for n in names)
git('whitespace','diff','--cached','--check','--',*selected,task+'FINAL.md',task+'STATUS.md',task+'code/')
git('commit','commit','-m','docs(runtime): verify enum type divergence on ARM and update phase summary')
local=git('local_head','rev-parse','HEAD')
git('push','push','origin','codex/runtime-validation')
remote=git('remote_head','ls-remote','--heads','origin','refs/heads/codex/runtime-validation').split()[0]
assert remote==local,(remote,local)
assert not git('scope_clean','status','--porcelain','--',task,*selected)
result={'local':local,'remote':remote,'equal':True,'time_unix':time.time()}
(logs/'RESULT.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result),flush=True)
