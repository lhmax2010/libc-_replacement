"""Record command argv, exact outputs, status and timing (build deadlines handled separately)."""
import datetime,json,pathlib,shlex,subprocess,sys
E=pathlib.Path(__file__).resolve().parent;R=E/'raw';R.mkdir(exist_ok=True)
deadline=datetime.datetime.fromisoformat('2026-09-28T08:30:00+08:00')
label,*argv=sys.argv[1:];base=R/label
assert not base.with_suffix('.argv.json').exists()
now=datetime.datetime.now(datetime.timezone.utc)
for suffix,value in [('argv.json',json.dumps(argv,ensure_ascii=False,indent=2)+'\n'),('command.txt',shlex.join(argv)+'\n'),('started.txt',now.isoformat()+'\n')]:pathlib.Path(str(base)+'.'+suffix).write_text(value)
with pathlib.Path(str(base)+'.stdout.txt').open('xb') as out,pathlib.Path(str(base)+'.stderr.txt').open('xb') as err:
    r=subprocess.run(argv,stdin=subprocess.DEVNULL,stdout=out,stderr=err)
pathlib.Path(str(base)+'.exitcode').write_text(str(r.returncode)+'\n')
pathlib.Path(str(base)+'.finished.txt').write_text(datetime.datetime.now(datetime.timezone.utc).isoformat()+'\n')
print(label,'exitcode',r.returncode)
for suffix in ['stdout.txt','stderr.txt']:print(pathlib.Path(str(base)+'.'+suffix).read_text(errors='replace')[:20000])
sys.exit(r.returncode)
