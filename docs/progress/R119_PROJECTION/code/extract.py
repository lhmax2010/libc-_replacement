#!/usr/bin/env python3
"""按 SHA+行去重后展开该行所有限定名；完整实参保存，解析歧义硬标未定。"""
from common import *
import bisect, collections, functools, re
VERIFIED={}

def verify(path,expected):
    if path not in VERIFIED:
        actual=sha(path)
        if actual!=expected:raise RuntimeError('SOURCE_SHA_CHANGED '+path)
        VERIFIED[path]=actual
    if VERIFIED[path]!=expected:raise RuntimeError('INCONSISTENT_EXPECTED_SHA '+path)

def mask(text):
    # 保留每个字符位置与换行。处理 raw strings 先于普通注释/字符串。
    pattern=r'R"([^ ()\\\t\r\n]{0,16})\(.*?\)\1"|/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern,lambda m: ''.join('\n' if x=='\n' else ' ' for x in m.group()),text,flags=re.S)

def arguments(code,start,original=None):
    original=code if original is None else original
    if start>=len(code) or code[start]!='<':return None,start,'NO_TEMPLATE_ARGUMENT_LIST'
    # Track brackets and parentheses separately. Comparisons inside parentheses
    # cannot accidentally close an outer template argument list.
    angle=1; stack=[];pieces=[];last=start+1;i=start+1
    while i<len(code):
        ch=code[i]
        if ch in '([{':stack.append(ch)
        elif ch in ')]}':
            expected={')':'(',']':'[','}':'{'}[ch]
            if not stack or stack[-1]!=expected:return None,i,'UNBALANCED_DELIMITER'
            stack.pop()
        elif not stack:
            if ch=='<':angle+=1
            elif ch=='>':
                angle-=1
                if angle==0:
                    pieces.append(original[last:i].strip());return pieces,i+1,'BALANCED_LEXICAL_ONLY'
            elif ch==',' and angle==1:pieces.append(original[last:i].strip());last=i+1
            elif ch in ';{}' and angle>0:return None,i,'STATEMENT_BEFORE_TEMPLATE_CLOSE'
        i+=1
    return None,i,'NO_TEMPLATE_CLOSE'

@functools.lru_cache(maxsize=12)
def source(path,expected):
    data=pathlib.Path(path).read_bytes();actual=hashlib.sha256(data).hexdigest()
    if actual!=expected:raise RuntimeError('SOURCE_SHA_CHANGED '+path)
    text=data.decode(errors='replace');code=mask(text)
    starts=[0]+[m.end() for m in re.finditer('\n',code)]
    return text,code,starts

def matches(name,code,start,end):
    pattern=r'\bstd\s*::\s*(?:(?:__1|__cxx11)\s*::\s*)?'+r'\s*::\s*'.join(map(re.escape,name[5:].split('::')))+r'\b'
    result=[]
    for m in re.compile(pattern).finditer(code,start):
        if m.start()>=end:break
        result.append(m)
    return result

def control():
    text='// std::enable_if<X>::type;\nstd::enable_if<(N > 2), std::vector<int>>::type f(); std::enable_if<true>::type g();\n'
    c=mask(text); found=matches('std::enable_if',c,0,len(c));assert len(found)==2
    a=[]
    for m in found:
        p=m.end()
        while c[p].isspace():p+=1
        args,e,status=arguments(c,p);a.append(args)
    assert a==[['(N > 2)','std::vector<int>'],['true']],a
    assert not matches('std::is_same',mask('"std::is_same<int,int>" // std::is_same<T,T>'),0,80)
    save(OUT/'EXTRACTION_CONTROLS.json',dict(input=text,actual=a,expected=a,comment_string_exclusion=True,nested_parenthesized_comparison=True,limits='词法平衡不等于 C++ AST；未编译的表达式不冒充已解析具体类型。'))

def run(end):
    control(); cs=candidates();stat=[];global_start=time.time()
    for ordinal,c in enumerate(cs[:end],1):
        rank=int(c['rank']); dest=OUT/'occurrences'/f'{rank:03}.json.gz'
        if dest.exists() and '--refresh' not in sys.argv:
            stat.append(json.loads((OUT/f'counts/{rank:03}.json').read_text()));continue
        gate(f'extract_gate_{ordinal:02}');begin=time.time()
        with gzip.open(OUT/f'inventory/{rank:03}.json.gz','rt') as f: rows=json.load(f)
        groups=collections.defaultdict(list)
        for r in rows:
            verify(r['cache_path'],r['sha256'])
            groups[(r['sha256'],int(r['line']))].append(r)
        output=[]; misses=0
        for (digest,line),refs in groups.items():
            first=refs[0];text,code,starts=source(first['cache_path'],digest)
            if line>len(starts):raise RuntimeError('LINE_OUTSIDE_FILE')
            begin_line=starts[line-1];end_line=starts[line] if line<len(starts) else len(code)
            found=matches(c['candidate'],code,begin_line,end_line)
            if not found:
                # 旧索引词法与本次 raw-string/位置处理不一致的记录仍显式落盘。
                misses+=1;output.append(dict(id=f'{rank}:{digest}:{line}:NONE',rank=rank,candidate=c['candidate'],line=line,column=0,template_arguments=None,parse_status='INDEX_LOCATION_NOT_REPRODUCED',result='RESULT_UNRESOLVED',reason='旧限定名位置未由本次保留字符位置的词法器复现；不丢弃该行。',references=refs));continue
            for m in found:
                p=m.end()
                while p<len(code) and code[p].isspace():p+=1
                args,e,status=arguments(code,p,text)
                tail=code[e:e+240]; suffix=re.match(r'\s*::\s*([A-Za-z_]\w*)',tail)
                prefix=code[max(0,m.start()-100):m.start()]
                foreign=bool(re.search(r'[A-Za-z_]\w*\s*::\s*$',prefix))
                context_start=starts[max(0,line-5)];context_end=starts[min(len(starts)-1,line+4)] if line+4<len(starts) else len(text)
                arg_original=text[p:e] if args is not None else ''
                output.append(dict(id=f'{rank}:{digest}:{line}:{m.start()-begin_line+1}',rank=rank,candidate=c['candidate'],line=line,column=m.start()-begin_line+1,template_arguments=args,raw_template_arguments=arg_original,template_expression=text[m.start():e] if args is not None else text[m.start():m.end()],suffix_member=suffix[1] if suffix else '',following=tail[:160].strip(),parse_status=status,foreign_qualification=foreign,context=text[context_start:context_end],references=refs))
        dest.parent.mkdir(parents=True,exist_ok=True)
        with gzip.open(dest,'wt') as f:json.dump(output,f,ensure_ascii=False)
        row=dict(ordinal=ordinal,rank=rank,candidate=c['candidate'],index_rows=len(rows),unique_sha_line=len(groups),expanded_occurrences=len(output),location_unreproduced=misses,elapsed_seconds=time.time()-begin)
        save(OUT/f'counts/{rank:03}.json',row);stat.append(row);print(row,flush=True)
    tsv(OUT/'EXTRACTION_PROGRESS.tsv',stat)
    save(OUT/f'VERIFIED_HEADERS_extract_{end:02}.json',VERIFIED)
    save(OUT/'EXTRACTION_STATUS.json',dict(completed_candidates=len(stat),requested_end=end,of_total=65,elapsed_this_run=time.time()-global_start,index_rows=sum(r['index_rows'] for r in stat),expanded_occurrences=sum(r['expanded_occurrences'] for r in stat),no_silent_limit=True))
if __name__=='__main__':run(int(sys.argv[1]))
