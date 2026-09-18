#!/usr/bin/env python3
"""保存 Json::Reader 真成员与实际调用点，不从模板名字推断。"""
from common import *
import json
p=json.loads((OUT/'W1/positive/json_reader_full.json').read_text())
h=pathlib.Path(p['header_matches'][0]['metadata']['cache_path'])
src=pathlib.Path(p['source_includes'][0]['root'])/'unpacked'
cc=list(src.rglob('src/xwalk/xwalk_extension.cc'))
assert len(cc)==1
cc=cc[0]
items=[(h,['using Errors = std::deque<ErrorInfo>','using Nodes = std::stack<Value*>','Nodes nodes_','Errors errors_']),
       (h.parent/'json.h',['#include "reader.h"']),
       (cc.with_suffix('.h'),['#include <json/json.h>']),
       (cc,['#include "xwalk/xwalk_extension.h"','Json::Reader reader;','reader.parse(msg, msg + strlen(msg), args)'])]
rows=[]
for path,tokens in items:
    lines=path.read_text().splitlines()
    hits=[]
    for token in tokens:
        found=[i for i,line in enumerate(lines) if token in line]
        assert found,(str(path),token)
        for i in found:
            hits.append('\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-2),min(len(lines),i+3))))
    rows.append(dict(path=str(path),sha256=digest(path),excerpts=hits))
    print(path,*hits,sep='\n')
alias=h.parents[2]/'json'
assert alias.is_symlink() and alias.resolve()==h.parent.resolve(),str(alias)
rows.append(dict(include_alias=str(alias),target=str(alias.readlink()),resolved=str(alias.resolve())))
save(OUT/'W1/positive/json_reader_full_include_chain.json',rows)
