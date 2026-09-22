"""Verify original RPMs and ELF/.text in memory. NEVER writes input or extracted copies."""
import hashlib,json,shlex,struct,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W3R_0922';rows=[];commands=[]
def text_section(data):
 assert data[:4]==b'\x7fELF';endian='<' if data[5]==1 else '>'
 if data[4]==1:
  off=struct.unpack_from(endian+'I',data,32)[0];size,num,names=struct.unpack_from(endian+'HHH',data,46);fmt=endian+'IIIIIIIIII'
 else:
  off=struct.unpack_from(endian+'Q',data,40)[0];size,num,names=struct.unpack_from(endian+'HHH',data,58);fmt=endian+'IIQQQQIIQQ'
 headers=[struct.unpack_from(fmt,data,off+i*size) for i in range(num)];s=headers[names];strings=data[s[4]:s[4]+s[5]]
 for h in headers:
  if strings[h[0]:].split(b'\0',1)[0]==b'.text':return data[h[4]:h[4]+h[5]]
 raise ValueError('no .text')
for row in json.loads((E/'build-audit/ELF_COMPARISON.json').read_text()):
 n=row['round'];a=row['arch'];verify=f'verify-bpf-{a}-libcxx' if n=='BPF_W1_0921' else f'verify-{a}'
 result=json.loads((P/'progress'/n/verify/'result.json').read_text());rpm=next(x for x in result['rpms'] if x['identity'].split('\t')[0]=='bpftrace');p=Path(rpm['path'])
 digest=hashlib.sha256(p.read_bytes()).hexdigest();assert digest==rpm['sha256']
 argv=['rpm2cpio',str(p)];r=subprocess.run(argv,capture_output=True);commands.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stderr=r.stderr.decode(errors='replace')));assert r.returncode==0
 archive=r.stdout;pos=0;elf=None
 while pos<len(archive):
  header=archive[pos:pos+110];assert header[:6] in (b'070701',b'070702')
  fields=[int(header[6+i*8:14+i*8],16) for i in range(13)];length,namesize=fields[6],fields[11];name=archive[pos+110:pos+110+namesize-1].decode();start=(pos+110+namesize+3)//4*4
  if name.lstrip('./')=='usr/bin/bpftrace':elf=archive[start:start+length]
  pos=(start+length+3)//4*4
  if name=='TRAILER!!!':break
 assert elf is not None;eh=hashlib.sha256(elf).hexdigest();th=hashlib.sha256(text_section(elf)).hexdigest()
 rows.append(dict(round=n,arch=a,rpm=str(p),rpm_sha256=digest,rpm_unchanged=True,elf_sha256=eh,text_sha256=th,pre_objcopy_elf_match=eh==row['sha256'],text_dump_match=th==row['text_sha256']))
 assert rows[-1]['pre_objcopy_elf_match'] and rows[-1]['text_dump_match']
(E/'RPM_READONLY_CHECK.json').write_text(json.dumps(dict(status='PASS',rows=rows,commands=commands,input_mutation=False),indent=2));print(json.dumps(rows,indent=2))
