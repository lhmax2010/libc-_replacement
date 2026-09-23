"""Read three previously downloaded Base-Toolchain RPM headers, not local rebuilds."""
import csv,hashlib,json,shlex,subprocess
from pathlib import Path
P=Path('progress/QB_PRECHECK_0922')
source=Path('progress/R100/tables/armv7l_internal_download_status.tsv')
rows=list(csv.DictReader(source.open(),delimiter='\t'))
results=[]
for name in ('libllvm','libicu','boost-devel'):
 row=next(x for x in rows if x['name']==name and x['arch']=='armv7l' and x['repo_id']=='base')
 f=Path(row['target']); h=hashlib.sha256()
 with f.open('rb') as stream:
  for block in iter(lambda:stream.read(1024*1024),b''):h.update(block)
 sha=h.hexdigest()
 assert sha==row['checksum']==row['actual_sha256'] and row['result']=='PASS'
 command=['rpm','-qp','--qf','%{NAME} %{PAYLOADCOMPRESSOR} %{PAYLOADFLAGS}\n',str(f)]
 r=subprocess.run(command,capture_output=True,text=True)
 result=dict(source_table=str(source),source_table_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),url=row['url'],path=str(f),bytes=f.stat().st_size,sha256=sha,command=shlex.join(command),argv=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
 results.append(result);print(json.dumps(result,ensure_ascii=False))
 assert r.returncode==0
(P/'RPM_PAYLOAD_HEADERS.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
