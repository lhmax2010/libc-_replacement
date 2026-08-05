#!/usr/bin/env bash
set -u

ROOT=/home/toolchain/development/libc++_replacement
GBS_CONF="$ROOT/progress/T3R/config/gbs_llvm.conf"

record() {
    local command="$1"
    printf 'COMMAND=%s\n' "$command"
    (cd "$ROOT" && bash -o pipefail -c "$command")
    local rc=$?
    printf 'EXIT_CODE=%d\n\n' "$rc"
    return 0
}

record "gbs --version"
record "dpkg-query -W -f='\${Package}\\t\${Version}\\n' gbs gbs-export git-buildpackage-rpm"
record "dpkg-query -S /usr/bin/gbs /usr/lib/python3/dist-packages/gitbuildsys/cmd_export.py /usr/lib/python3/dist-packages/gbp/scripts/pq_rpm.py"
record "gbs export --help"
record "nl -ba /usr/bin/gbs | sed -n '115,140p'"
record "nl -ba /usr/lib/python3/dist-packages/gitbuildsys/conf.py | sed -n '192,210p'"
record "nl -ba /usr/lib/python3/dist-packages/gitbuildsys/cmd_export.py | sed -n '105,212p'"
record "nl -ba /usr/lib/python3/dist-packages/gitbuildsys/cmd_export.py | sed -n '214,250p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/config.py | sed -n '88,108p;138,154p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/scripts/buildpackage_rpm.py | sed -n '132,160p;238,290p;570,610p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/git/repository.py | sed -n '1627,1668p;1717,1762p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/scripts/common/pq.py | sed -n '178,280p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/scripts/pq_rpm.py | sed -n '55,85p;100,177p;197,215p'"
record "nl -ba /usr/lib/python3/dist-packages/gbp/rpm/__init__.py | sed -n '614,710p'"
record "git -C '$ROOT/codes/llvm' show --format= --binary 3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b -- . ':(exclude)packaging/**' | wc -c"

record "for f in /etc/gbs.conf /home/toolchain/.gbs.conf '$ROOT/gbs_llvm.conf' '$GBS_CONF'; do if test -f \"\$f\"; then echo FILE:\$f; rg -n '^[[:space:]]*(upstream[-_]branch|upstream[-_]tag|packaging[-_]branch|patch[-_]export|pristine[-_]tar|squash[-_]patches[-_]until)[[:space:]]*=' \"\$f\" || echo NO_RELEVANT_KEY; else echo NOT_FOUND:\$f; fi; done"

record "/usr/bin/python3 - <<'PY'
from gbp.git.repository import GitRepository
r=GitRepository('/home/toolchain/development/libc++_replacement/codes/llvm')
for c in ['bdfb133d1383f1c7e04c5f973374c911708a383b','3d3e3da49e00ae89b0ff928dee811d6d0d1d4f0b']:
    i=r.get_commit_info(c)
    base=i['patchname'][:52]
    print(c)
    print('subject='+i['subject'])
    print('patchname='+i['patchname'])
    print('first_or_second_numbered_filename_template=000N-'+base+'.patch')
print('CURRENT_TIZEN_BASE_SOURCE_PATCH_SEQUENCE')
commits=list(reversed(r.get_commits('ca7933e47d3a3451d81e72ac174dcb5aa28b59d1', 'tizen_base')))
patch_index=0
for c in commits:
    i=r.get_commit_info(c)
    source_files=[p for paths in i['files'].values() for p in paths if not p.startswith('packaging/') and p != '.gbs.conf']
    if not source_files:
        print(c+' SKIPPED_NO_SOURCE_DIFF')
        continue
    patch_index += 1
    prefix=f'{patch_index:04d}-'
    base_maxlen=63-len(prefix)-len('.patch')
    filename=prefix+i['patchname'][:base_maxlen]+'.patch'
    print(f'{c} PATCH_FILE={filename} SPEC_TAG=Patch{patch_index-1} PREP_MACRO=%patch{patch_index-1} -p1 SOURCE_FILES={len(source_files)}')
PY"
