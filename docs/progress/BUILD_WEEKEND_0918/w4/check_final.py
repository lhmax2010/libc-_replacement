"""Cross-check final tables and executed script snapshots; no platform writes."""
import csv,hashlib,json
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
matrix=list(csv.DictReader((out/'W1_MATRIX.tsv').open(),delimiter='\t'))
assert len(matrix)==6
assert sum(x['overall']=='PASS_BUILD_PAYLOAD_MACRO_NOT_BOARD' for x in matrix)==4
assert sum(x['overall']=='FAIL_EXTRA_DYNAMIC_LIBBPF' for x in matrix)==1
assert next(x for x in matrix if x['arch']=='armv7l' and x['mode']=='libcxx')['build_exitcode']=='-15'
rpm=json.loads((out/'llvm_retry_partial/result.json').read_text())
assert rpm['whole_build_exitcode']==1 and len(rpm['rpms'])==18
assert rpm['status']=='RPM_PAYLOADS_VERIFIED_BUT_BUILD_INCOMPLETE'
board=json.loads((out/'board_cancel/results.json').read_text())
assert len(board)==40 and all(x['assertions_pass'] and x['remote_exitcode']==0 for x in board)
assert json.loads((out/'board_cancel/summary.json').read_text())['cleanup']=='VERIFIED_TASK_DIRECTORY_ABSENT_INITIAL_ROOT_STATE_RESTORED'
assert json.loads((out/'board-install-original-only/cleanup.json').read_text())['status']=='PACKAGES_AND_OWN_FILES_REMOVED_INITIAL_ROOT_STATE_RESTORED'
install=json.loads((out/'board-install-original-only/results.json').read_text())
assert len(install)==1 and install[0]['status']=='NOT_AVAILABLE_NORMAL_RPM_DEPENDENCY_TEST_FAILED'
packages=list(csv.DictReader((out/'PACKAGE_STATUS.tsv').open(),delimiter='\t'))
assert len(packages)==11 and len({x['package'] for x in packages})==11
assert all(x['remote_head_match']=='MATCH' for x in packages)
snapshots=[]
for f in sorted((out/'cells').glob('*/events.jsonl')):
    for line in f.read_text().splitlines():
        event=json.loads(line)
        if event.get('event')=='script_snapshot':
            source=Path(event['snapshot'])
            assert hashlib.sha256(source.read_bytes()).hexdigest()==event['sha256'],source
            snapshots.append(str(source))
for file in ('W1_REPORT.md','W2_REPORT.md','W3_REPORT.md','W4_REPORT.md','FINAL_RESULT.md','STATIC_CANCEL_WEEKLY.md','PACKAGE_STATUS.md','SPEC_GUARD_AUDIT.tsv','BOARD_ROOT_OPERATIONS.tsv'):
    assert (out/file).stat().st_size>0
result={'status':'PASS','bpftrace_cells':6,'passed_fallback_cells':4,'whole_llvm_build_exitcode':1,
        'separately_verified_llvm_rpms':18,'physical_board_cancel_runs':40,'physical_board_cancel_passes':40,
        'published_package_count':11,'matched_execution_snapshots':len(snapshots),
        'candidate_release_gate':'NOT_CLOSED','scope':'Data consistency and recorded script snapshots; not additional builds or functional equivalence.'}
(out/'FINAL_SELFCHECK.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result),flush=True)
