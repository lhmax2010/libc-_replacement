#!/usr/bin/env python3
"""只读正文核验并生成证据索引；不运行示例命令。"""
import csv,hashlib,pathlib,re
base=pathlib.Path('docs/progress/IMPL_0909/W3')
rows=[]
for name in ['SUPPORT_SCOPE_ZH.md','SUPPORT_SCOPE_EN.md','SELF_VERIFY_ZH.md','SELF_VERIFY_EN.md','SELF_VERIFY_COMMANDS.md']:
    p=base/name;text=p.read_text()
    for link in re.findall(r'\]\(([^)]+)\)',text):
        if link.startswith(('http://','https://')):continue
        q=(p.parent/link.split('#',1)[0]).resolve()
        assert q.exists(),(name,link)
        rows.append([name,link,hashlib.sha256(q.read_bytes()).hexdigest() if q.is_file() else 'DIRECTORY'])
    if name.startswith('SUPPORT_SCOPE'):
        for token in ['LLVM_22_TIZEN_1','11,402','7,848','3,554','235','81','NOT_OBSERVED','NOT_AVAILABLE','C++03']:
            assert token in text,(name,token)
    print('DOCUMENT_SHA256',hashlib.sha256(p.read_bytes()).hexdigest(),p)
with (base/'EVIDENCE_INDEX.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['document','reference','sha256']);w.writerows(rows)
print('LOCAL_LINKS_VALID',len(rows),'BILINGUAL_COVERAGE_TOKENS=PASS')
print('EXAMPLES_EXECUTED=NO; ARM_CURRENT_GAP=3554; reviewer_version_not_full_delivery')
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
