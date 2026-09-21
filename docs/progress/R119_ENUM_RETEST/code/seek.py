from common import *
import re
gate('seek_gate')
prior=ROOT/'docs/progress/R119_ARM_RETEST'
configs=json.loads((prior/'CONFIGURATION.json').read_text())
pkg=next(r for r in json.loads((OUT/'PODOFO_MATERIALS.json').read_text()) if r['name']=='podofo-devel')
assert pkg['exists']
incroot=pathlib.Path(pkg['extracted'])/'usr/include'
header=incroot/'podofo/base/PdfInputDevice.h';assert header.is_file()
save('SEEK_HEADER.json',dict(package=pkg,path=str(header),sha256=sha(header),text='\n'.join(f'{i+1}: {line}' for i,line in enumerate(header.read_text().splitlines()))))
TMP=ROOT/'tmp/R119_ENUM_RETEST';TMP.mkdir(parents=True,exist_ok=True)
src=OUT/'probes/seek.cpp'
src.write_text('''#include <podofo/base/PdfInputDevice.h>
// Qualified call deliberately suppresses virtual dispatch to expose the real declaration's symbol.
void probe_direct(PoDoFo::PdfInputDevice& d, std::streamoff off, std::ios_base::seekdir dir) {
  d.PoDoFo::PdfInputDevice::Seek(off,dir);
}
// Ordinary virtual call: do not infer the callee's name from this function's own symbol.
void probe_virtual(PoDoFo::PdfInputDevice& d, std::streamoff off, std::ios_base::seekdir dir) {
  d.Seek(off,dir);
}
''')
results=[]
for c in configs:
 lib=c['library'];opts=c['flags']+['-I'+str(incroot)];cc=c['compiler'];obj=TMP/f'seek_{lib}.o';ir=OUT/f'probes/seek_{lib}.ll'
 b=run('seek_build_'+lib,[cc,*opts,'-H','-c',str(src),'-o',str(obj)],check=False)
 if b['exit']:
  save('SEEK_BLOCK.json',dict(library=lib,record=b['record'],reason='真实ARM头编译失败，未替换声明'));raise SystemExit(b['exit'])
 headers={}
 for line in b['stderr'].splitlines():
  m=re.match(r'^\.+ (/.+)$',line)
  if m and pathlib.Path(m[1]).is_file():headers[m[1]]=sha(m[1])
 sy=run('seek_symbols_'+lib,['readelf','-h','-Ws','--wide',str(obj)])
 names=[line.split()[-1] for line in sy['stdout'].splitlines() if ' UND ' in line and 'PdfInputDevice4Seek' in line]
 assert len(names)==1,names
 run('seek_ir_'+lib,[cc,*opts,'-S','-emit-llvm',str(src),'-o',str(ir)])
 text=ir.read_text();blocks=re.findall(r'define[^\n]*@([^\s(]+).*?\n}',text,re.S)
 assert 'probe_virtual' in text and 'probe_direct' in text
 results.append(dict(arch='armv7l',library=lib,kind='COMPILE_ONLY_REAL_ARM_DEVEL_HEADER',symbol=names[0],build=b['record'],symbols=sy['record'],object_sha256=sha(obj),source=str(src.relative_to(ROOT)),source_sha256=sha(src),header_sha256=headers,ir=str(ir.relative_to(ROOT)),ir_sha256=sha(ir)))
save('SEEK_SYMBOLS.json',results)
print(json.dumps(results,ensure_ascii=False,indent=2)[:5000])
