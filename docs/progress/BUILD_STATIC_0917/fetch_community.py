import hashlib, json, subprocess, urllib.parse
from pathlib import Path
from probes import p, run
out=Path('tmp/STATIC_0917/community'); out.mkdir(exist_ok=True)
urls={
 'upstream-src-cmake':'https://raw.githubusercontent.com/bpftrace/bpftrace/v0.24.2/src/CMakeLists.txt',
 'upstream-current-cmake':'https://raw.githubusercontent.com/bpftrace/bpftrace/master/src/CMakeLists.txt',
 'upstream-tree':'https://api.github.com/repos/bpftrace/bpftrace/git/trees/v0.24.2?recursive=1',
 'upstream-release':'https://api.github.com/repos/bpftrace/bpftrace/releases/tags/v0.24.2',
 'gcc-hidden-unwinder':'https://gcc.gnu.org/legacy-ml/gcc/2012-03/msg00106.html',
 'gcc-bug-42850-mail':'https://gcc.gnu.org/pipermail/gcc-bugs/2010-January/327517.html',
 'glibc-cancel-help':'https://sourceware.org/legacy-ml/libc-help/2016-10/msg00012.html',
 'gcc-bug-42850':'https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42850',
 'glibc-bug-13378':'https://sourceware.org/bugzilla/show_bug.cgi?id=13378',
}
for term in ['pthread_cancel','static-libgcc','libgcc','_Unwind','SIGABRT','unwind','static']:
    urls['issues-'+term]='https://api.github.com/search/issues?'+urllib.parse.urlencode({'q':'repo:bpftrace/bpftrace '+term,'per_page':100})
manifest=[]
for name,url in urls.items():
    path=out/(name+'.response')
    r=run('fetch-'+name,['curl','-sS','-L','--max-time','40','--retry','0','-o',str(path),'-w','%{http_code} %{url_effective} %{size_download}\n',url],50)
    manifest.append(dict(name=name,url=url,result=r['stdout'].strip(),exitcode=r['exitcode'],path=str(path),sha256=hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else 'NOT_AVAILABLE'))
    if name.startswith('issues-') and path.exists():
        try:
            j=json.loads(path.read_text()); summary={k:j.get(k) for k in ['total_count','incomplete_results','message']}
            summary['items']=[{k:x.get(k) for k in ['number','title','html_url','state','body']} for x in j.get('items',[])]
            (p/(name+'.json')).write_text(json.dumps(summary,indent=2))
        except ValueError: pass
(p/'community_sources.json').write_text(json.dumps(manifest,indent=2))
