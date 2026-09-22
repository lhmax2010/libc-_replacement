"""Verify rebuilt static ELF, actual verbose link, and unchanged main/source files."""
import hashlib,json,re,shlex,subprocess,sys
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_LIBBPF_0922'; T=P/'tmp/BPF_LIBBPF_0922'; arch=sys.argv[1]; O=E/f'verify-{arch}'; O.mkdir(exist_ok=False)
def sha(f):
    with f.open('rb') as s: return hashlib.file_digest(s,'sha256').hexdigest()
records=[]
def run(label,cmd,large=False):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    destination=(T/'outputs'/arch if large else O)/(label+'.txt'); destination.write_text(r.stdout)
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout_path=str(destination),stdout_sha256=sha(destination),stderr=r.stderr))
    (O/'commands.json').write_text(json.dumps(records,indent=2)); assert r.returncode==0
    return r.stdout
assert (E/'cells'/f'after-{arch}'/'exitcode').read_text().strip()=='0'
binary=T/'outputs'/arch/'bpftrace-static'; raw=binary.with_name('bpftrace-static.unstripped'); old=T/'outputs'/arch/'before.unstripped'
def elf(f,label):
    data=run(label+'-dynamic',['readelf','-d',str(f)]); head=run(label+'-header',['readelf','-h',str(f)])
    return dict(path=str(f),bytes=f.stat().st_size,sha256=sha(f),needed=re.findall(r'Shared library: \[(.*?)\]',data),arch_ok=('AArch64' in head if arch=='aarch64' else bool(re.search(r'Machine:\s+ARM\b',head))))
before=elf(old,'before'); after=elf(binary,'after')
s=run('strings',['strings',str(binary)],True).splitlines(); markers=dict(std1=sum('_ZNSt3__1' in x for x in s),cxx11=sum('_ZNSt7__cxx11' in x for x in s),cxx11_broad=sum('St7__cxx11' in x or 'std::__cxx11' in x for x in s))
(O/'string-excerpts.txt').write_text('\n'.join(x for x in s if '_ZNSt3__1' in x)[:5000]+'\n'); del s
libdir='lib' if arch=='armv7l' else 'lib64'
root=P/('tmp/BPF_W1_0921/input-armv7l-origin' if arch=='armv7l' else 'tmp/BPF_W1_0921/input-aarch64-v3')
archive=root/f'usr/{libdir}/libbpf.a'
def symbols(text):
    return {l.split()[-1] for l in text.splitlines() if len(l.split())>=3 and l.split()[-2] in ('T','t','D','d','B','b','W','V')}
a=run('archive-symbols',['nm','--defined-only',str(archive)]); b=run('candidate-symbols',['nm','--defined-only',str(binary)],True)
hits=sorted(symbols(a)&symbols(b)); (O/'libbpf-symbol-intersection.txt').write_text('\n'.join(hits)+'\n')
src=T/f'rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2'; oldsrc=P/f'tmp/BPF_W1_0921/rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2'
baseline=json.loads((E/f'original-tree-{arch}.json').read_text()); protected=[]
for row in baseline:
    if row['kind']!='file': continue
    rel=row['path']
    if rel=='src/ast/CMakeLists.txt':
        digest=sha(oldsrc/rel); protected.append(dict(kind='original',path=rel,sha256=digest,unchanged=digest==row['sha256']))
    # All prior main-build files, and source files outside both build trees.
    if rel.startswith('build/') or (not rel.startswith('build-static/') and rel!='src/ast/CMakeLists.txt'):
        for kind,root_ in [('original',oldsrc),('copy',src)]:
            f=root_/rel; digest=sha(f); protected.append(dict(kind=kind,path=rel,sha256=digest,unchanged=digest==row['sha256']))
(O/'unchanged-files.json').write_text(json.dumps(protected,indent=2))
beforelink=(E/'cells'/f'before-{arch}'/'build.log').read_text(); afterlog=(E/'cells'/f'after-{arch}'/'build.log').read_text()
links=lambda s:[x for x in s.splitlines() if 'clang++ ' in x and ' -o bpftrace ' in x]
bl=links(beforelink); al=links(afterlog); assert len(bl)==len(al)==1,(len(bl),len(al))
(O/'link-before.txt').write_text(bl[0]+'\n'); (O/'link-after.txt').write_text(al[0]+'\n')
tokens_before=shlex.split(bl[0]); tokens_after=shlex.split(al[0])
from collections import Counter
(O/'link-token-diff.json').write_text(json.dumps(dict(removed=list((Counter(tokens_before)-Counter(tokens_after)).elements()),added=list((Counter(tokens_after)-Counter(tokens_before)).elements())),indent=2))
expected={'libm.so.6','libgcc_s.so.1','libc.so.6',('ld-linux.so.3' if arch=='armv7l' else 'ld-linux-aarch64.so.1')}
checks=dict(exact_needed=set(after['needed'])==expected,arch=after['arch_ok'],std1_present=markers['std1']>0,cxx11_absent=markers['cxx11']==markers['cxx11_broad']==0,libbpf_code_defined='bpf_object__open' in hits,old_libbpf_needed='libbpf.so.1' in before['needed'],no_bare_libbpf_after='-lbpf' not in tokens_after,archive_after=f'/usr/{libdir}/libbpf.a' in tokens_after,shared_unwinder='-shared-libgcc' in tokens_after and '-static-libgcc' not in tokens_after,protected_unchanged=all(x['unchanged'] for x in protected),main_not_compiled=not any('Building CXX' in x and '/build/' in x for x in afterlog.splitlines()))
result=dict(status='PASS' if all(checks.values()) else 'FAIL',checks=checks,before=before,after=after,markers=markers,libbpf_archive=dict(path=str(archive),sha256=sha(archive),symbol_intersection=len(hits)),protected_files=len(protected),static_compilation_announcements=sum(bool(re.search(r'Building (?:CXX|C) object',x)) for x in afterlog.splitlines()),raw_sha256=sha(raw),payload='w5.xzdio verified by input cell; ELF has no RPM compression')
(O/'result.json').write_text(json.dumps(result,indent=2)); print(json.dumps(result),flush=True)
raise SystemExit(0 if result['status']=='PASS' else 1)
