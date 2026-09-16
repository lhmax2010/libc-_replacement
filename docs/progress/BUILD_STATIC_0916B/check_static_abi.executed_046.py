import hashlib,json,subprocess
from pathlib import Path
p=Path('progress/BUILD_STATIC_0916B'); out=[]
for arch in ['x86_64','aarch64']:
    library=Path(f'tmp/NIGHT_0917/builds/runtime-{arch}-libcxx/lib/libc++abi.a')
    obj=Path(f'tmp/NIGHT_0917/builds/runtime-{arch}-libcxx/libcxxabi/src/CMakeFiles/cxxabi_static.dir/cxa_personality.cpp.o')
    commands=[]
    def run(cmd):
        r=subprocess.run(cmd,capture_output=True)
        commands.append(dict(command=cmd,exitcode=r.returncode,stdout_sha256=hashlib.sha256(r.stdout).hexdigest(),stderr=r.stderr.decode(errors='replace')))
        assert r.returncode==0
        return r.stdout
    member=run(['ar','p',str(library),'cxa_personality.cpp.o'])
    assert obj.read_bytes()==member
    names=run(['nm','--defined-only',str(library)]).decode(errors='replace')
    shared=Path(f'tmp/NIGHT_0917/builds/runtime-{arch}-libcxx/lib/libc++abi.so.1.0')
    dynamic=run(['nm','-D','--defined-only',str(shared)]).decode(errors='replace')
    binary=Path(f'tmp/NIGHT_0917/builds/bpftrace-{arch}-static/src/bpftrace')
    stringdata=run(['strings',str(binary)]).decode(errors='replace')
    out.append(dict(arch=arch,commands=commands,archive_sha256=hashlib.sha256(library.read_bytes()).hexdigest(),
      personality_member_sha256=hashlib.sha256(member).hexdigest(),personality_object=str(obj),member_equals_object=True,
      static_defined_markers=[x for x in names.splitlines() if '__forced_unwind' in x or '__gxx_personality_v0' in x],
      dynamic_defined_markers=[x for x in dynamic.splitlines() if '__forced_unwind' in x or '__gxx_personality_v0' in x],
      literal_user_pattern_hits=[x for x in stringdata.splitlines() if '_ZN10__cxxabiv114__forced_unwind' in x],
      actual_marker_hits=[x for x in stringdata.splitlines() if '__forced_unwind' in x]))
(p/'static_abi_identity.json').write_text(json.dumps(out,indent=2))
print('Both archives: linked personality member equals current compiled object; marker evidence recorded without assuming byte identity with shared object.')

