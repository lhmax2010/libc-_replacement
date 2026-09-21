from common import *
import csv,io,re
SUM=ROOT/'docs/progress/RUNTIME_PHASE_SUMMARY_0921'
gate('final_audit_gate')
ms=json.loads((OUT/'MEASUREMENTS.json').read_text());assert len(ms)==16
ids=['T0008','T0009','T0010','T0011']
expected={'gnu':['St13_Ios_Fmtflags','St12_Ios_Iostate','St13_Ios_Openmode','St12_Ios_Seekdir'],'cxx':['j','j','j','NSt3__18ios_base7seekdirE']}
for m in ms:
 assert m['encoding']==expected[m['library']][ids.index(m['id'])]
 assert m['size']==m['align']==4 and m['repeats']==5
 assert sha(OUT/'probes/types.cpp')==m['source_sha256']
 observed=[]
 for record in m['runs']:
  assert (ROOT/(record+'.exitcode')).read_text().strip()=='0'
  lines=(ROOT/(record+'.stdout')).read_text().splitlines()
  observed.append(next(line for line in lines if line.startswith(m['id']+'\t')))
 assert len(observed)==5 and set(observed)=={m['output']}
for p,h in json.loads((OUT/'PRIOR_HEADER_SHA256.json').read_text()).items():assert sha(p)==h
qs=json.loads((OUT/'BOUNDARY_QUERIES.json').read_text());assert len(qs)==4
assert all(not q['other_source_links'] and not q['all_references'] for q in qs)
assert sorted(len(q['definitions']) for q in qs)==[1,1,1,2]
assert json.loads((OUT/'BOUNDARY_CONTROL.json').read_text())['passed']
sy=json.loads((OUT/'SEEK_SYMBOLS.json').read_text());assert sy[0]['symbol']!=sy[1]['symbol']
virtual=json.loads((OUT/'VIRTUAL_CALL.json').read_text());assert [x['caller_selected_slot_index'] for x in virtual]==[6,6]
for x in sy:
 assert sha(ROOT/x['ir'])==x['ir_sha256']
 assert sha(ROOT/x['source'])==x['source_sha256']
 for p,h in x['header_sha256'].items():assert sha(p)==h
update=json.loads((OUT/'SUMMARY_UPDATE.json').read_text())
baseline=json.loads((OUT/'SUMMARY_V1_SHA256.json').read_text())
objects=run('baseline_git_objects',['git','ls-tree','-r','--format=%(objectname) %(path)',update['baseline_commit'],'--',str(SUM.relative_to(ROOT))])
count=0
for line in objects['stdout'].splitlines():
 oid,path=line.split(' ',1);rel=pathlib.Path(path).relative_to(SUM.relative_to(ROOT));p=OUT/'summary_v1'/rel
 data=p.read_bytes();assert hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest()==oid
 assert sha(p)==baseline[str(rel)];count+=1
assert count==24
allowed={x['file'] for x in update['changed']}
assert allowed=={'SUMMARY_zh.md','SUMMARY_en.md','BACKLOG_zh.md','BACKLOG_en.md','BACKLOG.tsv'}
for name,h in baseline.items():
 if name not in allowed:assert sha(SUM/name)==h,name
for row in update['changed']:assert sha(SUM/row['file'])==row['after_sha256']
old=list(csv.DictReader(io.StringIO((OUT/'summary_v1/BACKLOG.tsv').read_text()),delimiter='\t'))
new=list(csv.DictReader((SUM/'BACKLOG.tsv').open(),delimiter='\t'))
assert len(old)==62 and len(new)==66 and new[:62]==old
assert {r['candidate'] for r in new[62:]}=={'std::ios_base::'+s for s in ['fmtflags','iostate','openmode','seekdir']}
for lang in ['zh','en']:
 assert (SUM/f'BACKLOG_{lang}.md').read_text().startswith((OUT/f'summary_v1/BACKLOG_{lang}.md').read_text())
 text=(SUM/f'SUMMARY_{lang}.md').read_text()
 assert '568' in text and 'St13_Ios_Fmtflags' in text
 visible=re.sub(r'\]\([^)]+\)',']',text)
 assert not re.search(r'\b[RP]\d{1,3}(?:_|\b)',visible)
 if lang=='en':assert not re.search('[\u3400-\u9fff]',text)
# 核对本轮增加的仓内链接；原归档的相对链接保留旧目录语境。
checked=0
for p in [OUT/'FINAL.md',SUM/'SUMMARY_zh.md',SUM/'SUMMARY_en.md',SUM/'BACKLOG_zh.md',SUM/'BACKLOG_en.md',SUM/'VERSION_20260921_2.md']:
 for dest in re.findall(r'\]\(([^)]+)\)',p.read_text()):
  if '://' in dest:continue
  file=dest.split('#',1)[0];target=(p.parent/file).resolve()
  if target==OUT/'AUDIT.json':continue
  assert target.exists(),(p,dest);checked+=1
save('SCRIPT_SHA256.json',{str(p.relative_to(OUT)):sha(p) for p in list((OUT/'code').glob('*.py'))+list((OUT/'probes').glob('*.cpp'))})
save('AUDIT.json',dict(status='PASS',measurement_cells=16,target_observations=80,process_runs=20,arm_divergent=4,aarch64_divergent=4,same_probe_generator=True,same_target_configuration=True,header_hashes_unchanged=True,rpm_header_binding=True,seek_signature_cells=2,virtual_caller_slot_indices=[6,6],boundary_control=True,confirmed_signature_count=5,new_edges=0,x86_pairs=18,x86_edges=23,summary_original_files=24,old_version_matches_git=True,changed_summary_files=sorted(allowed),other_summary_files_unchanged=True,old_backlog_62_unchanged=True,added_findings=4,local_links_checked=checked,board_used=False,parallelism=1,rlimit_as_bytes=LIMIT))
print('AUDIT PASS: 16 cells; 80 observations; 4/4 divergence on both targets; old summary bytes verified against git; only authorized summary fields changed.')
