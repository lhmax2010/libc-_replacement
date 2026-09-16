from common import *
import csv,json,collections
dest=OUT/'W1'
with (dest/'BATCH_100.tsv').open()as f:rows=list(csv.DictReader(f,delimiter='\t'))
coverage=json.loads((dest/'HEADER_COVERAGE.json').read_text());edges={r['entity']:r for r in json.loads((dest/'EDGE_SEARCH_COUNTS.json').read_text())}
result=[]
for r in rows:
    e=edges[r['entity']]
    result.append(dict(rank=r['rank'],candidate=r['entity'],boundary_result='NOT_OBSERVED_PENDING_REVIEW',public_signature_package='NOT_OBSERVED',consumer_package='NOT_OBSERVED',boundary_shape='NOT_OBSERVED',per_candidate_elapsed_seconds='NOT_OBSERVED',header_occurrences=coverage['occurrences'].get(r['entity'],0),component_symbol_rows=e['component_rows'],runtime_symbol_rows=e['runtime_rows'],evidence='header_occurrences.tsv.gz;symbol_candidate_edges.tsv.gz',note='准备队列，不是已完成的定界表'))
with (dest/'REVIEW_PROGRESS_100.tsv').open('w',newline='')as f:
    w=csv.DictWriter(f,fieldnames=list(result[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(result)
data=dict(frozen_candidates=len(rows),completed_boundary_reviews=0,classification_counts='NOT_OBSERVED_PENDING_SCOPE_CONFIRMATION_AND_REVIEW',header_inventory=coverage['stats'],header_issues=len(coverage['issues']),header_scan_seconds=coverage['elapsed_seconds'],candidate_mean_minutes='NOT_OBSERVED:共有扫描时间不是逐候选定界耗时',symbol_material_note='已有表中的行数；不是不同消费包数、不是本轮已重核的 ELF 数')
save(dest/'CHECKPOINT.json',data)
print(json.dumps(data,ensure_ascii=False,indent=2))
print('header issue examples',json.dumps(coverage['issues'][:8],ensure_ascii=False))
