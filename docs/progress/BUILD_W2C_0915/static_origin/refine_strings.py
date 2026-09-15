"""Reclassify recorded strings without executing assets or discarding initial matches."""
import json
import re
from pathlib import Path
p=Path('progress/BUILD_W2C_0915/static_origin')
records=[json.loads(line) for line in (p/'commands.jsonl').read_text().splitlines()]
results=[]
for record in records:
    if not record['command'].startswith('strings -a '):continue
    arch='armv7l' if '/arm/' in record['command'] else 'aarch64'
    lines=record['stdout'].splitlines()
    patterns={
        'numeric_version_literal':r'^(?:v)?\d+\.\d+\.\d+(?:[-+][A-Za-z0-9_.+-]+)?$',
        'version_description':r'(?i)^(?:bpftrace|clang|LLVM|GCC|GNU C\+\+)\s+(?:version\s+)?v?\d+\.',
        'gnu_runtime_family':r'__cxx11|GLIBCXX_|__gnu_cxx|libstdc\+\+',
        'libcxx_runtime_family':r'NSt3__1|std::__1|libc\+\+',
    }
    groups={key:[line for line in lines if re.search(pattern,line)] for key,pattern in patterns.items()}
    results.append(dict(arch=arch,groups=groups,counts={k:len(v) for k,v in groups.items()}))
assert len(results)==2
(p/'additional_string_evidence.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
for item in results:
    print(json.dumps(dict(arch=item['arch'],counts=item['counts'],
        samples={key:vals[:12] for key,vals in item['groups'].items()}),ensure_ascii=False))
