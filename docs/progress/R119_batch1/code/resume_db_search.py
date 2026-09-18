#!/usr/bin/env python3
"""只读全量符号缓存的函数身份查询（含返回类型未编码的入口）。"""
import json
import pathlib
import re
import sqlite3
import sys

root=pathlib.Path(__file__).resolve().parents[4]
db=sqlite3.connect('file:'+str(root/'tmp/R119_batch1/full_elf/symbols.sqlite3')+'?mode=ro',uri=True)
pattern=re.compile(sys.argv[1])
meta={i:json.loads(m) for i,m in db.execute('SELECT id,metadata FROM elf WHERE status="PASS"')}
names=[n for n, in db.execute('SELECT DISTINCT name FROM normalized_symbol') if pattern.search(n)]
for name in names:
    rows=list(db.execute('SELECT elf_id,ndx FROM normalized_symbol WHERE name=?',(name,)))
    und=[i for i,s in rows if s=='UND']
    defs=[i for i,s in rows if s!='UND']
    if len(sys.argv)>2 and sys.argv[2]=='used' and not und:
        continue
    print('SYMBOL',name)
    for label,ids in [('UND',und),('DEF',defs)]:
        for i in sorted(set(ids))[:15]:
            print(label,i,meta[i]['sourcerpm'],meta[i]['path'])
    print('UND_COUNT',len(set(und)),'DEF_COUNT',len(set(defs)))
print('MATCHED_NAMES',len(names),'限定查询；不据零命中判定无消费方。')
