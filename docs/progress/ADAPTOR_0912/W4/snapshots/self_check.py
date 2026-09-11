#!/usr/bin/env python3
"""可重复的材料完整性检查；不把文本自动检查冒充语义证明。"""
import pathlib,sys,re,json,hashlib,csv
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r
o=r/'docs/progress/ADAPTOR_0912/W4';checks=[]
def check(name,result,detail=''):
 checks.append(dict(check=name,passed=bool(result),detail=detail))
check('chinese_scan_positive_control',bool(re.search(r'[\u3400-\u9fff]','positive: 汉')))
check('internal_label_positive_control',bool(re.search(r'\b(?:W[1-4]|R\d+|P\d+)\b','positive W2 R111 P7')))
for phase in ['W1','W2','W3']:
 p=o.parent/phase;bad=[];n=0
 for line in (p/'SHA256SUMS').read_text().splitlines():
  digest,relative=line.split('  ',1);f=p/relative;n+=1
  if not f.is_file() or hashlib.sha256(f.read_bytes()).hexdigest()!=digest:bad.append(relative)
 check('sealed_'+phase,not bad,dict(files=n,bad=bad))
texts={}
for language in ['zh','en']:
 f=o/('adaptor_analysis_'+language+'.md');s=f.read_text();texts[language]=s
 links=re.findall(r'\[[^\]]*\]\(([^)]+)\)',s);bad=[]
 for link in links:
  if not (f.parent/link.split('#')[0]).is_file():bad.append(link)
 check(language+'_links',not bad,dict(occurrences=len(links),bad=bad))
 plain=re.sub(r'\[[^\]]*\]\([^)]+\)',lambda m:m.group(0).split('](')[0]+']',s)
 check(language+'_no_internal_task_labels',not re.search(r'\b(?:W[1-4]|R\d+|P\d+)\b',plain))
 if language=='en':check('no_chinese_in_english',not re.search(r'[\u3400-\u9fff]',s))
 # Remove URLs before comparing numeric vocabulary; paired text need not be a literal translation.
 texts[language]=plain
numbers=lambda s:set(re.findall(r'(?<![A-Za-z0-9_])\d+(?![A-Za-z0-9_])',s))
check('paired_numeric_vocabulary',numbers(texts['zh'])==numbers(texts['en']),dict(zh=sorted(numbers(texts['zh'])),en=sorted(numbers(texts['en']))))
summary=json.loads((o.parent/'W2/summary.json').read_text())
check('normative_counts',len(summary)==6 and sum(x['runs'] for x in summary)==30 and sum(x['case_observations'] for x in summary)==360)
check('two_architecture_positive_results',all(x['exact_case_observations']==60 for x in summary if x['path']!='drop'))
check('negative_controls_detect_state_loss',all(x['exact_case_observations']==20 for x in summary if x['path']=='drop'))
scope=list(csv.DictReader((o.parent/'W2/STATE_SCOPE.tsv').open(),delimiter='\t'))
check('state_denominator',len(scope)==11 and sum(x['classification'] in ('optional_fields_preserve_initial','mismatch_preserves_output') for x in scope)==7 and sum(x['classification']=='callback_and_move_internal_state' for x in scope)==2)
parts={name:len((o.parent/'W2'/name).read_text().splitlines()) for name in ['bridge.h','backend.cpp','frontend.cpp','state_probe.cpp']}
check('source_line_counts',list(parts.values())==[8,26,22,49],parts)
clean=json.loads((o.parent/'W2/board_cleanup.json').read_text())
check('board_cleanup',clean['directory_absent'] and clean['removed_uploads']==6 and not clean['new_diagnostic_names'])
before=(o.parent/'W2/raw/011_diagnostics_before.stdout').read_text();after=(o.parent/'W2/raw/017_diagnostics_after.stdout').read_text()
names=lambda s:{x.strip() for x in s.splitlines() if x.strip() and not x.startswith('TARGET_EXIT=')}
sentinel='adaptor_positive_control_not_created_on_board'
check('diagnostic_difference_positive_control',names(after)-names(before)==set() and names(before+'\n'+sentinel)-names(before)=={sentinel})
dirty={'stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
check('user_changes_preserved',all(hashlib.sha256((r/'docs/progress/R115/W3/raw'/('033_angle_files.'+suffix)).read_bytes()).hexdigest()==digest for suffix,digest in dirty.items()))
(o/'self_check.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2)+'\n')
for x in checks:print(('PASS' if x['passed'] else 'FAIL'),x['check'],x['detail'])
sys.exit(0 if all(x['passed'] for x in checks) else 1)
