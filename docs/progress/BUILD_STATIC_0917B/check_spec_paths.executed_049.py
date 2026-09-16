import hashlib
import json
from pathlib import Path
from w1_probe import base,run,p

macro=next(s[1:] for s in Path('codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config').read_text().splitlines() if s.startswith('%toolchain_is() '))
rows=[]
for arch in ['armv7l','aarch64']:
    for mode in ['clang','gcc','undefined']:
        texts={}
        for version,spec in [('original','/tmp/night/bpftrace-source/packaging/bpftrace.spec'),('candidate','/tmp/static0917b/specs/bpftrace.spec')]:
            cmd=base+['--','rpmspec','-P','--target',arch,'--define',macro,'--define','_smp_mflags -j1']
            if mode!='undefined':
                cmd+=['--define','_toolchain '+mode]
            row=run(f'spec-{arch}-{mode}-{version}',cmd+[spec])
            assert row['exitcode']==0
            texts[version]=row['stdout']
        normalize=lambda text:'\n'.join(line for line in text.splitlines() if line.strip())
        same=normalize(texts['original'])==normalize(texts['candidate'])
        if mode!='clang':
            assert same
            assert '-stdlib=libc++' not in texts['candidate']
            assert '-lc++abi' not in texts['candidate']
            assert 'sed -i' not in texts['candidate']
        else:
            assert '"-shared-libgcc" "-static-libstdc++"' in texts['candidate']
            assert 'build-static/src/bpftrace' in texts['candidate']
        rows.append(dict(arch=arch,mode=mode,expanded_equal_ignoring_blank_lines=same,sha256=hashlib.sha256(texts['candidate'].encode()).hexdigest(),kind='RPM macro expansion only, NOT a build or install test'))
(p/'spec_path_checks.json').write_text(json.dumps(rows,indent=2))
print('PASS: six expansions; GCC/undefined unchanged. Full RPM validation remains pending.')
