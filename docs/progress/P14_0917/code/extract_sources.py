"""从真实 SRPM 内 tar 流选择小目标源码/头；不展开 Chromium 全树。"""
import hashlib,json,pathlib,sys,tarfile
from common import TMP,OUT
root=TMP/'chromium-efl'
rows=[];members=0
prefixes=('base/','net/','build/','third_party/abseil-cpp/absl/','partition_alloc/','third_party/boringssl/src/include/','url/','buildtools/')
with tarfile.open(fileobj=sys.stdin.buffer,mode='r|gz') as archive:
 for item in archive:
  members+=1
  parts=pathlib.PurePosixPath(item.name).parts
  if len(parts)<2 or '..' in parts or parts[0]!='chromium-efl-1.1.144':continue
  rel='/'.join(parts[1:])
  selected=rel.startswith(prefixes) and rel.endswith(('.h','.inc','.cc','.gn','.gni'))
  selected|=rel in ('BUILD.gn','.gn','DEPS','README.md','packaging/chromium-efl.spec','tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc','tizen_src/ewk/efl_integration/public/ewk_cookie_parser.h','tizen_src/ewk/efl_integration/private/ewk_private.h','tizen_src/ewk/efl_integration/BUILD.gn')
  if not selected or not item.isfile():continue
  assert item.size<20*1024*1024,(rel,item.size)
  data=archive.extractfile(item).read();target=root/rel;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
  rows.append({'path':rel,'size':len(data),'sha256':hashlib.sha256(data).hexdigest()})
(OUT/'W2/SOURCE_INDEX.json').write_text(json.dumps({'archive_members':members,'selected_files':rows},indent=2)+'\n')
print('SELECTED',len(rows),'BYTES',sum(x['size'] for x in rows),flush=True)
