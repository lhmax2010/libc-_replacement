"""扩大至冻结 R22 全部候选边，不再限于 R26 的切分边。"""
from common import *
import csv,re,gzip,json,collections,shlex
csv.field_size_limit(20_000_000)
d=OUT/'W1'; p=OLD/'progress/R22/tables/source_edge_classification.tsv'
with p.open()as f:src=list(csv.DictReader(f,delimiter='\t'))
symbols=sorted({s for r in src for s in r['cpp_symbols'].split(';')if s})
inp=d/'demangle_input.txt';inp.write_text('\n'.join(symbols)+'\n')
rc,txt,raw=record('expanded_demangle',['bash','-c','c++filt < '+shlex.quote(str(inp))]);assert rc==0
dm=dict(zip(symbols,txt.splitlines()));assert len(dm)==len(symbols)
names={}
with (ROOT/'docs/progress/P7_0909/inputs/source_package_records.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):names[r['location']]=r['name']
with (d/'BATCH_100.tsv').open()as f:entities=[r['entity']for r in csv.DictReader(f,delimiter='\t')]
alias={'string':'basic_string<char,','wstring':'basic_string<wchar_t,','istream':'basic_istream<char,','ostream':'basic_ostream<char,','ifstream':'basic_ifstream<char,','ofstream':'basic_ofstream<char,','stringstream':'basic_stringstream<char,','ostringstream':'basic_ostringstream<char,','istringstream':'basic_istringstream<char,','streambuf':'basic_streambuf<char,','ios':'basic_ios<char,','string_view':'basic_string_view<char,'}
patterns={n:re.compile(re.escape(n)+r'(?![A-Za-z_0-9])')for n in entities}
def match(s):
    s=s.replace('std::__cxx11::','std::').replace('std::__1::','std::').replace('std::filesystem::__cxx11::','std::filesystem::').replace('std::chrono::_V2::','std::chrono::')
    return[n for n,q in patterns.items()if q.search(s)or n[5:]in alias and 'std::'+alias[n[5:]]in s]
assert match('fn(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)')
assert not match('fn(std::mapper&)')
biz=collections.defaultdict(list);allrows=[];seen=set()
for i,r in enumerate(src,2):
    runtime=names.get(r['provider_source_rpm'])in ['gcc','llvm']
    for sym in r['cpp_symbols'].split(';'):
        if not sym:continue
        text=dm[sym]
        for n in match(text):
            row=dict(entity=n,consumer_source=names.get(r['consumer_source_rpm'],'NOT_OBSERVED'),consumer_sourcerpm=r['consumer_source_rpm'],provider_source=names.get(r['provider_source_rpm'],'NOT_OBSERVED'),provider_sourcerpm=r['provider_source_rpm'],runtime_provider=str(runtime),raw_symbol=sym,demangled=text,source_table=str(p),source_line=i)
            allrows.append(row)
            if runtime or text.startswith(('std::','typeinfo ','vtable ','VTT ','guard variable ','non-virtual thunk to std::','virtual thunk to std::')):continue
            key=(n,row['provider_source'],row['consumer_source'],sym)
            if key not in seen:biz[n].append(row);seen.add(key)
save(d/'EXPANDED_BUSINESS_QUEUE.json',biz)
with gzip.open(d/'expanded_symbol_edges.tsv.gz','wt',newline='')as f:
    w=csv.DictWriter(f,fieldnames=list(allrows[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(allrows)
save(d/'EXPANDED_SCOPE.json',{'source':str(p),'sha256':digest(p),'source_edges':len(src),'unique_symbols':len(symbols),'matched_rows':len(allrows),'input_sha256':digest(inp),'demangle_record':raw,'positive_control':'std::__cxx11::basic_string<char,...>','negative_control':'std::mapper','limit':'R22 的 phase_nodes 及唯一解析 DT_NEEDED 边；不包括全部 dlopen / 外部产物，不把未命中等同无消费方'})
old=json.loads((d/'BUSINESS_SYMBOL_QUEUE.json').read_text())
for n in entities:
    added=[r for r in biz[n]if (r['provider_source'],r['consumer_source'],r['raw_symbol'])not in {(e['provider_source'],e['consumer_source'],e['raw_symbol'])for e in old.get(n,[])}]
    print(n,'business_rows',len(biz[n]),'additional_rows',len(added))
    if not old.get(n):
        for r in added[:3]:print(' NEW',r['provider_source'],'->',r['consumer_source'],r['demangled'])
