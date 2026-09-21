"""Preserve this run's trace and restore the historical evidence path."""
import hashlib,json,pathlib,subprocess
root=pathlib.Path('/home/toolchain/development/libc++_replacement');base=root/'progress/ARM_W5_0921'
row=json.loads((base/'trace-preservation.json').read_text())
src=root/row['original'];old=root/row['backup'];dst=base/'cells/w5-original/host-failed.strace'
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
assert sha(old)==row['sha256']
assert not dst.exists()
commands=[]
for a,b in ((src,dst),(old,src)):
    before=sha(a);cmd=['cp','-a','--',str(a),str(b)]
    r=subprocess.run(cmd,capture_output=True,text=True)
    commands.append(dict(command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,sha256=before))
    assert r.returncode==0 and sha(b)==before
result=dict(commands=commands,historical_trace_restored=True,old_sha256=sha(src),new_trace_sha256=sha(dst))
(base/'TRACE_STORAGE.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
