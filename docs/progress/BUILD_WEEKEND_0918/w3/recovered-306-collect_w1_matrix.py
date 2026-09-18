"""Summarize observed W1 cells; absent or unfinished records are not successes."""
import csv
import datetime
import json
from pathlib import Path

out=Path('progress/BUILD_WEEKEND_0918')
rows=[]; packages=[]
build_cells={
    ('armv7l','gcc'):'bpf-armv7l-gcc-linkerfixed',
    ('aarch64','gcc'):'bpf-aarch64-gcc-linkerfixed',
    ('armv7l','undefined'):'bpf-armv7l-undefined-harness',
    ('aarch64','undefined'):'bpf-aarch64-undefined-harness',
    ('armv7l','libcxx'):'bpf-armv7l-libcxx',
    ('aarch64','libcxx'):'bpf-aarch64-libcxx-checkers',
}
def finished(cell):
    f=out/'cells'/cell/'events.jsonl'
    if not f.exists(): return 'NOT_OBSERVED'
    events=[json.loads(x) for x in f.read_text().splitlines()]
    end=[x for x in events if x.get('event')=='finished']
    return end[-1]['exitcode'] if end else 'NOT_OBSERVED_COMPLETION'

for arch in ('armv7l','aarch64'):
    for mode in ('libcxx','gcc','undefined'):
        build=build_cells[arch,mode]
        result=out/f'verify-bpf-{arch}-{mode}/result.json'
        audit=out/f'audit-bpf-{arch}-{mode}/result.json'
        row=dict(arch=arch,mode=mode,build_cell=build,
                 build_exitcode=finished(build),payload_status='NOT_OBSERVED',
                 static_sha256='NOT_OBSERVED',static_needed='NOT_OBSERVED',
                 compiler_commands='NOT_OBSERVED',libcxx_option_commands='NOT_OBSERVED',
                 macro_audit='NOT_OBSERVED',overall='NOT_OBSERVED',board='NOT_OBSERVED')
        data=None
        if result.exists():
            data=json.loads(result.read_text())
            row.update(payload_status=data['status'],static_sha256=data['static_binary_sha256'],
                       static_needed=';'.join(data['static_needed']))
            for rpm in data['rpms']: packages.append(dict(arch=arch,mode=mode,**rpm))
        elif arch=='aarch64' and mode=='libcxx':
            data=json.loads((out/'aarch64-first-rpm-set-result.json').read_text())
            row.update(payload_status=data['status'],static_needed=';'.join(data['static_needed']),
                       overall='FAIL_EXTRA_DYNAMIC_LIBBPF')
            for rpm in data['rpm_set']: packages.append(dict(arch=arch,mode=mode,**rpm))
        if audit.exists():
            a=json.loads(audit.read_text())
            row.update(compiler_commands=a['observed_compiler_command_count'],
                       libcxx_option_commands=a['commands_with_libcxx_options'],macro_audit=a['status'])
        if row['build_exitcode']==0 and row['payload_status']=='RPM_PAYLOAD_CHECK_PASS_NOT_FUNCTIONAL_EQUIVALENCE' and row['macro_audit']=='PASS':
            assert finished(f'check-{arch}-{mode}')==0
            if mode=='gcc':
                # Supersede the incorrectly addressed ARM supplementary check.
                h=json.loads((out/f'hal-candidate-v2-{arch}-gcc.json').read_text())
                assert all(x['exitcode']==0 for x in h['commands'])
            row['overall']='PASS_BUILD_PAYLOAD_MACRO_NOT_BOARD'
        rows.append(row)

with (out/'W1_MATRIX.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t',lineterminator='\n'); w.writeheader(); w.writerows(rows)
(out/'W1_RPM_MANIFEST.json').write_text(json.dumps(packages,ensure_ascii=False,indent=2))
(out/'W1_MATRIX.json').write_text(json.dumps({'observed_at':datetime.datetime.now().astimezone().isoformat(),
    'cells':rows,'rpm_file_count':len(packages),'scope':'Six architecture/path cells, not six RPM files; no board result implied.'},ensure_ascii=False,indent=2))
print(json.dumps(rows,ensure_ascii=False,indent=2))
