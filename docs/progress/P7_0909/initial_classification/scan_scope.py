#!/usr/bin/env python3
"""第一阶段：冻结 RPM 文件清单与安装头文件形态；不做接口兼容性裁决。"""
import concurrent.futures
import collections
import csv
import datetime
import gzip
import hashlib
import json
import pathlib
import re
import shlex
import stat
import subprocess
import time
import xml.etree.ElementTree as ET

ROOT = pathlib.Path.cwd()
OUT = ROOT/'docs/progress/P7_0909'
R11 = pathlib.Path('/home/toolchain/development/libc++_replacement/progress/R11')
CACHE = pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
NS = {'f':'http://linux.duke.edu/metadata/filelists','r':'http://linux.duke.edu/metadata/repo'}
HEAD_EXT = {'.h','.hh','.hpp','.hxx','.ipp','.tcc','.inc','.inl','.h++'}

def load(path):
    with path.open(newline='') as f:
        return list(csv.DictReader(f,delimiter='\t'))

def write(name,fields,rows):
    p=OUT/'tables'/name;p.parent.mkdir(parents=True,exist_ok=True)
    with p.open('w',newline='') as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter='\t',lineterminator='\n',extrasaction='ignore')
        w.writeheader();w.writerows(rows)

def digest(path,algorithm='sha256'):
    h=hashlib.new(algorithm)
    with path.open('rb') as f:
        for block in iter(lambda:f.read(4*1024*1024),b''):h.update(block)
    return h.hexdigest()

def header(path):
    p=pathlib.PurePosixPath(path)
    return p.suffix.lower() in HEAD_EXT or (path.startswith('/usr/include/') and not p.suffix)

def category(row):
    name=row['name'];src=row['sourcerpm']
    if name.startswith('capi-') or src.startswith('capi-'):return 'NATIVE_CAPI_CANDIDATE'
    if re.match(r'(dali|efl(?:-|\.)|libscl-ui)',src) or 'nui' in name.lower():return 'UI_FRAMEWORK'
    if re.match(r'(chromium|lightweight-web-engine|webapi-plugins|wrt|webkit|webview|vc-webview)',src):return 'WEB_ENGINE_RELATED'
    if row['repo_id']=='unified_binary' and not re.match(r'(kernel|linux|gcc|llvm|clang|python|go-|dotnet|mono|rust|cmake|meson|swig|ninja)',src):return 'SERVICE_MIDDLEWARE_CANDIDATE'
    return 'OTHER'

TOKEN=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)
CPP=[('namespace',r'\bnamespace\s*(?:[A-Za-z_]\w*|\{)'),('class',r'\bclass\s+(?:[A-Za-z_]\w*|\[)'),('template',r'\btemplate\s*<'),('std',r'\bstd\s*::'),('using_alias',r'\busing\s+\w+\s*='),('cpp_keyword',r'\b(?:constexpr|consteval|constinit|decltype|typename|noexcept)\b'),('access_specifier',r'\b(?:public|private|protected)\s*:')]
CPP=[(name,re.compile(pattern)) for name,pattern in CPP]
CDECL=re.compile(r'\btypedef\b|\b(?:struct|enum|union)\s+\w*\s*\{|\b[A-Za-z_]\w*\s*\([^;{}]*\)\s*;',re.S)

def shape(text,path):
    def clean(m):
        s=m.group();return '"C"' if s=='"C"' else ''.join('\n' if ch=='\n' else ' ' for ch in s)
    code=TOKEN.sub(clean,text)
    cpp=[(n,m.start()) for n,pattern in CPP if (m:=pattern.search(code))]
    linkage=re.search(r'\bextern\s*"C"|\b(?:__BEGIN_DECLS|G_BEGIN_DECLS|EINA_BEGIN_DECLS)\b',code)
    c=CDECL.search(code)
    if cpp:
        form='CPP_DECLARATION_SIGNAL'
        n,pos=min(cpp,key=lambda x:x[1])
    elif c:
        form='C_DECLARATION_SHAPE';n,pos='declaration',c.start()
    else:
        form='UNRESOLVED_OR_INCLUDE_MACRO_ONLY';n,pos='none',0
    private=bool(re.search(r'(^|[/_.-])(?:internal|private|integration-api|devel-api)([/_.-]|$)',path))
    public=bool(re.search(r'[@\\](?:ingroup|addtogroup)\s+CAPI_|/public-api/',text if '/public-api/' not in path else text+path))
    line=code.count('\n',0,pos)+1
    lines=text.splitlines()
    return dict(form=form,cpp_signal=n if cpp else '',extern_c_marker='YES' if linkage else 'NO',private_path='YES' if private else 'NO',public_marker='YES' if public else 'NO',evidence_line=line,evidence=lines[line-1].strip()[:600] if lines else '')

