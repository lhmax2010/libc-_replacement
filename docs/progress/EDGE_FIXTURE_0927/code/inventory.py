from common import *
import csv
gate();root=TMP/'root';rows=list(csv.DictReader((OUT/'EDGES.tsv').open(),delimiter='\t'));rpms=json.loads((OUT/'RPMS.json').read_text())
inventory=[];cache={}
for r in rows:
 edge=int(r['edge']);stem=pathlib.Path(r['x86_provider_elf']).name.split('.so')[0]+'.so'
 files=sorted(set(p.resolve() for p in (root/'usr/lib64').glob(stem+'*') if p.is_file()))
 assert len(files)<=1,(edge,files)
 rec={'edge':edge,'consumer':r['consumer'],'source_package':r['provider'],'interface':r['interface'],'expected_symbol':r['x86_raw_symbol'],'old_provider_elf':r['x86_provider_elf']}
 if not files:
  rec.update(status='PROVIDER_ELF_NOT_FOUND');inventory.append(rec);continue
 p=files[0];rel='/'+str(p.relative_to(root));owner=[]
 for pkg in rpms:
  listed=ROOT/(pkg['file_list_record']+'.stdout')
  if rel in listed.read_text().splitlines():owner.append(pkg['name'])
 if str(p) not in cache:
  cache[str(p)]=run('provider_symbols_'+str(edge),['nm','-D','--defined-only',p])
  run('provider_dynamic_'+str(edge),['readelf','-d',p])
 nm=cache[str(p)];symbol=r['x86_raw_symbol'].split('@')[0]
 rec.update(provider=str(p.relative_to(ROOT)),provider_sha256=sha(p),runtime_packages=owner,symbol_record=nm['record'],symbol_present=any(l.split()[-1].split('@')[0]==symbol for l in nm['stdout'].splitlines() if l.split()))
 pattern=r['interface'].split('::')[-1].split('(')[0]
 if edge==23:pattern='Reader('
 h=run('header_search_'+str(edge),['rg','-n','-F',pattern,root/'usr/include'],check=False)
 rec.update(header_search_record=h['record'],header_hits=h['stdout'].splitlines(),status='INVENTORIED')
 inventory.append(rec)
save('INVENTORY.json',inventory)
for r in inventory:print(r['edge'],r.get('runtime_packages'),r.get('symbol_present'),len(r.get('header_hits',[])))
