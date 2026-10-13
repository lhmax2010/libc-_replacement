"""Parse exact report status words, cross-check text, queue browser downloads."""
import csv
import collections
import html
import json
import pathlib
import re
import urllib.parse

out = pathlib.Path(__file__).resolve().parent
work = pathlib.Path('tmp/QB_1194606')
base = 'https://quickbuild.tizen.org/download/1194606/html/GBS%20Reports@%5E@standard-armv7l:aarch64:x86_64/index.html'
source = (work / 'downloads/report_index_response.html').read_text()
details = source.split('<h3>Details</h3>', 1)[1]
rows = []
arches = ['armv7l', 'aarch64', 'x86_64']
for tr in re.findall(r'<tr>(.*?)</tr>', details, re.S):
    cells = re.findall(r'<td>(.*?)</td>', tr, re.S)
    if not cells:
        continue
    assert len(cells) == 4
    package = html.unescape(cells[0].strip())
    for arch, cell in zip(arches, cells[1:]):
        status = html.unescape(re.sub('<[^>]+>', '', cell)).strip()
        link = re.search(r'href="([^"]+)"', cell)
        url = urllib.parse.urljoin(base, html.unescape(link[1])) if link else 'NOT_AVAILABLE'
        rows.append(dict(package=package, architecture=arch, status=status, log_url=url))
assert len({(r['package'], r['architecture']) for r in rows}) == len(rows)
text_rows = {}
for line in (work / 'downloads/report_index.txt').read_text().split('Details\n', 1)[1].splitlines()[1:]:
    cells = line.split('\t')
    if len(cells) == 4:
        text_rows[cells[0]] = cells[1:]
assert len(text_rows) * 3 == len(rows)
for row in rows:
    assert text_rows[row['package']][arches.index(row['architecture'])] == row['status']
with (out / 'BUILD_STATUS.tsv').open('w') as f:
    w = csv.DictWriter(f, fieldnames=list(rows[0]), delimiter='\t'); w.writeheader(); w.writerows(rows)
md = ['| 包名 | armv7l | aarch64 | x86_64 |', '|---|---|---|---|']
md += ['| ' + ' | '.join([p, *s]) + ' |' for p, s in text_rows.items()]
(out / 'BUILD_STATUS.md').write_text('\n'.join(md) + '\n')
summary = {a: dict(collections.Counter(r['status'] for r in rows if r['architecture'] == a)) for a in arches}
(out / 'STATUS_COUNTS.json').write_text(json.dumps(summary, indent=2) + '\n')
plan = []
for row in rows:
    if row['status'] in ('failed', 'unresolvable') or (row['package'] in ('llvm', 'libcxx-runtimes', 'bpftrace') and row['status'] == 'succeeded'):
        assert row['log_url'] != 'NOT_AVAILABLE'
        name = f"logs/{row['package']}/{row['architecture']}.log"
        job = {'action': 'download', 'url': row['log_url'], 'name': name}
        queue_name = f'{100 + len(plan):03d}_log.json'
        dest = work / 'commands' / queue_name
        assert not dest.exists()
        dest.write_text(json.dumps(job, indent=2) + '\n')
        plan.append(dict(**row, path=str(work / 'downloads' / name), command=queue_name))
(out / 'DOWNLOAD_PLAN.json').write_text(json.dumps(plan, indent=2) + '\n')
print(json.dumps({'packages': len(text_rows), 'cells': len(rows), 'counts': summary, 'queued_logs': len(plan)}, indent=2))
