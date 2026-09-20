#!/usr/bin/env python3
"""保存静态投影规则依据，不使用旧计划中的例化表达式。"""
from common import *
import re
def main():
    rules={};libdefs={lib:json.loads((OLD/f'A_DEFINITIONS_{lib}.json').read_text()) for lib in ['gnu','cxx']}
    for c in candidates():
        name=c['candidate'];rank=c['rank']
        if not name.startswith('std::is_') and name not in ('std::enable_if','std::enable_if_t','std::conditional','std::conditional_t','std::void_t'):continue
        defs={}
        for lib,byrank in libdefs.items():
            allrows=byrank[rank];needle=name.split('::')[-1]
            declarations=[r for r in allrows if re.search(r'\b(?:struct|class|using)\s+(?:__attribute__\s*\(.*?\)\s*)?'+needle+r'\b',r['preprocessed'])]
            if not declarations:declarations=allrows
            # 保存全部匹配定义，不限制前三条。每个真实源路径摘要硬核对。
            for r in declarations:
                if sha(r['path'])!=r['sha256']:raise RuntimeError('definition source changed')
            defs[lib]=declarations
        rules[name]=dict(rank=int(rank),kind='BOOL_VALUE' if name.startswith('std::is_') else 'TYPE_PROJECTION',definitions=defs,scope='仅标准库主模板约定内的 ::value 布尔查询或公开类型投影；不把查询值猜成 true/false，不代填实际参数。')
    extra={}
    paths={
      'gnu':'tmp/API_0911/W1_CONT/gnu14_sdk/usr/lib64/gcc/x86_64-tizen-linux-gnu/14.2.0/include/c++/type_traits',
      'cxx':'tmp/IMPL_0908/build-native/include/c++/v1/__type_traits/integral_constant.h'}
    for lib,rel in paths.items():
        p=ROOT/rel;lines=p.read_text().splitlines();found=[]
        for i,s in enumerate(lines):
            if ('struct integral_constant' in s or 'using bool_constant' in s or 'using __bool_constant' in s or 'using _BoolConstant' in s or 'typedef integral_constant<bool' in s):
                found.append('\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-2),min(len(lines),i+20))))
        assert found
        extra[lib]=dict(path=rel,sha256=sha(p),excerpts=found)
    save(OUT/'STATIC_RULE_DEFINITIONS.json',dict(rules=rules,bool_value_root=extra))
    for name,entry in rules.items():
        print(name)
        for lib,rows in entry['definitions'].items():
            for r in rows:
                print(lib,r['path'],r['line'],r['preprocessed']); print(r['source'])
if __name__=='__main__':main()
