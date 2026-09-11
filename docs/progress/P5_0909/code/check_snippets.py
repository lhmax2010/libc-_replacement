#!/usr/bin/env python3
"""从最终报告抽取片段，显式补声明做语法检查；不链接、不运行、不声称 ABI 通过。"""
import csv
import hashlib
import pathlib
import re
import subprocess
import sys
base=pathlib.Path('docs/progress/P5_0909')
directory=base/'snippets'; directory.mkdir(exist_ok=True)
prefix='''#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <stdexcept>
typedef struct engine_s* engine_handle;
int internal_cpp_function();
struct Widget; struct Config;
extern "C" std::size_t provider_size(const void*);
extern "C" void destroy_payload(void*);
'''
results=[]
for report in ['api_compatibility_report.md','api_compatibility_report_EN.md','abi_comparison_preliminary.md','abi_comparison_preliminary_EN.md']:
    text=(pathlib.Path('HQ_files')/report).read_text()
    for i,m in enumerate(re.finditer(r'^```(c|cpp)\n(.*?)^```',text,re.M|re.S),1):
        language,source=m.groups()
        code=source if language=='c' else prefix+source
        path=directory/f'{report}_{i:02d}.{language}'
        path.write_text(code)
        stem=base/f'raw/syntax/{report}_{i:02d}'
        cmd=[sys.executable,str(base/'code/record.py'),str(stem),'nice','-n','15','ionice','-c','3',
             'gcc' if language=='c' else 'g++','-std=c11' if language=='c' else '-std=c++17',
             '-fsyntax-only','-Wall','-Wextra',str(path)]
        rc=subprocess.run(cmd).returncode
        results.append([report,i,rc,'as_written' if language=='c' else 'explicit_declarations_added',str(path),hashlib.sha256(path.read_bytes()).hexdigest()])
with (base/'SYNTAX_RESULTS.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['report','block','exit','method','snapshot','sha256']);w.writerows(results)
print('SYNTAX_ONLY',len(results),'PASS',sum(r[2]==0 for r in results),'NO_LINK_NO_EXECUTION')
sys.exit(any(r[2]!=0 for r in results))
