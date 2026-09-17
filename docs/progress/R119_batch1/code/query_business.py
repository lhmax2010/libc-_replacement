"""按业务函数名称查冻结全体交集，不要求返回类型出现在修饰名中。"""
from common import *
import csv,json,re
csv.field_size_limit(20_000_000)
query=re.compile(sys.argv[1]);d=OUT/'W1'
meta=json.loads((d/'EXPANDED_SCOPE.json').read_text())
raw=ROOT/meta['demangle_record']
if not raw.with_suffix('.stdout').exists():raw=OUT/'raw'/pathlib.Path(meta['demangle_record']).name
dm=dict(zip((d/'demangle_input.txt').read_text().splitlines(),raw.with_suffix('.stdout').read_text().splitlines()))
rows=[]
with (OLD/'progress/R22/tables/source_edge_classification.tsv').open()as f:
    for i,r in enumerate(csv.DictReader(f,delimiter='\t'),2):
        if r['provider_source_rpm'].startswith(('gcc-','llvm-')):continue
        for symbol in r['cpp_symbols'].split(';'):
            if symbol and query.search(dm[symbol]):
                x={'provider':r['provider_source_rpm'],'consumer':r['consumer_source_rpm'],'symbol':symbol,'demangled':dm[symbol],'source_line':i};rows.append(x);print(json.dumps(x,ensure_ascii=False))
save(d/'queries'/(re.sub(r'[^A-Za-z0-9]+','_',sys.argv[1]).strip('_')+'.json'),rows)
print('MATCHES',len(rows),'LIMIT','冻结 DT_NEEDED 交集，不含所有 dlopen/外部材料；零匹配不是全平台无消费方。')
