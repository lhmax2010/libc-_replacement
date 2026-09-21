"""Summarize existing GDB/build evidence without inferring unobserved values."""
import collections,hashlib,json,pathlib
base=pathlib.Path('progress/ARM_LZMA_0921B');cell=base/'cells/gdb-preserved'
rows=[]
for n,line in enumerate((cell/'gdb-observations.jsonl').read_text().splitlines(),1):
    row=json.loads(line);row['evidence_line']=n;rows.append(row)
build=(cell/'build.log').read_text(errors='replace').splitlines()
summary={
    'rpmbuild_exitcode':(cell/'exitcode').read_text().strip() if (cell/'exitcode').exists() else 'NOT_OBSERVED',
    'gdb_exitcode':(cell/'gdb.exitcode').read_text().strip() if (cell/'gdb.exitcode').exists() else 'NOT_OBSERVED',
    'event_counts':dict(collections.Counter(r['event'] for r in rows)),
    'wrote':[{'line':n,'text':s} for n,s in enumerate(build,1) if s.startswith('Wrote:')],
    'errors':[{'line':n,'text':s} for n,s in enumerate(build,1) if 'error:' in s or 'cpio:' in s],
    'three_points':[r for r in rows if r['event'] in ('lzopen_return','lzwrite_entry','cpio_header_entry')],
    'conditional_returns':[r for r in rows if r['event']=='conditional_return'],
    'observer_errors':[r for r in rows if r['event']=='observer_error'],
    'close_events':[r for r in rows if r['event']=='lzclose_counter'],
    'script_sha256':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in (base/'gdb_observer.py',base/'guard.py',base/'capture_failure.py')},
}
(base/'GDB_RUN_SUMMARY.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n')
brief={k:v for k,v in summary.items() if k not in ('three_points','conditional_returns','close_events')}
brief['first_three_points']=summary['three_points'][:9]
brief['first_returns']=summary['conditional_returns'][:9]
print(json.dumps(brief,ensure_ascii=False,indent=2))
