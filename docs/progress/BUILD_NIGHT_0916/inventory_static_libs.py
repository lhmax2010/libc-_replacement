"""Inventory named static dependencies in existing target roots, without installing anything."""
import csv
import json
from pathlib import Path
import shlex
import subprocess

p=Path('progress/BUILD_NIGHT_0916')
roots={
 'armv7l': Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-libcxx/local/BUILD-ROOTS/scratch.armv7l.0').resolve(),
 'aarch64': Path('tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0').resolve(),
 'x86_64': Path('tmp/GBS-ROOT/BUILD-W2-bcc-x86-libcxx/local/BUILD-ROOTS/scratch.x86_64.0').resolve(),
}
libs={
 'LLVM components':('required','libLLVM*.a'),
 'Clang C API':('required','libclang.a'),
 'Clang components':('required','libclang*.a'),
 'BCC':('required','libbcc.a'),
 'BCC runtime':('required','libbcc_bpf.a'),
 'BCC loader':('required','libbcc-loader-static.a'),
 'libbpf':('required','libbpf.a'),
 'ELF':('required','libelf.a'),
 'zlib':('required','libz.a'),
 'LZMA':('required','liblzma.a'),
 'libc++':('required','libc++.a'),
 'libc++abi':('required','libc++abi.a'),
 'libgcc unwinder':('unwinder runtime constraint','libgcc_eh.a'),
 'libxml2':('LLVM transitive if enabled','libxml2.a'),
 'libzstd':('LLVM/BFD transitive if enabled','libzstd.a'),
 'libedit':('LLVM transitive if enabled','libedit.a'),
 'libffi':('LLVM transitive if enabled','libffi.a'),
 'terminfo':('LLVM/libedit transitive if enabled','libtinfo.a'),
 'BFD':('optional feature equivalence','libbfd.a'),
 'opcodes':('optional feature equivalence','libopcodes.a'),
 'iberty':('BFD transitive','libiberty.a'),
 'sframe':('BFD transitive if enabled','libsframe.a'),
 'DWARF':('optional feature equivalence','libdw.a'),
 'EBL':('DWARF transitive if required','libebl.a'),
 'bzip2':('DWARF transitive','libbz2.a'),
 'pcap':('optional skb_output','libpcap.a'),
 'blazesym':('optional feature equivalence','libblazesym*.a'),
 'systemd':('optional ENABLE_SYSTEMD','libsystemd.a'),
 'libstdc++fs':('existing CMake adds for Clang; compatibility needs review','libstdc++fs.a'),
}
logs=(p/'inventory_commands.jsonl').open('w')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True,errors='replace')
 logs.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n');logs.flush()
 return r
records=[];installed={};inventory={}
for arch,root in roots.items():
 assert root.is_dir(),root
 all_archives=[]
 for directory in ['usr/lib','usr/lib64','usr/libexec']:
  base=root/directory
  if base.is_dir() and not base.is_symlink():
   all_archives.extend(x for x in base.rglob('*.a') if x.is_file())
 inventory[arch]=[dict(path=str(x.relative_to(root)),bytes=x.stat().st_size) for x in sorted(all_archives)]
 rpm=['rpm','--root',str(root),'--dbpath','/var/lib/rpm']
 r=run(rpm+['-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{VCS}\n'])
 assert r.returncode==0
 (p/f'{arch}_installed_packages.tsv').write_text(r.stdout)
 installed[arch]=[line for line in r.stdout.splitlines() if line.split('\t')[0] in ['llvm-static-devel','clang-devel','bcc-tools-devel','libc++-devel','libc++abi-devel','zlib-devel','elfutils-devel','libbpf-devel','xz-devel']]
 for library,(role,pattern) in libs.items():
  matches=[x for x in all_archives if x.match(pattern)]
  owners=[]
  for f in matches if len(matches)<5 else matches[:1]:
   r=run(rpm+['-qf','/'+str(f.relative_to(root)),'--qf','%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}'])
   owners.append(dict(path=str(f.relative_to(root)),rpm=r.stdout.strip(),exitcode=r.returncode))
  records.append(dict(arch=arch,library=library,role=role,pattern=pattern,count=len(matches),
                      status='PRESENT_IN_ROOT' if matches else 'NOT_AVAILABLE_IN_ROOT',
                      paths=[str(x.relative_to(root)) for x in matches],owners=owners))
 # Full installed file lists distinguish absent payload from simply not installed.
 for pkg in ['llvm-static-devel','clang-devel','bcc-tools-devel','libc++-devel','libc++abi-devel']:
  r=run(rpm+['-ql',pkg])
  (p/f'{arch}_{pkg}_files.txt').write_text(r.stdout)
 # Read only a bounded set of LLVM CMake dependency declarations.
 for name in ['LLVMConfig.cmake','LLVMExports.cmake','ClangTargets.cmake']:
  matches=list((root/'usr').glob('lib*/cmake/*/'+name))
  for f in matches:
   (p/f'{arch}_{name}').write_bytes(f.read_bytes())

(p/'static_archive_inventory.json').write_text(json.dumps(dict(roots={a:str(r) for a,r in roots.items()},
 records=records,installed_providers=installed,all_archives=inventory),ensure_ascii=False,indent=2)+'\n')
with (p/'static_dependencies.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['arch','library','role','pattern','count','status','paths'])
 for r in records:w.writerow([r[k] if k!='paths' else ';'.join(r[k]) for k in ['arch','library','role','pattern','count','status','paths']])
for arch in roots:
 print(arch, 'archives',len(inventory[arch]))
 print(json.dumps(installed[arch],ensure_ascii=False))
 for r in records:
  if r['arch']==arch:print(r['library'],r['count'],r['status'])
