#!/usr/bin/env python3
"""机械抽取示例做语法检查，不链接、不执行；补头控制不代表 ABI 验证。"""
import csv,hashlib,pathlib,re,subprocess,sys
b=pathlib.Path('docs/progress/P4_0909'); d=b/'snippets'; d.mkdir(exist_ok=True)
text=pathlib.Path('HQ_files/api_compatibility_report.md').read_text()
prefix='#include <cstddef>\n#include <cstdio>\n#include <string>\n#include <vector>\n#include <map>\n#include <deque>\n#include <memory>\n#include <stdexcept>\ntypedef struct engine_s* engine_handle;\nint internal_cpp_function();\nstruct Widget; struct Config;\n'
records=[]
for i,m in enumerate(re.finditer(r'^```([^\n]*)\n(.*?)^```',text,re.M|re.S),1):
    lang=m.group(1)
    if lang not in ['c','cpp']:continue
    src=m.group(2); ext='c' if lang=='c' else 'cpp';compiler='gcc' if lang=='c' else 'g++'
    for variant,code in [('original',src),('headers_control',('#include <stddef.h>\n' if lang=='c' else prefix)+src)]:
        p=d/f'api_{i:02d}_{variant}.{ext}';p.write_text(code)
        stem=b/f'raw/syntax/api_{i:02d}_{variant}'
        cmd=[sys.executable,'docs/progress/R114/code/record.py',str(stem),'nice','-n','15','ionice','-c','3',compiler,
             '-std=c11' if lang=='c' else '-std=c++17','-fsyntax-only','-Wall','-Wextra',str(p)]
        rc=subprocess.run(cmd).returncode
        records.append([i,text[:m.start()].count('\n')+1,variant,rc,str(p),hashlib.sha256(p.read_bytes()).hexdigest()])
with (b/'SYNTAX_RESULTS.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['block','line','variant','exit','source','sha256']);w.writerows(records)
print('SYNTAX_ONLY_CHECKS',len(records),'NO_LINK_NO_EXECUTION=YES')
print('Raw fragments with placeholders or file-scope statements are not complete translation units; see manual CODE_REVIEW.md.')