def controls():
    cases=[('c_guard.h','#ifdef __cplusplus\nextern "C" {\n#endif\nint sample_get(int h);\n#ifdef __cplusplus\n}\n#endif\n','C_DECLARATION_SHAPE'),
           ('cpp_builtin.h','class Engine { public: int width() const; };\n','CPP_DECLARATION_SIGNAL'),
           ('cpp_std.hpp','#include <string>\nstd::string name();\n','CPP_DECLARATION_SIGNAL'),
           ('comment_only_cpp.h','// class Fake { std::string x; };\n/* namespace ignored {} */\nint value(void);\n','C_DECLARATION_SHAPE'),
           ('mixed.h','extern "C" { int value(void); }\n#ifdef __cplusplus\nnamespace app { class View {}; }\n#endif\n','CPP_DECLARATION_SIGNAL'),
           ('umbrella.h','#include <real_api.h>\n#define VERSION 1\n','UNRESOLVED_OR_INCLUDE_MACRO_ONLY')]
    results=[]
    for name,text,expected in cases:
        p=OUT/'controls'/name;p.parent.mkdir(exist_ok=True);p.write_text(text)
        found=shape(text,name);assert found['form']==expected,(name,found)
        results.append(dict(name=name,expected=expected,actual=found['form'],result='PASS'))
    write('positive_controls.tsv',['name','expected','actual','result'],results)

