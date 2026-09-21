from common import *
import csv,gzip,sqlite3,xml.etree.ElementTree as ET
gate('inputs_gate')
base=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
found=[]
for repo in ['base_binary','unified_binary']:
 for event,p in ET.iterparse(gzip.open(base/'repodata'/f'{repo}.primary.xml.gz','rb'),events=['end']):
  if p.tag!='{http://linux.duke.edu/metadata/common}package':continue
  if p.findtext('{*}name') in ['podofo','podofo-devel'] and p.findtext('{*}arch')=='armv7l':
   digest=p.findtext('{*}checksum');root=base/'extracted'/digest[:2]/digest
   row=dict(repo=repo,name=p.findtext('{*}name'),arch=p.findtext('{*}arch'),version=p.find('{*}version').attrib,rpm_sha256=digest,location=p.find('{*}location').attrib['href'],extracted=str(root),exists=root.is_dir())
   found.append(row);print(json.dumps(row))
  p.clear()
save('PODOFO_MATERIALS.json',found)
div=ROOT/'docs/progress/R119_DIVERGENT'
for name in ['DIVERGENT_EXPOSURE.tsv','PUBLIC_DECLARATIONS.tsv']:
 rows=[r for r in csv.DictReader((div/name).open(),delimiter='\t') if r['id'] in ['T0008','T0009','T0010','T0011']]
 save('prior_'+name.replace('.tsv','.json'),rows);print(name,json.dumps(rows,ensure_ascii=False))
db=ROOT/'tmp/R119_batch1/full_elf/symbols.sqlite3'
con=sqlite3.connect('file:'+str(db)+'?mode=ro',uri=True)
print('SCHEMA',con.execute('select name,sql from sqlite_master where type in ("table","index")').fetchall())
print('ELF COUNT',con.execute('select count(*) from elf').fetchone())
