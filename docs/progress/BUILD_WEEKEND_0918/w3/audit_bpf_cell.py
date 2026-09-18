"""Audit expanded candidate spec and actually emitted compiler commands per cell."""
import hashlib,json,re,shlex,subprocess,sys
from pathlib import Path
p=Path.cwd(); out=p/'progress/BUILD_WEEKEND_0918'; arch,mode,cell=sys.argv[1:]
assert arch in ('armv7l','aarch64') and mode in ('gcc','undefined','libcxx')
cmd=json.loads((out/'cell_commands'/f'{cell}.json').read_text())
split=cmd.index('--'); base=cmd[:split]; args=cmd[split+2:]
assert cmd[split+1]=='rpmbuild'
args=[x for x in args if x not in ('--noclean','-bb')]
spec=args.pop()
assert spec=='/tmp/task/specs/bpftrace.spec' or (mode=='undefined' and spec=='/tmp/task/specs/bpftrace-undefined-harness.spec')
query=base+['--','rpmspec',*args,'-P',spec]
r=subprocess.run(query,capture_output=True,text=True)
dest=out/f'audit-bpf-{arch}-{mode}'; dest.mkdir(exist_ok=False)
(dest/'expanded.spec').write_text(r.stdout)
(dest/'query.json').write_text(json.dumps({'command':shlex.join(query),'exitcode':r.returncode,'stderr':r.stderr},indent=2))
assert r.returncode==0
requires=[x for x in r.stdout.splitlines() if re.match(r'^BuildRequires\s*:',x,re.I)]
libcxx_requires=[x for x in requires if re.search(r'libc\+\+(?:abi)?(?:-|\s|$)',x)]
lines=(out/'cells'/cell/'build.log').read_text(errors='replace').splitlines()
compiler=[]
for line in lines:
    # Verbose Make compile commands and linker command lines, not comments,
    # diagnostics or the spec text. Preserve the precise recognized scope.
    if re.search(r'(?:^|&&\s+|^\+\s+)(?:\S*/)?(?:[\w.-]+-)?(?:clang\+\+|clang|g\+\+|gcc)(?:\s|$)',line):
        compiler.append(line)
(dest/'observed_compiler_commands.txt').write_text('\n'.join(compiler)+'\n')
forbidden=[line for line in compiler if '-stdlib=libc++' in line or '-lc++abi' in line]
result={'arch':arch,'mode':mode,'build_cell':cell,'expanded_buildrequires':requires,
        'libcxx_buildrequires':libcxx_requires,'observed_compiler_command_count':len(compiler),
        'commands_with_libcxx_options':len(forbidden),
        'candidate_spec_sha256':hashlib.sha256((p/'tmp/WEEKEND_0918/specs/bpftrace.spec').read_bytes()).hexdigest(),
        'actual_entry_spec':spec,
        'entry_spec_sha256':hashlib.sha256((p/'tmp/WEEKEND_0918/specs'/Path(spec).name).read_bytes()).hexdigest(),
        'scope':'Expanded spec with this cell macro arguments plus emitted verbose compiler/linker commands; not arbitrary unseen external configuration.'}
assert compiler,'No actual compiler command recognized; cannot count this as zero forbidden options'
if mode in ('gcc','undefined'):
    assert not libcxx_requires and not forbidden
    assert '-stdlib=libc++' not in r.stdout and '-lc++abi' not in r.stdout
else:
    assert libcxx_requires and forbidden
result['status']='PASS'
(dest/'result.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result,indent=2))
