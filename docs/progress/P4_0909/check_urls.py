#!/usr/bin/env python3
"""逐条记录 GET 原文命令、HTTP 状态与退出码，至多两个请求；不把 200 当作内容证明。"""
import concurrent.futures,csv,pathlib,subprocess,sys
base=pathlib.Path('docs/progress/P4_0909')
rows=list(csv.DictReader((base/'URL_TARGETS.tsv').open(),delimiter='\t'))
def check(item):
    i,row=item; stem=base/f'raw/urls/{i:03d}'
    cmd=[sys.executable,'docs/progress/R114/code/record.py',str(stem),'nice','-n','15','ionice','-c','3',
         'curl','--location','--max-time','25','--retry','1','--fail','--output','/dev/null',
         '--write-out','HTTP_CODE=%{http_code}\nEFFECTIVE_URL=%{url_effective}\n',row['url']]
    rc=subprocess.run(cmd).returncode
    return [row['url'],row['in_added_section'],rc,pathlib.Path(str(stem)+'.stdout').read_text().strip()]
with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool: result=list(pool.map(check,enumerate(rows,1)))
with (base/'URL_RESULTS.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t'); w.writerow(['url','in_added_section','curl_exit','http_output']); w.writerows(result)
print('REQUESTS',len(result),'SUCCESS',sum(x[2]==0 for x in result),'FAILED',sum(x[2]!=0 for x in result))
print('ADDED_SECTION',sum(x[1]=='True' for x in result),'FAILED',sum(x[1]=='True' and x[2]!=0 for x in result))
