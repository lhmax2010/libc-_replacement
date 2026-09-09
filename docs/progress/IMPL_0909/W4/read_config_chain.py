#!/usr/bin/env python3
"""只读项目配置继承链；只输出父项目及接收选项，不输出权限成员或凭据。"""
import csv,pathlib,re,subprocess,sys
r=pathlib.Path.cwd();base=r/'docs/progress/IMPL_0909/W4'
repo=r/'tmp/IMPL_0909/W4/gerrit-config-audit.git';rec=r/'docs/progress/R114/code/record.py'
project='platform/upstream/llvm';seen=set();rows=[]
for i in range(10):
    assert project not in seen and re.fullmatch(r'[A-Za-z0-9_./-]+',project) and '..' not in project
    seen.add(project)
    stem=base/'raw'/f'config_chain_{i:02}_fetch'
    command=['git','--git-dir='+str(repo),'fetch','--depth=1','--no-tags',
             'ssh://lhmax2025@review.tizen.org:29418/'+project,'refs/meta/config']
    assert subprocess.run([sys.executable,str(rec),str(stem),'timeout','40',*command]).returncode==0
    sha=subprocess.check_output(['git','--git-dir='+str(repo),'rev-parse','FETCH_HEAD'],text=True).strip()
    query=['git','--git-dir='+str(repo),'config','--blob',sha+':project.config','--get-regexp',r'^(receive\.|access\.inheritfrom)']
    qstem=base/'raw'/f'config_chain_{i:02}_receive'
    result=subprocess.run([sys.executable,str(rec),str(qstem),*query]);assert result.returncode in (0,1)
    content=pathlib.Path(str(qstem)+'.stdout').read_text()
    options=dict(line.split(' ',1) for line in content.splitlines())
    rows.append([project,sha,options.get('access.inheritfrom','All-Projects' if project!='All-Projects' else 'ROOT'),
                 options.get('receive.requirechangeid','INHERIT'),options.get('receive.requiresignedoffby','INHERIT'),
                 options.get('receive.requirecontributoragreement','INHERIT')])
    print('CONFIG',rows[-1],flush=True)
    if project=='All-Projects':break
    project=options.get('access.inheritfrom','All-Projects')
else:raise AssertionError('inheritance depth exceeded')
with (base/'GERRIT_CONFIG_CHAIN.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['project','config_sha','parent','require_change_id','require_signed_off_by','require_contributor_agreement']);w.writerows(rows)
for col in range(3,6):
    value=next((row[col] for row in rows if row[col].lower() not in ('inherit','')), 'NOT_OBSERVED')
    print('EFFECTIVE',col,value,flush=True)
print('READ_ONLY_REMOTE=YES; no review push attempted')
