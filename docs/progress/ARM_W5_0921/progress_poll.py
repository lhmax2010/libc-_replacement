import datetime,json,pathlib
out=pathlib.Path('progress/ARM_W5_0921/cells/w5-original')
scope=pathlib.Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/arm-w5-0921.scope')
lines=(out/'build.log').read_text(errors='replace').splitlines()
samples=[json.loads(s) for s in (out/'threads.jsonl').read_text().splitlines()]
latest=next((r for r in reversed(samples) if r['processes']),None)
threads=[]
if latest:
    threads=[dict(pid=r['pid'],threads=next(s for s in r['status'].splitlines() if s.startswith('Threads:')),tasks=[t['comm'] for t in r['tasks']]) for r in latest['processes']]
print(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),lines=len(lines),last_lines=lines[-2:],wrote=sum(s.startswith('Wrote:') for s in lines),latest_threads=threads,memory_current=(scope/'memory.current').read_text().strip() if scope.exists() else None,memory_events=(scope/'memory.events').read_text() if scope.exists() else None,exitcode=(out/'exitcode').read_text().strip() if (out/'exitcode').exists() else 'RUNNING'),ensure_ascii=False))
