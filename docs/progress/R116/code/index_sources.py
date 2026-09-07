#!/usr/bin/env python3
"""从实际下载命令生成来源索引；不把失败下载列为取得的源码。"""
import pathlib, shlex, hashlib, csv
r=pathlib.Path('docs/progress/R116');rows=[]
for p in sorted((r/'raw/fetch').glob('*.command.txt')):
 name=p.name.removesuffix('.command.txt')
 cmd=p.read_text().split('command: ',1)[1];args=shlex.split(cmd)
 url=args[-1];rc=(p.parent/(name+'.exitcode')).read_text().strip()
 target=r/'sources'/(name+'.txt')
 rows.append([name,url,rc,str(target) if target.exists() else 'NOT_AVAILABLE',hashlib.sha256(target.read_bytes()).hexdigest() if target.exists() else 'NOT_AVAILABLE'])
with (r/'source_index.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['name','url','curl_exit','decoded_snapshot','sha256']);w.writerows(rows)
print('indexed_fetches',len(rows),'successful',sum(x[2]=='0' for x in rows))
