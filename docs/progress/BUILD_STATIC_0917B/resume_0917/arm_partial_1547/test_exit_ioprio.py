"""Check ioprio reporting for a live then unreaped owned child; no build changes."""
import json, os, subprocess, time
pid=os.fork()
if pid==0:
    r=subprocess.run(['ionice','-c','3','-p',str(os.getpid())])
    if r.returncode: os._exit(r.returncode)
    time.sleep(2)
    os._exit(0)
for phase,delay in [('live',0.5),('exited_unreaped',2.0)]:
    time.sleep(delay)
    for cmd in [['ps','-p',str(pid),'-o','pid,stat,comm'],['ionice','-p',str(pid)]]:
        r=subprocess.run(cmd,capture_output=True,text=True)
        print(json.dumps(dict(phase=phase,command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)),flush=True)
print(json.dumps(dict(waitpid=os.waitpid(pid,0))))
