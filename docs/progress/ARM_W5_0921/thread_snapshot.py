"""Sample host /proc only; no guest environment or ptrace changes."""
import datetime,json,pathlib,sys,time
out=pathlib.Path(sys.argv[1]);cg=sys.argv[2]
pids=(pathlib.Path('/sys/fs/cgroup')/cg.lstrip('/')/'cgroup.procs').read_text().split()
rows=[]
for pid in pids:
    p=pathlib.Path('/proc')/pid
    try:
        args=[s.decode(errors='replace') for s in (p/'cmdline').read_bytes().split(b'\0') if s]
        # The main guest process, not a shell command containing rpmbuild text.
        if not any(pathlib.Path(s).name=='rpmbuild' for s in args[:3]):continue
        status=(p/'status').read_text()
        tasks=[]
        for t in sorted((p/'task').iterdir()):
            try: tasks.append(dict(tid=int(t.name),comm=(t/'comm').read_text().strip(),stat=(t/'stat').read_text()))
            except FileNotFoundError:pass
        env={}
        for s in (p/'environ').read_bytes().split(b'\0'):
            if s.startswith((b'QEMU_RESERVED_VA=',b'RPM_BUILD_NCPUS=',b'MAKEFLAGS=')):
                k,v=s.split(b'=',1);env[k.decode()]=v.decode()
        rows.append(dict(pid=int(pid),argv=args,status=status,tasks=tasks,selected_environment=env))
    except (FileNotFoundError,ProcessLookupError):continue
lines=(out/'build.log').read_text(errors='replace').splitlines()
row=dict(time=datetime.datetime.now().astimezone().isoformat(),monotonic_ns=time.monotonic_ns(),wrote=sum(s.startswith('Wrote:') for s in lines),last_build_lines=lines[-2:],processes=rows)
with (out/'threads.jsonl').open('a') as f:f.write(json.dumps(row)+'\n')
print(json.dumps(dict(time=row['time'],wrote=row['wrote'],samples=[dict(pid=r['pid'],threads=next(s for s in r['status'].splitlines() if s.startswith('Threads:')),task_count=len(r['tasks'])) for r in rows])))
