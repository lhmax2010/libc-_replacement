#!/usr/bin/env python3
"""核对双语证据链接、关键分母、内部编号与脚本/文档快照；不冒充人工审批。"""
import ast,csv,hashlib,json,pathlib,re,shutil
r=pathlib.Path.cwd();base=r/'docs/progress/IMPL_0908/W4';w3=base.parent/'W3'
documents=[base/'SUPPORT_SCOPE_ZH.md',base/'SUPPORT_SCOPE_EN.md'];texts=[p.read_text() for p in documents]
link_sets=[];evidence=[]
for p,text in zip(documents,texts):
 links=re.findall(r'\[[^\]]+\]\(([^)]+)\)',text);assert links
 resolved=[]
 for href in links:
  assert not href.startswith(('http:','https:')),'this check covers repository evidence, not remote availability'
  target=(p.parent/href.split('#',1)[0]).resolve();assert target.is_file(),(p,href)
  resolved.append(str(target.relative_to(r)))
  evidence.append([p.name,href,str(target.relative_to(r)),hashlib.sha256(target.read_bytes()).hexdigest()])
 link_sets.append(set(resolved))
 prose=re.sub(r'\[[^\]]+\]\([^)]+\)',lambda m:m[0].split('](',1)[0][1:],text)
 internal=re.compile(r'(?<![A-Za-z0-9_])(?:R\d{2,3}|W[1-4]|D-?\d{2})(?![A-Za-z0-9_])')
 assert internal.search('R115') and internal.search('D-11')
 assert not internal.search(prose),(p,'internal label in prose')
 assert 'f3c1af692b579add991861e1f7c4950f6af39932' in text
assert link_sets[0]==link_sets[1],('bilingual source-set mismatch',link_sets)
native=json.loads((w3/'summary_x86_64_new_full.json').read_text());arm=json.loads((w3/'summary_armv7l_new_combined.json').read_text())
assert native['tests_recorded']==11402 and arm['tests_recorded']==7848 and arm['historical_not_in_result']==3554
assert len(native['new_failure_candidates'])==len(arm['new_failure_candidates'])==1
extra=list(csv.DictReader((w3/'timed_versions.tsv').open(),delimiter='\t'))
for arch in ['x86_64','armv7l']:
 rows=list(csv.DictReader((w3/f'matrix_{arch}_final.tsv').open(),delimiter='\t'))
 assert len(rows)==225 and sum(x['arch']==arch for x in extra)==10
for text in texts:
 for number in ['235','11,402','7,848','3,554','81']:assert number in text,number
 for term in ['LLVM_22_TIZEN_1','LLVM_22','pubsync','shared_timed_mutex','call_once','std::async','aarch64','NOT_OBSERVED','NOT_AVAILABLE','C++03','C++20','C++26']:assert term in text,term
with (base/'EVIDENCE_INDEX.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['draft','link','repository_path','sha256']);w.writerows(evidence)
snap=base/'snapshots';snap.mkdir(exist_ok=True)
for p in [base/'code/check_drafts.py',r/'docs/progress/R114/code/record.py',r/'tools/resource_gate.sh']:
 shutil.copyfile(p,snap/p.name)
ast.parse((base/'code/check_drafts.py').read_text())
files=sorted([*documents,base/'EVIDENCE_INDEX.tsv',base/'REVIEW_CHECKLIST.md',base/'REPORT.md',*snap.iterdir()])
with (base/'SHA256SUMS').open('w') as f:
 for p in files:f.write(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+str(p.relative_to(r))+'\n')
print('BILINGUAL_LINKS=PASS occurrences='+str(len(evidence))+' unique_sources='+str(len(link_sets[0])))
print('INTERNAL_LABELS_IN_PROSE=0 POSITIVE_CONTROL=PASS')
print('KEY_DENOMINATORS=PASS 235_per_arch native=11402 arm=7848 gap=3554 new_failure=1')
print('SNAPSHOTS_AND_DOCUMENT_HASHES=PASS; semantic review remains a human handoff, not automated approval')
