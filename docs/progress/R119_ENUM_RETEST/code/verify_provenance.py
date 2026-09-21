from common import *
import re,csv
gate('provenance_gate')
pkg=next(x for x in json.loads((OUT/'PODOFO_MATERIALS.json').read_text()) if x['name']=='podofo-devel')
base=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
rpm=base/'rpms'/pkg['repo']/pkg['location'];assert rpm.is_file() and sha(rpm)==pkg['rpm_sha256']
r=run('podofo_rpm_dump',['rpm','-qp','--dump',str(rpm)])
dump={line.split()[0]:line.split()[3] for line in r['stdout'].splitlines() if len(line.split())>4 and line.startswith('/')}
files={}
for s in json.loads((OUT/'SEEK_SYMBOLS.json').read_text()):
 for p,h in s['header_sha256'].items():
  if p.startswith(pkg['extracted']+'/'):
   assert sha(p)==h and dump[p[len(pkg['extracted']):]]==h,p
   files[p]=h
assert files
save('RPM_BINDING.json',dict(package=pkg,rpm=str(rpm),rpm_sha256=sha(rpm),dump_record=r['record'],verified_files=files))
irs=[]
for lib in ['gnu','cxx']:
 p=OUT/f'probes/seek_{lib}.ll';text=p.read_text()
 block=next(b for b in re.findall(r'define[^\n]+\{.*?\n}',text,re.S) if 'probe_virtual' in b.splitlines()[0])
 slots=re.findall(r'getelementptr inbounds ptr, ptr %\d+, i64 (\d+)',block)
 assert slots==['6'],slots
 assert re.search(r'call void %\d+\(',block)
 assert 'PdfInputDevice4Seek' not in block
 irs.append(dict(library=lib,record=str(p.relative_to(ROOT)),sha256=sha(p),virtual_call_block=block,caller_selected_slot_index=6,interpretation='仅为本次调用方编译选择的虚表槽位，不是两套provider虚表兼容性实测'))
save('VIRTUAL_CALL.json',irs)
source=ROOT/'docs/progress/R119_RECLASSIFY/CROSS_CANDIDATE_EDGE_MAP.tsv'
existing=[r for r in csv.DictReader(source.open(),delimiter='\t') if r['candidate'] in ['std::ios_base::fmtflags','std::ios_base::iostate','std::ios_base::openmode','std::ios_base::seekdir']]
save('EXISTING_MEMBER_EDGES.json',dict(source=str(source.relative_to(ROOT)),sha256=sha(source),rows=existing))
print('RPM file hashes verified',len(files),'virtual slots',[x['caller_selected_slot_index'] for x in irs],'existing member associations',len(existing))
