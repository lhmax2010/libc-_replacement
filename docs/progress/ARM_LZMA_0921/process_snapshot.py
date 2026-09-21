import datetime,json,pathlib,subprocess
b=pathlib.Path('progress/ARM_LZMA_0921');work=pathlib.Path('tmp/ARM_LZMA_0921')
header=(work/'rpmbuild-lzma.calls.tsv').open().readline()
pid=int(header.split('pid=')[1].split()[0]);p=pathlib.Path('/proc')/str(pid)
row={'time':datetime.datetime.now().astimezone().isoformat(),'pid':pid,'tap_header':header,'command':p.joinpath('cmdline').read_bytes().split(b'\0')[:-1]}
row['command']=[v.decode(errors='replace') for v in row['command']]
row['selected_environment']={}
for v in p.joinpath('environ').read_bytes().split(b'\0'):
    if v.startswith((b'QEMU_RESERVED_VA=',b'CMAKE_BUILD_PARALLEL_LEVEL=',b'RPM_BUILD_NCPUS=',b'MAKEFLAGS=')):
        k,x=v.split(b'=',1);row['selected_environment'][k.decode()]=x.decode()
for filename in ('status','maps','cgroup'):
    row['host_proc_'+filename]=p.joinpath(filename).read_text()
cg=next(s.split(':',2)[2] for s in row['host_proc_cgroup'].splitlines() if s.startswith('0::'))
for name in ('memory.max','memory.events'):
    row[name]=(pathlib.Path('/sys/fs/cgroup')/cg.lstrip('/')/name).read_text()
for cmd in (['ps','-p',str(pid),'-o','pid,ppid,ni,stat,etime,comm'],['ionice','-p',str(pid)]):
    r=subprocess.run(cmd,capture_output=True,text=True)
    row.setdefault('checks',[]).append({'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
assert row['memory.max'].strip()=='16536457216'
assert row['selected_environment']['QEMU_RESERVED_VA']=='0x100000000'
assert row['checks'][1]['stdout'].strip()=='idle'
name=b/('rpmbuild-host-snapshot-'+datetime.datetime.now().strftime('%H%M%S')+'.json')
name.write_text(json.dumps(row,indent=2)+'\n')
print(json.dumps({'saved':str(name),'pid':pid,'command':row['command'],'memory.max':row['memory.max'],'selected_environment':row['selected_environment'],'checks':row['checks']}))
