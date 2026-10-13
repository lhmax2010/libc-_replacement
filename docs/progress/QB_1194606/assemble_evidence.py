"""Derive review evidence from immutable downloads; no build or source edits."""
import csv, difflib, hashlib, json, pathlib, re, subprocess, xml.etree.ElementTree as ET

out = pathlib.Path(__file__).resolve().parent
work = pathlib.Path('tmp/QB_1194606')
download = work / 'downloads'
for directory in ('evidence', 'sources', 'excerpts', 'candidate-diffs'):
    (out / directory).mkdir(exist_ok=True)
def sha(b): return hashlib.sha256(b).hexdigest()
def table(name, rows):
    with (out / name).open('w') as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0]), delimiter='\t')
        writer.writeheader(); writer.writerows(rows)
def numbered(lines, start=1):
    return ''.join(f'{i}\t{line}\n' for i, line in enumerate(lines, start))

# Preserve the actual simple report response; authenticated UI wrappers remain tmp.
for name in ('report_index_response.html', 'report_index.txt', 'manifest.xml'):
    (out / 'evidence' / name).write_bytes((download / name).read_bytes())
parent_raw = (download / 'build_log_page.txt').read_text().splitlines()
parent = [s.replace('\xa0', ' ') for s in parent_raw]
config_start = next(i for i, s in enumerate(parent) if s.endswith(' - PROJECT_CONFIG:')) + 2
config_end = next(i for i in range(config_start, len(parent)) if parent[i].endswith(' - ---------------------------------'))
config = '\n'.join(re.sub(r'^.*?INFO\s+- ?', '', s) for s in parent[config_start:config_end])+'\n'
(out / 'evidence/project_config.from-log').write_text(config)
(out / 'evidence/project_config.numbered-raw.txt').write_text(numbered(parent_raw[config_start:config_end], config_start+1))
required = ['Support: libc++-devel libc++abi-devel', 'Keep: libc++-devel libc++abi-devel', 'Keep: libc++ libc++abi']
config_check = {s: config.splitlines().count(s) for s in required}
config_check.update(fuse_ld_lld='-fuse-ld=lld' in config, undefined_version='--undefined-version' in config,
                    sha256=sha(config.encode()), parent_log_lines=[config_start+1, config_end])
(out / 'PROJECT_CONFIG_CHECK.json').write_text(json.dumps(config_check, indent=2)+'\n')

expected = list(csv.DictReader(pathlib.Path('docs/progress/LLVM_W4_0923/QUICKBUILD_BRANCHES_R4.tsv').open(), delimiter='\t'))
manifest = {p.attrib['name']: p.attrib['revision'] for p in ET.parse(download/'manifest.xml').getroot().findall('project')}
inputs=[]; selected=set()
for item in expected:
    repo, revision = item['gerrit_repository'], item['remote_sha']
    hits = [(i+1, s) for i, s in enumerate(parent) if (repo+'/'+revision in s or (repo in s and 'git checkout '+revision in s))]
    assert hits, (repo, revision)
    selected.update(i for i, s in hits)
    inputs.append(dict(**item, actual_execution_revision=revision, execution_evidence_lines=','.join(str(i) for i,s in hits),
                       revision_match='MATCH', actual_branch='NOT_OBSERVED (checkout by SHA)',
                       published_manifest_revision=manifest.get(repo,'NOT_OBSERVED'),
                       manifest_match='MATCH' if manifest.get(repo)==revision else 'MISMATCH'))
selected.update(i+1 for i,s in enumerate(parent) if ('libcxx-runtimes.spec' in s or 'HEAD is now at' in s))
(out / 'evidence/input-execution.numbered.txt').write_text(''.join(numbered([parent_raw[i-1]],i) for i in sorted(selected)))
table('INPUT_COMPARISON.tsv', inputs)

downloads=[]; failures=[]
for item in json.loads((out/'DOWNLOAD_PLAN.json').read_text()):
    result=json.loads((work/'results'/item['command']).read_text())
    data=pathlib.Path(item['path']).read_bytes()
    assert result['ok'] and result['status']==200 and sha(data)==result['sha256']
    downloads.append(dict(package=item['package'], architecture=item['architecture'], status=item['status'],
                          url=item['log_url'], bytes=len(data), sha256=sha(data), local_path=item['path']))
    if item['status']!='failed': continue
    lines=data.decode().splitlines(); package=item['package']
    pattern = 'undefined reference to' if package=='cereal' else ('./version:1:1: error:' if package=='icu' else 'error: Installed (but unpackaged) file(s) found:')
    anchor=next(i for i,s in enumerate(lines) if pattern in s)
    low,high=max(0,anchor-40),min(len(lines),anchor+41)
    target=f'excerpts/{package}-{item["architecture"]}.txt'
    (out/target).write_text(numbered(lines[low:high],low+1))
    failures.append(dict(package=package, architecture=item['architecture'], first_causal_error_line=anchor+1,
                         first_error=lines[anchor], category={'cereal':'其它：构建期跨标准库 ABI 链接不匹配','icu':'libc++ 头文件/API 差异：头文件名碰撞'}.get(package,'rpm 打包/文件清单'),
                         known_baseline='NEW', excerpt=target, dependency_cascade='NO'))
