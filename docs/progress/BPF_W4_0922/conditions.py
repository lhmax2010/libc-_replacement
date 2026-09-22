"""Read historical command/log evidence; do not execute builds or change macros."""
import hashlib,json,re,shutil
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922/condition-check';E.mkdir(exist_ok=False)
W=P/'docs/progress/BPF_W1_0921';rows=[];excerpts=[];ids=[]
def read(p,pattern):
 data=p.read_bytes();ids.append(dict(path=str(p),sha256=hashlib.sha256(data).hexdigest()))
 matches=[dict(line=i,text=line) for i,line in enumerate(data.decode(errors='replace').splitlines(),1) if re.search(pattern,line)]
 excerpts.append(dict(file=str(p),matches=matches));return matches
cfg=P/'codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config';gbs=P/'config/gbs_llvm.conf';spec=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push/packaging/bpftrace.spec'
read(cfg,r'_toolchain|toolchain_is|build_with_libcxx');read(gbs,r'.');read(spec,r'^%if|^%define build_with|^install .*bpftrace-static')
assert 'build_with_libcxx' not in cfg.read_text() and not any(x in gbs.read_text() for x in ('build_with_libcxx','_toolchain'))
shutil.copy2(cfg,E/'Base-Toolchain.project_config.snapshot');shutil.copy2(gbs,E/'gbs_llvm.conf.snapshot')
for arch in ('armv7l','aarch64'):
 for variant in ('libcxx','gcc','undefined'):
  cell=f'bpf-{arch}-{variant}';f=W/'cell_commands'/f'{cell}.json';cmd=json.loads(f.read_text());defs=[(v,cmd[i+1]) for i,v in enumerate(cmd[:-1]) if v in ('--define','--undefine')]
  read(f,r'toolchain|--define|--undefine|spec')
  root=Path(cmd[cmd.index('--ro-bind')+1]);mac=root/'home/abuild/.rpmmacros';matches=read(mac,r'_toolchain|build_with_libcxx|toolchain_is')
  assert not any('build_with_libcxx' in x['text'] for x in matches)
  log=W/'cells'/cell/'build.log';lm=read(log,r'HARNESS_|The CXX compiler identification|^\+ (export CC=|bpftrace_original_ldflags=|test 1 = 1|install -m 0755)')
  text=log.read_text();assert not any('build_with_libcxx' in val for _,val in defs)
  if variant=='libcxx':
   assert not any('_toolchain' in val for _,val in defs)
   assert 'The CXX compiler identification is Clang 22.1.8' in text and '+ test 1 = 1' in text
   toolchain='clang';bwith='1 (spec defines at lines 1-5)';option='no toolchain/build_with_libcxx CLI definition'
  elif variant=='gcc':
   assert ('--define','_toolchain_override gcc') in defs
   assert 'The CXX compiler identification is GNU 14.2.0' in text and 'build-static/src/bpftrace' not in text
   toolchain='gcc';bwith='UNDEFINED (condition evaluates 0)';option='--define _toolchain_override gcc'
  else:
   assert ('--undefine','_toolchain') in defs and 'HARNESS_AFTER_INCLUDE=0||' in text
   toolchain='UNDEFINED';bwith='UNDEFINED (condition evaluates 0)';option='--undefine _toolchain; undefined-harness.spec also undefines at parse time'
  rows.append(dict(cell=cell,toolchain=toolchain,build_with_libcxx_during_spec=bwith,cli=option,definitions=defs,evidence_command=str(f.relative_to(P)),evidence_log=str(log.relative_to(P)),assessment='historical argv/log plus conditional spec reconstruction; no historical direct numeric macro dump claimed'))
(E/'SIX_CELLS.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2))
(E/'INPUT_IDENTITIES.json').write_text(json.dumps(ids,indent=2))
(E/'EXCERPTS.json').write_text(json.dumps(excerpts,ensure_ascii=False,indent=2))
(E/'EXCERPTS.txt').write_text('\n\n'.join('FILE '+x['file']+'\n'+'\n'.join(f"{m['line']}: {m['text']}" for m in x['matches']) for x in excerpts))
print('SIX_CELLS_RECONCILED; mismatched effective combinations NOT_OBSERVED; no builds')
