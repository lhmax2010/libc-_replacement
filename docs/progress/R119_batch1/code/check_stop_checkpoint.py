"""只核验现有中止产物的一致性；不重启源码扫描。"""
from common import *
import json,csv
d=OUT/'W1'
checkpoint=json.loads((d/'CHECKPOINT_026.json').read_text())
assert checkpoint['statuses']=={'CROSS_PACKAGE':14,'NOT_OBSERVED':12},checkpoint
assert checkpoint['scope_complete']is False
with (d/'REVIEW_PROGRESS_100.tsv').open()as f:rows=list(csv.DictReader(f,delimiter='\t'))
assert len(rows)==100 and rows[25]['candidate']=='std::atomic'and rows[25]['result']=='CROSS_PACKAGE'
assert all(r['result']=='NOT_OBSERVED_PENDING_REVIEW'for r in rows[26:])
archive=json.loads((d/'entity_full/ARCHIVE.json').read_text())
assert len(archive['items'])==100
for r in archive['items']:assert digest(OUT/r['path'])==r['sha256'],r['path']
index=json.loads((d/'positive/INDEX.json').read_text())
specs=json.loads((OUT/'code/positive_specs.json').read_text())
assert {r['key']for r in index}=={s['key']for s in specs}
assert len(index)==19
assert (OUT/'raw/105_resource_recheck.exitcode').read_text().strip()=='20'
assert (OUT/'raw/102_source_include_index.exitcode').read_text().strip()=='-15'
assert (OUT/'W1/source_includes/INTERRUPTED.md').is_file()
rc,branch,raw=record('checkpoint_branch',['git','branch','--show-current']);assert rc==0 and branch.strip()=='codex/runtime-validation'
save(OUT/'STOP_SELF_CHECK.json',{'status':'PASS','cross_package':14,'not_observed_in_reviewed_26':12,'remaining_unfinished':74,'scope_complete':False,'archive_entities':100,'positive_proof_keys':len(index),'resource_exit':20,'source_scan_exit':-15,'board':'板子已清理并释放，W1 未重新连接','limit':'这是中止产物一致性核验，不是100项定界完成或全平台阴性证明。'})
print('CHECKPOINT',14,12,74,'ARCHIVE',100,'RESOURCE_EXIT',20)
