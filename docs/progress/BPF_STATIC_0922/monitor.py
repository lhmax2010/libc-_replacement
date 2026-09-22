"""Read current build state only; bounded output and append-only observations."""
import datetime,json,re,sys
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'; label=sys.argv[1]; d=E/'cells'/label
events=(d/'events.jsonl').read_text().splitlines(); log=(d/'build.log').read_text(errors='replace').splitlines()
row=dict(time=datetime.datetime.now().astimezone().isoformat(),cell=label,exitcode=(d/'exitcode').read_text().strip() if (d/'exitcode').exists() else 'RUNNING',last_event=json.loads(events[-1]) if events else None,last_target=next((x for x in reversed(log) if re.match(r'\[\s*\d+[%/]',x)),'NOT_OBSERVED'),last_wrote=[x for x in log if x.startswith('Wrote:')],log_bytes=(d/'build.log').stat().st_size)
with (E/'MONITOR.jsonl').open('a') as f:f.write(json.dumps(row)+'\n')
print(json.dumps(row,ensure_ascii=False))
