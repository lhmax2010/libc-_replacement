"""Strict read-only validation of the two review commits; archive evidence."""
import hashlib,json,pathlib,re,subprocess,sys
E=pathlib.Path(__file__).resolve().parent
R=pathlib.Path('tmp/PRJCONF_REVIEW_1012/repo')
P='TIZEN/Tizen/Tizen-Base-Toolchain/project_config'
label=sys.argv[1]; commands=[]
def git(*args):
    argv=['git','-C',str(R),*args]
    p=subprocess.run(argv,capture_output=True)
    commands.append({'argv':argv,'exitcode':p.returncode,'stdout':p.stdout.decode(errors='replace'),'stderr':p.stderr.decode(errors='replace')})
    (E/(label+'_commands.json')).write_text(json.dumps(commands,ensure_ascii=False,indent=2)+'\n')
    assert p.returncode==0,argv
    return p.stdout
base=git('rev-parse','origin/master').decode().strip()
shas=git('rev-list','--reverse','origin/master..HEAD').decode().splitlines();assert len(shas)==2
assert git('rev-parse',shas[0]+'^').decode().strip()==base
assert git('rev-parse',shas[1]+'^').decode().strip()==shas[0]
before=git('show','origin/master:'+P)
assert before.count(b'-fuse-ld=lld ')==1
after_revert=before.replace(b'-fuse-ld=lld ',b'')
assert git('show',shas[0]+':'+P)==after_revert
pairs=[(b'Support: libstdc++-devel\n',b'Support: libc++-devel libc++abi-devel\n'),(b'Keep: libstdc++-devel\n',b'Keep: libc++-devel libc++abi-devel\n'),(b'Keep: cpp gcc libstdc++\n',b'Keep: libc++ libc++abi\n')]
expected=after_revert
for anchor,addition in pairs:
    assert expected.count(anchor)==1 and addition not in expected
    expected=expected.replace(anchor,anchor+addition)
actual=git('show','HEAD:'+P);assert actual==expected and (R/P).read_bytes()==actual
assert git('diff','--name-only','origin/master..HEAD').decode().splitlines()==[P]
assert not git('status','--porcelain')
assert b'fuse-ld=lld' not in actual and b'undefined-version' not in actual
assert not re.search(rb'(?m)^(?:Support|Keep):.*\blld\b',actual)
for path in ('Tizen-Base','Tizen-Unified','Tizen-Unified-Toolchain'):
    name='TIZEN/Tizen/'+path+'/project_config'
    assert git('rev-parse','origin/master:'+name)==git('rev-parse','HEAD:'+name)
records=[]
for sha,msgfile in zip(shas,('REVERT_MESSAGE.txt','LIBCXX_MESSAGE.txt')):
    identity=git('show','-s','--format=%an <%ae>%n%cn <%ce>',sha).decode().splitlines()
    assert identity==['Hao Lin <hao.lin@samsung.com>']*2
    message=git('show','-s','--format=%B',sha).decode()
    ids=re.findall(r'^Change-Id: (I[0-9a-f]{40})$',message,re.M);assert len(ids)==1
    signoffs=re.findall(r'^Signed-off-by: (.+)$',message,re.M);assert signoffs==['Hao Lin <hao.lin@samsung.com>']
    body=re.sub(r'(?m)^(?:Change-Id|Signed-off-by):.*\n?','',message).strip()
    assert body==(E/msgfile).read_text().strip()
    assert git('diff-tree','--no-commit-id','--name-only','-r',sha).decode().splitlines()==[P]
    records.append({'sha':sha,'author':identity[0],'committer':identity[1],'Change-Id':ids[0],'Signed-off-by':signoffs[0]})
    (E/(label+'_'+msgfile.replace('_MESSAGE.txt','')+'_COMMIT.diff')).write_bytes(git('show','--format=fuller',sha))
assert records[0]['Change-Id']!=records[1]['Change-Id']
git('diff','--check','origin/master..HEAD')
diff=git('diff','origin/master..HEAD');(E/(label+'_FINAL_DIFF.patch')).write_bytes(diff)
result={'result':'PASS','baseline':base,'commits':records,'file_sha256':hashlib.sha256(actual).hexdigest(),'files_changed':[P],'revert_exact_single_flag_removal':True,'libcxx_exact_three_additions':True,'other_three_configs_unchanged':True,'worktree_clean':True,'fuse_ld_lld':False,'undefined_version':False,'Support_Keep_lld':False,'added_lines':[b.decode().strip() for a,b in pairs]}
(E/(label+'_VALIDATION.json')).write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False,indent=2))
