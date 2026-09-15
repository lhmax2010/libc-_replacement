#!/usr/bin/env python3
import json,pathlib,re,subprocess,sys
provider=sys.argv[1];target=int(sys.argv[2],16)
cmd=['readelf','--debug-dump=frames',provider];print('COMMAND',json.dumps(cmd),flush=True)
p=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
cies={};block=[];matches=[]
def consume(block):
 if not block:return
 header=block[0];address=header.split()[0]
 if ' CIE' in header:cies[address]=list(block)
 if ' FDE' in header:
  match=re.search(r'cie=([0-9a-f]+) pc=([0-9a-f]+)\.\.([0-9a-f]+)',header)
  if match and int(match[2],16)<=target<int(match[3],16):matches.append((match[1],list(block)))
for line in p.stdout:
 if re.match(r'^[0-9a-f]+ .*\b(CIE|FDE)\b',line):consume(block);block=[]
 block.append(line)
consume(block);stderr=p.stderr.read();rc=p.wait();print('READELF_EXIT',rc);print(stderr,end='')
assert rc==0 and matches
for cie,entry in matches:
 print('MATCHED_FDE',end='\n');print(''.join(entry),end='')
 found=next(v for k,v in cies.items()if int(k,16)==int(cie,16))
 print('REFERENCED_CIE');print(''.join(found),end='')
