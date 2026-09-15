"""Record measured command intervals, without summing overlapping operations."""
import datetime
import json
from pathlib import Path

p = Path('progress/BUILD_W2C_0915')
labels = ['011_resume_controlled_retry','018_restore_config_first',
          '019_restore_config_retry','022_restore_config_second',
          '024_verify_restored_config','026_verify_restored_retry',
          '029_explain_checkpoint','045_confirmed_config_check','048_llvm_confirmed_resume']
rows=[]
for label in labels:
    matches=list((p/'raw').glob(label+'.started.txt'))
    if not matches:
        rows.append(dict(label=label,status='NOT_AVAILABLE',reason='Exact recorded command label not found'))
        continue
    started=matches[0].read_text().strip()
    finish_file=p/f'raw/{label}.finished.txt'
    if not finish_file.exists():
        rows.append(dict(label=label,status='NOT_OBSERVED',started=started))
        continue
    finished=finish_file.read_text().strip()
    delta=datetime.datetime.fromisoformat(finished)-datetime.datetime.fromisoformat(started)
    rows.append(dict(label=label,started=started,finished=finished,wall_seconds=delta.total_seconds(),
                     exitcode=(p/f'raw/{label}.exitcode').read_text().strip()))
events=[json.loads(line) for line in (p/'build-w2c-llvm-arm-retry.events.jsonl').read_text().splitlines()]
start=next(event for event in events if event['event']=='start')['time']
finish=next(event for event in events if event['event']=='finished')['time']
failed=(datetime.datetime.fromisoformat(finish)-datetime.datetime.fromisoformat(start)).total_seconds()
result=dict(command_intervals=rows,wrong_configuration_attempt_seconds=failed,
    rework=json.loads((p/'rework_summary.json').read_text()),
    qualification='Intervals may overlap; not summed. Recorded repeated object durations do not establish exact counterfactual additional wall-clock time.')
(p/'timing_summary.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False))
