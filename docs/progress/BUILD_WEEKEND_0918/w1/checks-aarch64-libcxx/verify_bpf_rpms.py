"""Inspect actual final RPM payloads; never treat build success as acceptance."""
import hashlib
import json
import shlex
import subprocess
import sys
import struct
from pathlib import Path

p=Path.cwd(); arch,mode=sys.argv[1:]
assert arch in ('armv7l','aarch64') and mode in ('libcxx','gcc','undefined')
label=f'bpf-{arch}-{mode}'
out=p/'progress/BUILD_WEEKEND_0918'/f'verify-{label}'
out.mkdir(exist_ok=False)
top=p/'tmp/WEEKEND_0918'/f'rpm-{label}'
payload=p/'tmp/WEEKEND_0918'/f'payload-{label}'
assert not payload.exists(); payload.mkdir()
commands=[]; rpms=[]
def run(cmd,cwd=None):
    r=subprocess.run(cmd,cwd=cwd,capture_output=True,text=True,errors='replace')
    commands.append({'command':shlex.join(cmd),'cwd':str(cwd) if cwd else str(p),'exitcode':r.returncode,
                     'stdout':r.stdout,'stderr':r.stderr})
    (out/'commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0,(cmd,r.stderr)
    return r.stdout
for f in sorted((top/'RPMS').rglob('*.rpm')):
    identity=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}',str(f)])
    name=identity.split('\t')[0]
    run(['rpm','--checksig','--nosignature',str(f)])
    compression=run(['rpm','-qp','--qf','%{PAYLOADCOMPRESSOR}|%{PAYLOADFLAGS}',str(f)])
    assert compression=='xz|5T1'
    with f.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    names=run(['rpm','-qpl',str(f)])
    assert all('..' not in Path(x).parts for x in names.splitlines())
    (out/f'{name}.files.txt').write_text(names)
    (out/f'{name}.scripts.txt').write_text(run(['rpm','-qp','--scripts',str(f)]))
    rpms.append({'path':str(f),'identity':identity,'sha256':sha,'bytes':f.stat().st_size,'compression':compression})
    if name in ('bpftrace','bpftrace-static'):
        d=payload/name; d.mkdir()
        run(['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -id --quiet --no-absolute-filenames','extract',str(f)],cwd=d)
assert {'bpftrace','bpftrace-static','bpftrace-common'} <= {x['identity'].split('\t')[0] for x in rpms}
binary=payload/'bpftrace-static/usr/bin/bpftrace-static'
main=payload/'bpftrace/usr/bin/bpftrace'
with binary.open('rb') as stream: static_sha=hashlib.file_digest(stream,'sha256').hexdigest()
details={'rpms':rpms,'static_binary_sha256':static_sha,'arch':arch,'mode':mode,'static_binary_bytes':binary.stat().st_size}
for key,f in [('main',main),('static',binary)]:
    run(['file',str(f)])
    dynamic=run(['readelf','-dW',str(f)])
    (out/f'{key}.dynamic.txt').write_text(dynamic)
    details[key+'_needed']=[x.split('[')[1].split(']')[0] for x in dynamic.splitlines() if '(NEEDED)' in x]
    headers=run(['readelf','-hW',str(f)])
    assert ('AArch64' if arch=='aarch64' else 'ARM') in headers
