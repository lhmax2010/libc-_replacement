"""Read-only trace audit after the one packaging reproduction."""
import collections, json, re
from pathlib import Path
b = Path('progress/ARM_RPM_DIAG')
assert (b/'raw/045_reproduce.exitcode').exists()
paths = {'host': b/'host.strace', 'guest': Path('tmp/ARM_RPM_DIAG/guest-rpmbuild.strace')}
summary = {}
for kind, path in paths.items():
    counts = collections.Counter(); last = collections.defaultdict(lambda: collections.deque(maxlen=12))
    tail = collections.deque(maxlen=400)
    with (b/(kind+'-error-calls.tsv')).open('w') as out:
        out.write('trace_line\tcategory\tactual_trace_line\n')
        for number, line in enumerate(path.open(errors='replace'), 1):
            text = line.rstrip('\n'); tail.append(f'{number}: {text}')
            categories = []
            if re.search(r'\bENOSYS\b|errno=38\b', text): categories.append('ENOSYS')
            if re.search(r'\b(?:EPERM|EACCES|ENOSPC|EIO|ENOMEM)\b|errno=(?:1|13|28|5|12)\b', text): categories.append('OTHER_SELECTED_ERROR')
            if re.search(r'\b(?:write|pwrite64|mknod|mknodat|link|linkat|setxattr|lsetxattr|fsetxattr|fallocate)\(', text) and re.search(r'= -1\b', text): categories.append('SELECTED_WRITE_OPERATION_ERROR')
            if 'create archive failed' in text: categories.append('ARCHIVE_DIAGNOSTIC')
            for category in categories:
                counts[category] += 1; last[category].append({'line':number, 'text':text})
                out.write(f'{number}\t{category}\t{text}\n')
    (b/(kind+'-final-400-lines.txt')).write_text('\n'.join(tail)+'\n')
    summary[kind] = {'path':str(path), 'lines':number, 'bytes':path.stat().st_size, 'counts':dict(counts), 'last_matches':{k:list(v) for k,v in last.items()}}
(b/'trace-audit.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
