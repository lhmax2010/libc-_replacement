#!/usr/bin/env python3
"""补充精确检索的正向对照和 gRPC 调用范围；不执行归档中的内容。"""
import pathlib, tarfile, re, hashlib, json
r=pathlib.Path('docs/progress/R116')
terms=['pthread_cancel','pthread_setcanceltype','PTHREAD_CANCEL_ASYNCHRONOUS','std::shared_mutex','std::shared_timed_mutex','wbuffer_convert']
fixture='\n'.join(terms)+'\n#include <boost/locale.hpp>\n'
for term in terms:
    assert sum(term in line for line in fixture.splitlines())==1
    print('SYNTHETIC_POSITIVE',term,'PASS')
assert re.search(r'#\s*include\s*[<\"]boost/',fixture)
print('SYNTHETIC_POSITIVE boost_include PASS')
(r/'sources/search_positive_control.txt').write_text(fixture)
with tarfile.open('tmp/R116/chromium-src-complete.tar.gz','r|gz') as tar:
    for m in tar:
        if not m.isfile() or m.size>16*1024*1024: continue
        wanted=m.name in ['third_party/grpc/BUILD.gn','third_party/protobuf/third_party/utf8_range/CMakeLists.txt','third_party/protobuf/third_party/utf8_range/README.md']
        scan=m.name.startswith('third_party/grpc/') and pathlib.Path(m.name).suffix in ['.cc','.h','.gn','.gni']
        if not wanted and not scan: continue
        data=tar.extractfile(m).read(); text=data.decode('utf-8',errors='replace')
        hits=[(n,line) for n,line in enumerate(text.splitlines(),1) if 'Thread::Kill' in line or 'util/posix/thd.cc' in line or 'boost.cpp' in line]
        if wanted or hits:
            key=hashlib.sha256(m.name.encode()).hexdigest()[:16]
            (r/'sources/corpus_matches'/(key+'.txt')).write_bytes(data)
            (r/'sources/corpus_matches'/(key+'.path')).write_text(m.name+'\nsha256='+hashlib.sha256(data).hexdigest()+'\n')
            for n,line in hits: print(m.name,n,line.strip())
data=json.loads((r/'sources/angle_fix.txt').read_text()[4:])
rev=data['current_revision']
print('ANGLE',data['status'],rev,data['revisions'][rev]['commit']['message'])
