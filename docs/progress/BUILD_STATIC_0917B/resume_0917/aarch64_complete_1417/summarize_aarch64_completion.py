import collections
import json
import re
from pathlib import Path

out=Path('progress/BUILD_STATIC_0917B/resume_0917')
counts={}
for label,name in [('first','llvm-aarch64.stopped-133701.log'),
                   ('duplicate','llvm-aarch64.stopped-134423.log'),
                   ('final','llvm-aarch64.log')]:
    lines=(out/name).read_text(errors='replace').splitlines()
    targets=[line for line in lines if re.match(r'^\[\d+/\d+\]',line)]
    counts[label]={'log':name,'announced_targets':len(targets),
        'object_compiles':sum(bool(re.search(r'\] Building (?:CXX|C|ASM) object ',line)) for line in targets),
        'written_rpms':[line.removeprefix('Wrote: ') for line in lines if line.startswith('Wrote: ')],
        'missing_build_id_warning_lines':sum('warning: Missing build-id' in line for line in lines)}
(out/'aarch64_execution_counts.json').write_text(json.dumps(counts,indent=2))
print(json.dumps(counts,indent=2))
