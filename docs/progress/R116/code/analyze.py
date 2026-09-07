#!/usr/bin/env python3
import collections,csv,pathlib,re,hashlib,json
r=pathlib.Path('docs/progress/R116'); rows=list(csv.DictReader((r/'measurements.tsv').open(),delimiter='\t'))
assert len(rows)==150
groups=collections.defaultdict(list)
for row in rows:
 assert row['provider_assert']=='True' and row['status'] not in ['MISMATCH','PROVIDER_NOT_CONFIRMED'],row
 groups[tuple(row[k] for k in ['provider','abi_pair','probe','mode'])].append(row)
with (r/'measurement_summary.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['provider','abi_pair','probe','mode','runs','actual_exits'])
 for key,items in groups.items():
  assert sorted(int(x['repeat']) for x in items)==[1,2,3,4,5]
  rc=','.join(sorted(set(x['actual_exit'] for x in items)));w.writerow([*key,5,rc]);print(*key,'5/5',rc)
def symbols(filename):
 result=set()
 for line in (r/'raw'/filename).read_text().splitlines():
  fields=line.split()
  if len(fields)>=8 and fields[0].rstrip(':').isdigit() and fields[3] in ['FUNC','IFUNC','OBJECT'] and fields[6]!='UND':
   name=fields[7];pair=re.split(r'@@?',name);result.add((pair[0],pair[1] if len(pair)>1 else 'UNVERSIONED'))
 return result
platform=symbols('036_platform_symbols.stdout');experiment=symbols('037_experiment_symbols.stdout')
with (r/'symbol_comparison.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['symbol','version','platform_sysroot','experiment'])
 for pair in sorted(platform|experiment):w.writerow([*pair,int(pair in platform),int(pair in experiment)])
print('platform_versioned_symbols=',len(platform),'experiment_versioned_symbols=',len(experiment),'platform_pairs_missing_in_experiment=',len(platform-experiment))
for pair in sorted(platform-experiment):print('MISSING_PAIR',*pair)
required=['_Unwind_Backtrace','_Unwind_ForcedUnwind','_Unwind_GetCFA','_Unwind_GetIP','_Unwind_Resume','__gcc_personality_v0']
for name in required:
 print('GLIBC_REQUIRED',name,sorted(v for n,v in experiment if n==name));assert any(n==name for n,v in experiment)
deps=(r/'sources/deps_pinned.txt').read_text()
for name,value in [('libcxx_revision','97b436da4c33663581d394f4ee0a5977fc38c2f4')]:assert re.search(r"'"+name+r"':\s*'"+value+r"'",deps)
print('pinned_libcxx_matches_chromium_deps=PASS')
