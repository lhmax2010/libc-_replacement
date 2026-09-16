import re,subprocess
from common import *
targets=[0x603a1e6,0xa0f9970,0xa0f9a40,0x601fb5b]
p=subprocess.Popen(['readelf','--debug-dump=frames',str(PROVIDER)],stdout=subprocess.PIPE,text=True)
cies={};block=[];matches=[]
def consume(lines):
 if not lines:return
 head=lines[0]
 if ' CIE' in head:cies[int(head.split()[0],16)]=list(lines)
 m=re.search(r'FDE cie=([0-9a-f]+) pc=([0-9a-f]+)\.\.([0-9a-f]+)',head)
 if m:
  for target in targets:
   if int(m[2],16)<=target<int(m[3],16):matches.append((target,int(m[1],16),list(lines)))
for line in p.stdout:
 if re.match(r'^[0-9a-f]+ .*\b(CIE|FDE)\b',line):consume(block);block=[]
 block.append(line)
consume(block);assert p.wait()==0
for target,cie,lines in matches:
 print('TARGET',hex(target));print(''.join(lines));print(''.join(cies[cie]))
assert len(matches)==len(targets),(matches,targets)
