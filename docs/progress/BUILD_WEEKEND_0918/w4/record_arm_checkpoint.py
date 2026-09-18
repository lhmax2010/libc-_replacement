"""Record preserved files after the ARM build stops; no resume or cleanup."""
import datetime,hashlib,json,re
from pathlib import Path

out=Path('progress/BUILD_WEEKEND_0918')
raw=out/'raw/355_build_arm_libcxx.exitcode'
assert raw.exists(),'Wait for the original command to finish first'
cell=out/'cells/bpf-armv7l-libcxx'
events=[json.loads(x) for x in (cell/'events.jsonl').read_text().splitlines()]
base=Path('tmp/WEEKEND_0918/rpm-bpf-armv7l-libcxx')
source=base/'BUILD/bpftrace-0.24.2'
objects=[]
for f in sorted(source.rglob('*.o')):
    if not f.is_file(): continue
    with f.open('rb') as stream: digest=hashlib.file_digest(stream,'sha256').hexdigest()
    objects.append({'path':str(f),'bytes':f.stat().st_size,'sha256':digest})
caches=[]
for f in sorted(source.rglob('CMakeCache.txt')):
    data=f.read_bytes()
    caches.append({'path':str(f),'sha256':hashlib.sha256(data).hexdigest(),
                   'relevant_lines':[x for x in data.decode(errors='replace').splitlines()
                     if re.match(r'(CMAKE_(CXX_COMPILER|CXX_FLAGS|BUILD_TYPE)|STATIC_LINKING|LIBCLANG_STATIC)',x)]})
progress=re.findall(r'^\[\s*\d+%\].*$',(cell/'build.log').read_text(errors='replace'),re.M)
result={'recorded_at':datetime.datetime.now().astimezone().isoformat(),
        'original_command_guard_exitcode':int(raw.read_text().strip()),
        'build_end_events':[x for x in events if x.get('event') in ('finished','stop')],
        'last_progress_lines':progress[-5:],'preserved_build_root':str(base),
        'object_file_count':len(objects),'objects':objects,'cmake_caches':caches,
        'rpm_files':[str(x) for x in sorted((base/'RPMS').rglob('*.rpm'))],
        'scope':'File presence and hashes preserve a checkpoint; not proof that every object is complete or that an RPM passed. Make progress-line counts are not unique compilation-unit counts.',
        'resume_note':'Do not blindly rerun the original -bb: its %prep and mkdir build can replace or conflict with the preserved tree. Resume procedure must explicitly account for existing build/ and build-static/; no such recipe change has been made.'}
(out/'ARM_BUILD_CHECKPOINT.json').write_text(json.dumps(result,ensure_ascii=False,indent=2))
print('Preserved object files',len(objects),'RPM files',len(result['rpm_files']),
      'original command exitcode',result['original_command_guard_exitcode'])