if mode=='libcxx':
    strings_file=payload/'static.strings.txt'
    cmd=['strings','-a',str(binary)]
    with strings_file.open('wb') as stream:
        r=subprocess.run(cmd,stdout=stream,stderr=subprocess.PIPE)
    with strings_file.open('rb') as stream: sha=hashlib.file_digest(stream,'sha256').hexdigest()
    commands.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stderr':r.stderr.decode(errors='replace'),
                     'stdout_path':str(strings_file),'stdout_sha256':sha})
    (out/'commands.json').write_text(json.dumps(commands,indent=2))
    assert r.returncode==0
    std1=cxx11=0; excerpt=[]
    with strings_file.open(errors='replace') as stream:
        for line in stream:
            std1 += '_ZNSt3__1' in line
            cxx11 += '_ZNSt7__cxx11' in line
            if '_ZNSt3__1' in line and len(excerpt)<20: excerpt.append(line)
    details['std1_string_lines']=std1
    details['cxx11_string_lines']=cxx11
    (out/'namespace_excerpt.txt').write_text(''.join(excerpt))
    assert 'libgcc_s.so.1' in details['static_needed']
    assert not any(x.startswith(('libc++.so','libc++abi.so','libLLVM','libstdc++')) for x in details['static_needed'])
    allowed={'libgcc_s.so.1','libc.so.6','libm.so.6','libdl.so.2','librt.so.1','libpthread.so.0',
             'ld-linux-aarch64.so.1','ld-linux.so.3','ld-linux-armhf.so.3'}
    details['unexpected_static_dynamic_dependencies']=sorted(set(details['static_needed'])-allowed)
    assert not details['unexpected_static_dynamic_dependencies'],details['unexpected_static_dynamic_dependencies']
    assert 'libc++.so.1' in details['main_needed']
    assert 'libstdc++.so.6' not in details['main_needed']
    assert details['cxx11_string_lines']==0
    # RPM stripping can remove private symbol names. Verify ABI evidence on
    # this build's unstripped binary and bind it to the RPM's allocated code
    # and constant-data sections; do not infer ABI absence from stripped text.
    raw=top/'BUILD/bpftrace-0.24.2/build-static/src/bpftrace'
    raw_strings=run(['strings','-a',str(raw)])
    details['unstripped_std1_string_lines']=sum('_ZNSt3__1' in x for x in raw_strings.splitlines())
    details['unstripped_cxx11_string_lines']=sum('_ZNSt7__cxx11' in x for x in raw_strings.splitlines())
    assert details['unstripped_std1_string_lines']>0 and details['unstripped_cxx11_string_lines']==0
    def section_hashes(path):
        with path.open('rb') as f:
            header=f.read(64); assert header[:4]==b'\x7fELF'
            endian='<' if header[5]==1 else '>'
            wide=header[4]==2
            shoff=struct.unpack_from(endian+('Q' if wide else 'I'),header,40 if wide else 32)[0]
            entsize,count,names=struct.unpack_from(endian+'HHH',header,58 if wide else 46)
            assert count>0 and names<count
            f.seek(shoff); table=f.read(entsize*count)
            fmt=endian+('IIQQQQIIQQ' if wide else 'IIIIIIIIII')
            sections=[struct.unpack_from(fmt,table,i*entsize) for i in range(count)]
            sh=sections[names]; f.seek(sh[4]); strings=f.read(sh[5]); result={}
            for sh in sections:
                name=strings[sh[0]:].split(b'\0',1)[0].decode()
                if name in ('.text','.rodata'):
                    f.seek(sh[4]); data=f.read(sh[5]); assert len(data)==sh[5]
                    result[name]={'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()}
            assert set(result)=={'.text','.rodata'}
            return result
    raw_sections=section_hashes(raw); rpm_sections=section_hashes(binary)
    assert raw_sections==rpm_sections,'RPM code/constant-data differs from this build binary'
    details['unstripped_to_rpm_sections']=raw_sections
    details['namespace_evidence_scope']='This build unstripped ELF; RPM .text/.rodata hashes identical. Stripped symbol-name absence is not treated as ABI absence.'
else:
    original=p/'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace'/('aarch64' if arch=='aarch64' else 'arm')/'usr/bin/bpftrace'
    with original.open('rb') as stream: original_sha=hashlib.file_digest(stream,'sha256').hexdigest()
    details['original_sha256']=original_sha
    assert static_sha==original_sha, 'RPM static payload differs from Source1002 original'
    assert 'libstdc++.so.6' in details['main_needed']
    assert 'libc++.so.1' not in details['main_needed']
details['status']='RPM_PAYLOAD_CHECK_PASS_NOT_FUNCTIONAL_EQUIVALENCE'
(out/'result.json').write_text(json.dumps(details,indent=2))
print(json.dumps(details,indent=2))
