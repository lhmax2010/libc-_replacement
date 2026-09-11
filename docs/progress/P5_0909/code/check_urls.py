#!/usr/bin/env python3
"""逐一验证修改后六稿所有 HTTP 链接；最多两个请求，保存每次 GET 的原命令和退出码。"""
import concurrent.futures
import csv
import pathlib
import re
import subprocess
import sys

base = pathlib.Path('docs/progress/P5_0909')
occurrences = []
for path in sorted(pathlib.Path('HQ_files').glob('*.md')):
    for number, line in enumerate(path.read_text().splitlines(), 1):
        for target in re.findall(r'\]\(([^)]+)\)', line):
            occurrences.append([path.name, number, target])
with (base / 'FINAL_LINK_OCCURRENCES.tsv').open('w') as stream:
    writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
    writer.writerow(['file', 'line', 'target'])
    writer.writerows(occurrences)
assert all(x[2].startswith('https://') for x in occurrences)
assert re.findall(r'\]\(([^)]+)\)', '[正向对照](missing.md)') == ['missing.md']
targets = sorted({x[2] for x in occurrences})
def check(item):
    number, url = item
    stem = base / f'raw/urls/{number:03d}'
    rc = subprocess.run([sys.executable, str(base / 'code/record.py'), str(stem),
        'nice', '-n', '15', 'ionice', '-c', '3', 'curl', '--location', '--max-time', '25',
        '--retry', '1', '--fail', '--output', '/dev/null', '--write-out',
        'HTTP_CODE=%{http_code}\nEFFECTIVE_URL=%{url_effective}\n', url]).returncode
    return [url, rc, pathlib.Path(str(stem) + '.stdout').read_text().strip()]
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
    results = list(pool.map(check, enumerate(targets, 1)))
with (base / 'URL_RESULTS.tsv').open('w') as stream:
    writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
    writer.writerow(['url', 'curl_exit', 'http_output'])
    writer.writerows(results)
print('TOTAL_OCCURRENCES', len(occurrences), 'UNIQUE', len(results), 'FAILURES', sum(r[1] != 0 for r in results))
sys.exit(any(r[1] != 0 for r in results))
