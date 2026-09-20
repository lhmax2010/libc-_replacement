"""The coarse trace grep is not a syscall parser: qualify its write candidates."""
import csv,json,re
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG');rows=[]
for r in csv.DictReader((b/'guest-error-calls.tsv').open(),delimiter='\t'):
    if r['category']!='SELECTED_WRITE_OPERATION_ERROR':continue
    calls=re.findall(r'\b([0-9]+) ([A-Za-z_][A-Za-z_0-9]*)\(',r['actual_trace_line'])
    r.update(pid_call_fragments=calls,interleaved=len(calls)>1,
             interpretation='Multiple calls on one line: cannot attach the error to write' if len(calls)>1 else 'Requires individual review')
    rows.append(r)
result={'coarse_candidates':len(rows),'interleaved_candidates':sum(r['interleaved'] for r in rows),'rows':rows,
        'warning':'trace-audit.json SELECTED_WRITE_OPERATION_ERROR is a coarse line-match category, NOT a count of failed guest writes. ENOSYS there is also matching-line count, not parsed call count.'}
(b/'guest-interleaving-review.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
