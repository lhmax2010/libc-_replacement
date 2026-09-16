from common import *
import gzip,collections
names=collections.defaultdict(set);kinds=collections.Counter()
for line in gzip.open(OUT/'headers/declarations.jsonl.gz','rt'):
 d=json.loads(line);names[d['entity']].add(d['library']);kinds[d['cursor_kind']]+=1
print('entity_names',len(names),'cursor_counts',dict(kinds))
for name in ['std::vector','std::string','std::is_same_v','std::ranges::sort','std::ranges::views::all','std::execution::seq','std::result_of','std::same_as','__gnu_cxx::__versa_string','__gnu_pbds::tree']:
 print(name,sorted(names.get(name,[])))
status=json.loads((OUT/'headers/PARSE_STATUS.json').read_text())
print('parse_status',dict(collections.Counter(s['status'] for s in status)))
bad=collections.defaultdict(set)
for s in status:
 if s['status']!='PARSED':bad[s['library']].add(s['header'])
print('bad_headers',{k:sorted(v) for k,v in bad.items()})