table('DOWNLOADS.tsv',downloads); table('FAILURES.tsv',failures)

sources=[]
def source(repo, revision, path, dest):
    argv=['git','-C',repo,'show',revision+':'+path]
    result=subprocess.run(argv,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    assert result.returncode==0, (argv,result.stderr.decode())
    (out/'sources'/dest).parent.mkdir(parents=True,exist_ok=True)
    (out/'sources'/dest).write_bytes(result.stdout)
    sources.append(dict(repository=repo, revision=revision, path=path, saved='sources/'+dest,
                        sha256=sha(result.stdout), command=json.dumps(argv), exitcode=result.returncode))
    return result.stdout.decode()
def patch(name, files):
    text=''
    for path, before, after in files:
        assert before != after
        text+=''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
    (out/'candidate-diffs'/name).write_text(text)

cereal_repo='tmp/QB_1194606/cereal-repo'; cereal_rev=manifest['platform/upstream/cereal']
cs=source(cereal_repo,cereal_rev,'packaging/cereal.spec','cereal/cereal.spec')
source(cereal_repo,cereal_rev,'CMakeLists.txt','cereal/CMakeLists.txt')
source(cereal_repo,cereal_rev,'sandbox/CMakeLists.txt','cereal/sandbox-CMakeLists.txt')
needle='%build\nmkdir build\n'
replacement='''%build
%global cereal_libcpp OFF
%if %{defined _toolchain}
%if %{toolchain_is clang}
%global cereal_libcpp ON
%endif
%endif
mkdir build
'''
assert cs.count(needle)==1
changed=cs.replace(needle,replacement).replace('-DSKIP_PORTABILITY_TEST=ON','-DCLANG_USE_LIBCPP=%{cereal_libcpp} -DSKIP_PORTABILITY_TEST=ON')
patch('cereal-libcpp-build-only.patch',[('packaging/cereal.spec',cs,changed)])
patch('cereal-skip-performance-ALTERNATIVE.patch',[('packaging/cereal.spec',cs,cs.replace('-DSKIP_PORTABILITY_TEST=ON','-DSKIP_PERFORMANCE_COMPARISON=ON -DSKIP_PORTABILITY_TEST=ON'))])

icu_repo='codes/R104/icu'; icu_rev=next(r['remote_sha'] for r in expected if r['obs_package']=='icu')
source(icu_repo,icu_rev,'packaging/icu.spec','icu/icu.spec')
im=source(icu_repo,icu_rev,'source/tools/tzcode/Makefile.in','icu/Makefile.in')
source(icu_repo,icu_rev,'source/tools/tzcode/tz2icu.cpp','icu/tz2icu.cpp')
old='$(CXX) $(CXXFLAGS) -I$(srcdir) -I$(top_srcdir)/common $(srcdir)/tz2icu.cpp -o $@'
assert im.count(old)==1
patch('icu-tz2icu-quote-search.patch',[('source/tools/tzcode/Makefile.in',im,im.replace(old,old.replace('-I$(srcdir)','-iquote $(srcdir)')))])

qrepo='codes/qemu-accel'; qrev=manifest['platform/upstream/qemu-accel']; changes=[]
for name in ('qemu-accel.spec.in','qemu-accel-armv7l.spec','qemu-accel-aarch64.spec','qemu-accel-riscv64.spec'):
    path='packaging/'+name
    qs=source(qrepo,qrev,path,'qemu-accel/'+name)
    mark='## clang-accel\n# Prepare clang files list\n'
    assert qs.count(mark)==1
    new='''## clang-accel
# Include copied libc++ SONAME paths in the generated file list.
for library in libc++.so.1 libc++abi.so.1; do
  if test -e "${buildroot_emul_path}%{_libdir}/$library"; then
    echo "%{_libdir}/$library" >> clang.list
  fi
done
# Prepare clang files list
'''
    changes.append((path,qs,qs.replace(mark,new)))
patch('qemu-accel-libcpp-file-list.patch',changes)
source(qrepo,qrev,'packaging/pre_checkin.sh','qemu-accel/pre_checkin.sh')
table('SOURCE_IDENTITIES.tsv',sources)
print(json.dumps({'logs':len(downloads),'log_bytes':sum(x['bytes'] for x in downloads),
                  'failed_cells':len(failures),'config':config_check,
                  'manifest_mismatches':[x['obs_package'] for x in inputs if x['manifest_match']!='MATCH']},ensure_ascii=False,indent=2))