def main():
    controls()
    records=load(R11/'tables/binary_package_records.tsv')
    downloads={r['checksum']:r for r in load(R11/'tables/binary_download_status.tsv')}
    snapshots={r['repo_id']:r for r in load(R11/'tables/snapshot_identity.tsv')}
    files_by_sha={};identities=[]
    inputs=OUT/'inputs';inputs.mkdir(exist_ok=True)
    for name in ['binary_package_records.tsv','snapshot_identity.tsv']:
        (inputs/name).write_bytes((R11/'tables'/name).read_bytes())
    for repo in ['base_binary','unified_binary']:
        repomd=CACHE/'repodata'/f'{repo}.repomd.xml'
        assert digest(repomd)==snapshots[repo]['repomd_sha256']
        (inputs/repomd.name).write_bytes(repomd.read_bytes())
        tree=ET.parse(repomd)
        expected=tree.find("r:data[@type='filelists']/r:checksum",NS)
        lists=CACHE/'repodata'/f'{repo}.filelists.xml.gz'
        assert digest(lists,expected.attrib['type'])==expected.text
        (inputs/lists.name).write_bytes(lists.read_bytes())
        identities.append(dict(repo=repo,repomd_sha256=digest(repomd),filelists_sha256=digest(lists),source=str(lists),result='MATCH_FROZEN_METADATA'))
        with gzip.open(lists,'rb') as f:
            for _,elem in ET.iterparse(f,events=('end',)):
                if elem.tag!='{'+NS['f']+'}package':continue
                sha=elem.attrib['pkgid']
                files=[(e.text,e.attrib.get('type','file')) for e in elem.findall('f:file',NS)]
                files_by_sha[sha]=files;elem.clear()
    assert all(r['checksum'] in files_by_sha for r in records)
    write('input_identity.tsv',['repo','repomd_sha256','filelists_sha256','source','result'],identities)
    groups=collections.defaultdict(list)
    all_rows=[]
    for r in records:
        paths=files_by_sha[r['checksum']]
        hs=[p for p,t in paths if t!='dir' and header(p)]
        includes=[p for p in hs if '/include/' in p or p.startswith('/usr/include/')]
        devel=bool(re.search(r'(^|-)devel(?:-|$)|(^|-)headers(?:-|$)',r['name']))
        row={**r,'devel_name':'YES' if devel else 'NO','header_paths':len(hs),'include_header_paths':len(includes),'category':category(r)}
        all_rows.append(row)
        groups[(r['repo_id'],r['name'],r['epoch'],r['version'],r['release'])].append(row)
    fields=['repo_id','name','arch','epoch','version','release','sourcerpm','checksum','devel_name','header_paths','include_header_paths','category']
    write('all_binary_records.tsv',fields,all_rows)
    priority={'x86_64':0,'noarch':1,'aarch64':2,'armv7l':3}
    selected=[];logical=[]
    for key,variants in sorted(groups.items()):
        chosen=min(variants,key=lambda r:(priority.get(r['arch'],10),r['arch']))
        paths_by_arch={r['arch']:{p for p,t in files_by_sha[r['checksum']] if t!='dir' and header(p)} for r in variants}
        union=set().union(*paths_by_arch.values());intersection=set.intersection(*paths_by_arch.values())
        row={**chosen,'available_arches':','.join(sorted(paths_by_arch)),'header_path_sets_equal':'YES' if union==intersection else 'NO','variant_count':len(variants)}
        logical.append(row)
        if chosen['header_paths'] or chosen['devel_name']=='YES':selected.append(row)
    write('logical_packages.tsv',fields+['available_arches','header_path_sets_equal','variant_count'],logical)
    write('candidate_packages_before_scan.tsv',fields+['available_arches','header_path_sets_equal','variant_count'],selected)
    print(f'FROZEN binary_records={len(records)} logical_packages={len(logical)} selected_candidates={len(selected)}',flush=True)
    header_rows=[];package_rows=[];failures=[]
    def scan(item):
        index,r=item
        sha=r['checksum'];download=downloads[sha];rpm=pathlib.Path(download['target'])
        stem=OUT/'raw'/'rpm_queries'/f'{index:04d}_{r["name"]}_{r["arch"]}'
        stem.parent.mkdir(parents=True,exist_ok=True)
        query='%{NAME}\n%{SUMMARY}\n%{GROUP}\n%{FILEDIGESTALGO}\n[%{FILENAMES}\t%{FILEMODES}\t%{FILEDIGESTS}\t%{FILELINKTOS}\n]'
        command=['rpm','-qp','--qf',query,str(rpm)]
        pathlib.Path(str(stem)+'.command.txt').write_text('cwd: '+str(ROOT)+'\ncommand: '+shlex.join(command)+'\n')
        started=datetime.datetime.now(datetime.timezone.utc).isoformat()
        try:
            p=subprocess.run(command,capture_output=True,timeout=60)
        except (OSError,subprocess.TimeoutExpired) as exc:
            pathlib.Path(str(stem)+'.stderr').write_text(str(exc));pathlib.Path(str(stem)+'.exitcode').write_text('NOT_AVAILABLE\n')
            return {**r,'scan_status':'NOT_AVAILABLE','detail':str(exc)},[],[dict(package=r['name'],path=str(rpm),reason=str(exc))]
        pathlib.Path(str(stem)+'.stdout').write_bytes(p.stdout);pathlib.Path(str(stem)+'.stderr').write_bytes(p.stderr)
        pathlib.Path(str(stem)+'.exitcode').write_text(str(p.returncode)+'\n')
        pathlib.Path(str(stem)+'.time.json').write_text(json.dumps(dict(start=started,end=datetime.datetime.now(datetime.timezone.utc).isoformat()))+'\n')
        if p.returncode:
            return {**r,'scan_status':'NOT_AVAILABLE','detail':'RPM_QUERY_FAILED'},[],[dict(package=r['name'],path=str(rpm),reason='RPM_QUERY_FAILED')]
        actual=digest(rpm)
        assert actual==sha,(r['name'],'RPM_DIGEST_CHANGED')
        lines=p.stdout.decode('utf-8',errors='replace').splitlines()
        assert lines[0]==r['name']
        summary,group,algo=lines[1:4]
        algo={'1':'md5','2':'sha1','8':'sha256','10':'sha512'}.get(algo)
        data={}
        for line in lines[4:]:
            bits=line.split('\t')
            assert len(bits)==4,(r['name'],line)
            data[bits[0]]=bits[1:]
        outrows=[];errors=[]
        payload=CACHE/'extracted'/sha[:2]/sha
        hpaths=[p for p,t in files_by_sha[sha] if t!='dir' and header(p)]
        for path in hpaths:
            result=dict(repo_id=r['repo_id'],name=r['name'],sourcerpm=r['sourcerpm'],arch=r['arch'],category=r['category'],rpm_sha256=sha,path=path,scope='INCLUDE_TREE' if '/include/' in path else 'OTHER_INSTALLED_PATH',status='PASS',file_sha256='',rpm_digest='',form='NOT_AVAILABLE',cpp_signal='',extern_c_marker='',private_path='',public_marker='',evidence_line='',evidence='',cache_path=str(payload/path.lstrip('/')))
            if path not in data:
                result.update(status='NOT_AVAILABLE',evidence='FILELIST_NOT_IN_RPM_HEADER');errors.append(dict(package=r['name'],path=path,reason=result['evidence']));outrows.append(result);continue
            mode,expected,link=data[path];local=payload/path.lstrip('/')
            if stat.S_ISLNK(int(mode)):
                # 不跨 RPM 追绝对符号链接，以免误读宿主机头文件。
                result.update(status='SYMLINK_NOT_FOLLOWED',evidence=link);outrows.append(result);continue
            if not local.is_file() or local.is_symlink():
                result.update(status='NOT_AVAILABLE',evidence='CACHED_FILE_UNAVAILABLE');errors.append(dict(package=r['name'],path=path,reason=result['evidence']));outrows.append(result);continue
            raw=local.read_bytes();result['file_sha256']=hashlib.sha256(raw).hexdigest();result['rpm_digest']=expected
            if not algo or (expected and hashlib.new(algo,raw).hexdigest()!=expected):
                result.update(status='DIGEST_MISMATCH',evidence='NOT_USED');errors.append(dict(package=r['name'],path=path,reason=result['status']));outrows.append(result);continue
            if b'\0' in raw:
                result.update(status='NOT_TEXT',form='NOT_AVAILABLE');outrows.append(result);continue
            result.update(shape(raw.decode('utf-8',errors='replace'),path));outrows.append(result)
        forms=collections.Counter(x['form'] for x in outrows if x['scope']=='INCLUDE_TREE')
        private=sum(x['private_path']=='YES' for x in outrows if x['scope']=='INCLUDE_TREE')
        public=sum(x['public_marker']=='YES' for x in outrows if x['scope']=='INCLUDE_TREE')
        package={**r,'scan_status':'PARTIAL' if errors else 'PASS','detail':'','rpm_sha256_actual':actual,'summary':summary,'rpm_group':group,'c_shape_headers':forms['C_DECLARATION_SHAPE'],'cpp_signal_headers':forms['CPP_DECLARATION_SIGNAL'],'unresolved_headers':forms['UNRESOLVED_OR_INCLUDE_MACRO_ONLY'],'unavailable_headers':forms['NOT_AVAILABLE'],'private_path_headers':private,'public_marker_headers':public,'rpm_url':download['url']}
        return package,outrows,errors
    with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
        for index,(package,headers,errors) in enumerate(pool.map(scan,enumerate(selected,1)),1):
            package_rows.append(package);header_rows.extend(headers);failures.extend(errors)
            if index%50==0:print(f'SCANNED {index}/{len(selected)} headers={len(header_rows)} failures={len(failures)}',flush=True)
    write('packages.tsv',fields+['available_arches','header_path_sets_equal','variant_count','scan_status','detail','rpm_sha256_actual','summary','rpm_group','c_shape_headers','cpp_signal_headers','unresolved_headers','unavailable_headers','private_path_headers','public_marker_headers','rpm_url'],package_rows)
    write('headers.tsv',['repo_id','name','sourcerpm','arch','category','rpm_sha256','path','scope','status','file_sha256','rpm_digest','form','cpp_signal','extern_c_marker','private_path','public_marker','evidence_line','evidence','cache_path'],header_rows)
    write('failures.tsv',['package','path','reason'],failures)
    bycat=[]
    for cat in ['NATIVE_CAPI_CANDIDATE','UI_FRAMEWORK','WEB_ENGINE_RELATED','SERVICE_MIDDLEWARE_CANDIDATE','OTHER']:
        packs=[r for r in package_rows if r['category']==cat];hs=[r for r in header_rows if r['category']==cat and r['scope']=='INCLUDE_TREE'];counts=collections.Counter(r['form'] for r in hs)
        bycat.append(dict(category=cat,candidate_binary_groups=len(packs),header_binary_groups=sum(int(r['include_header_paths'])>0 for r in packs),source_rpm_records=len({r['sourcerpm'] for r in packs if int(r['include_header_paths'])>0}),c_only_shape_packages=sum(int(r.get('c_shape_headers',0))>0 and int(r.get('cpp_signal_headers',0))==0 and int(r.get('unavailable_headers',0))==0 for r in packs),cpp_signal_packages=sum(int(r.get('cpp_signal_headers',0))>0 for r in packs),include_headers=len(hs),c_shape_headers=counts['C_DECLARATION_SHAPE'],cpp_signal_headers=counts['CPP_DECLARATION_SIGNAL'],unresolved_headers=counts['UNRESOLVED_OR_INCLUDE_MACRO_ONLY'],unavailable_headers=counts['NOT_AVAILABLE']))
    write('category_summary.tsv',list(bycat[0]),bycat)
    summary=dict(binary_records=len(records),logical_binary_packages=len(logical),candidate_binary_packages=len(selected),devel_name_records=sum(r['devel_name']=='YES' for r in all_rows),devel_name_groups=sum(r['devel_name']=='YES' for r in logical),headers_all_installed_paths=len(header_rows),header_status=dict(collections.Counter(r['status'] for r in header_rows)),failures=len(failures),selected_arches=dict(collections.Counter(r['arch'] for r in selected)))
    (OUT/'SUMMARY.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary),flush=True)

if __name__=='__main__':main()
