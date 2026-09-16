from common import *
import re,struct
p=OUT/'W2/raw/symbol_original_4.stdout'
s=p.read_text()
alloc={}
for m in re.finditer(r'^ALLOC attempt=(\d+) size=(\d+) ptr=(\S+) live=(\d+) failed=(\d+)',s,re.M):
 alloc[int(m[1])]={'size':int(m[2]),'pointer':int(m[3],16) if m[3]!='(nil)' else 0,'live':int(m[4]),'failed':int(m[5])}
data={int(m[1]):bytes.fromhex(m[2]) for m in re.finditer(r'^LIVE_BYTES attempt=(\d+) hex=(\w+)',s,re.M)}
assert data[1]==b'N'*64 and data[2]==b'V'*64
slots=struct.unpack('<8Q',data[3]);assert slots[0]==alloc[1]['pointer'] and slots[1]==256 and slots[4]==alloc[2]['pointer'] and slots[5]==512
assert [alloc[i]['size'] for i in [1,2,3,4]]==[257,513,64,128]
assert all(alloc[i]['live'] for i in [1,2,3]) and alloc[4]['failed']
save('W2','ALLOCATION_OWNERSHIP.json',{'observed_allocations':alloc,'vector_slot_words':list(slots),'validated':'64-byte pair storage points to exact name and value allocations; size/length/content matched','name_source':'net/cookies/parsed_cookie.cc:576','value_source':'net/cookies/parsed_cookie.cc:585','vector_source':'net/cookies/parsed_cookie.cc:634','constructor_source':'net/cookies/parsed_cookie.cc:147','note':'Source-to-stripped-binary function naming is structural inference, not DWARF line mapping. Pointer ownership relationships are directly measured.'})
print('OWNERSHIP_ASSERTIONS_PASS')
