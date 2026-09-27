import pathlib,os,sys,json,subprocess
E=pathlib.Path(__file__).resolve().parent
cg=pathlib.Path('/proc/self/cgroup').read_text(); rel=cg.strip().split('::')[-1]
limit=(pathlib.Path('/sys/fs/cgroup')/rel.lstrip('/')/'memory.max').read_text().strip()
total=int(next(x.split()[1] for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
expected=(total//2)//os.sysconf('SC_PAGE_SIZE')*os.sysconf('SC_PAGE_SIZE')
assert limit==str(expected),(limit,expected)
assert os.getpriority(os.PRIO_PROCESS,0)==19
io=subprocess.run(['ionice','-p',str(os.getpid())],capture_output=True,text=True)
assert io.returncode==0 and 'idle' in io.stdout
(E/('RESOURCE_'+sys.argv[1]+'.json')).write_text(json.dumps({'pid':os.getpid(),'cgroup':cg,'memory.max':limit,'total_memory_bytes':total,'nice':os.getpriority(os.PRIO_PROCESS,0),'ionice':io.stdout,'argv':sys.argv[2:]},indent=2)+'\n')
os.execvp(sys.argv[2],sys.argv[2:])
