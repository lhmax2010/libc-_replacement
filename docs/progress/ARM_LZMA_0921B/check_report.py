"""Check quotes, snapshots and stated counts against actual evidence."""
import hashlib,json,pathlib
b=pathlib.Path('progress/ARM_LZMA_0921B');c=b/'cells/gdb-preserved'
report=(b/'FINAL_RESULT.md').read_text()
lines=(c/'gdb-observations.jsonl').read_text().splitlines()
for n in (21,22,23):assert lines[n-1] in report,n
summary=json.loads((b/'GDB_RUN_SUMMARY.json').read_text())
post=json.loads((b/'POSTCHECK_GDB.json').read_text())
assert len(summary['wrote'])==18
assert summary['event_counts']['lzclose_counter']==19
assert summary['event_counts']['lzopen_return']==summary['event_counts']['lzwrite_entry']==summary['event_counts']['cpio_header_entry']==1
assert post['rpmbuild_exitcode']==1 and post['gdb_exitcode']==0
assert post['old_errors']==post['new_errors']
assert post['no_extra_return_breakpoints_activated']
for name in ('gdb_observer.py','guard.py','capture_failure.py'):
    actual=hashlib.sha256((b/name).read_bytes()).hexdigest()
    assert actual==summary['script_sha256'][name]
    assert (b/name).read_bytes()==(c/('executed-'+name)).read_bytes()
result=dict(raw_three_point_quotes_exact=True,snapshots_match_executed_hashes=True,
    wrote_count=18,lzclose_count=19,observer_errors=summary['observer_errors'],
    no_extra_return_breakpoints=True,no_new_trial=True,
    final_report_sha256=hashlib.sha256((b/'FINAL_RESULT.md').read_bytes()).hexdigest())
(b/'REPORT_CHECK.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
