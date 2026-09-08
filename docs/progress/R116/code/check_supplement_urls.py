#!/usr/bin/env python3
"""核验新节所有唯一 HTTP 链接，保留逐条命令、状态和退出码。"""
import concurrent.futures, pathlib, re, subprocess, sys
r=pathlib.Path('docs/progress/R116')
zh=(r/'REPORT_ZH.md').read_text().split('## 为什么 Tizen 不采用 Chromium 的方案\n',1)[1].split('\n## 适用性结论',1)[0]
en=(r/'REPORT_EN.md').read_text().split('## Why Tizen does not adopt Chromium\'s approach\n',1)[1].split('\n## Applicability conclusions',1)[0]
extract=lambda s:set(re.findall(r'\]\((https://[^)]+)\)',s))
urls=sorted(extract(zh));assert set(urls)==extract(en)
for section in [zh,en]:
 rows=[line for line in section.splitlines() if line.startswith('|')][2:]
 assert len(rows)==5
 for row in rows:
  columns=row.split('|');assert 'https://' in columns[2] and 'https://' in columns[4]
print('BILINGUAL_URL_SET_AND_FIVE_ROWS=PASS',flush=True)
def check(item):
 i,url=item
 cmd=[sys.executable,'docs/progress/R114/code/record.py',str(r/f'raw/supplement/urls/{i:02}'),
      'nice','-n','15','ionice','-c','3','curl','--fail','--location','--max-time','25','--retry','1',
      '--output','/dev/null','--write-out','HTTP_CODE=%{http_code}\nEFFECTIVE_URL=%{url_effective}\n',url]
 rc=subprocess.run(cmd).returncode
 return url,rc
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:results=list(pool.map(check,enumerate(urls,1)))
for url,rc in results:print('URL_EXIT',rc,url)
assert all(rc==0 for _,rc in results),results
print('ALL_SUPPLEMENT_URLS_ACCESSIBLE=PASS',len(urls))
