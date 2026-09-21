import datetime,json,pathlib
base=pathlib.Path('progress/ARM_LZMA_MODE_0921/cells/gdb-mode-entry')
scope=pathlib.Path('/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/arm-lzma-mode-0921.scope')
log=(base/'build.log').read_text(errors='replace').splitlines()
obs=[json.loads(s) for s in (base/'gdb-observations.jsonl').read_text().splitlines()]
events=[json.loads(s) for s in (base/'events.jsonl').read_text().splitlines()]
print(json.dumps(dict(time=datetime.datetime.now().astimezone().isoformat(),log_lines=len(log),last_lines=log[-2:],wrote=sum(x.startswith('Wrote:') for x in log),close_count=obs[-1]['close_count'] if obs else None,observations=obs[-2:],last_resource_event=[e for e in events if e['event'] in ('build_scheduler_verified','stop','paused','resumed','finished','gdb_finished')][-1:],memory_current=(scope/'memory.current').read_text().strip() if scope.exists() else None,memory_events=(scope/'memory.events').read_text() if scope.exists() else None,exitcode=(base/'exitcode').read_text().strip() if (base/'exitcode').exists() else 'RUNNING'),ensure_ascii=False))
