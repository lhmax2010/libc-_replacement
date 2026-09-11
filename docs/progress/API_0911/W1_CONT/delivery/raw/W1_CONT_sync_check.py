#!/usr/bin/env python3
"""只读核对：只有本地 P9 文件逐字节等于合并树，才允许另行同步 Git 元数据。"""
import pathlib,subprocess,json,hashlib
r=pathlib.Path.cwd();old='3a773714d85b266cd9141460577ecdb827fa4a34';merged='cf0045ca89f390ba2275aec5bc407bf3ad73b1a0'
def git(*a):return subprocess.check_output(['git',*a])
assert git('rev-parse','HEAD').decode().strip()==old
assert not git('diff','--cached','--name-only')
assert subprocess.run(['git','merge-base','--is-ancestor',old,merged]).returncode==0
changes=git('diff','--name-status',old,merged).decode().splitlines();rows=[]
for line in changes:
 status,path=line.split('\t');assert status=='A' and path.startswith('docs/progress/P9_0911/'),line
 p=r/path;blob=git('show',merged+':'+path)
 rows.append(dict(path=path,exists=p.is_file(),equal=p.is_file() and p.read_bytes()==blob))
result=dict(old=old,merged=merged,added_files=len(rows),all_existing_bytes_equal=all(x['equal'] for x in rows),differences=[x for x in rows if not x['equal']])
(r/'docs/progress/API_0911/delivery/W1_CONT_sync_check.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
