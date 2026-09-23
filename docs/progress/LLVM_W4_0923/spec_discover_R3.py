"""Inventory the explicitly requested execution-record families, without reading source trees."""
import collections,hashlib,json,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/spec-audit_R3';E.mkdir(exist_ok=True)
cmd=['rg','--files','docs/progress','-g','*argv*.json','-g','**/cell_commands/*.json','-g','*command*.json','-g','*build.log']
r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0
paths=[p for p in r.stdout.splitlines() if not p.startswith('docs/progress/LLVM_W4_0923/')]
extra_cmd=['rg','-l','-g','*.stdout*','-g','*.log','-g','!LLVM_W4_0923/**','-e',r'processing recipe |(?:^|\s)(?:exec )?rpmbuild |gbs .* build ','docs/progress']
extra=subprocess.run(extra_cmd,capture_output=True,text=True);assert extra.returncode in (0,1)
supplement=[p for p in extra.stdout.splitlines() if not p.startswith('docs/progress/LLVM_W4_0923/')]
command_cmd=['rg','--files','docs/progress','-g','*.command.txt']
command_result=subprocess.run(command_cmd,capture_output=True,text=True);assert command_result.returncode==0
supplement += [p for p in command_result.stdout.splitlines() if not p.startswith('docs/progress/LLVM_W4_0923/')]
(E/'supplementary-discovery_R3.json').write_text(json.dumps(dict(commands=[dict(argv=extra_cmd,command=shlex.join(extra_cmd),exitcode=extra.returncode),dict(argv=command_cmd,command=shlex.join(command_cmd),exitcode=command_result.returncode)],reason='Older rounds store actual build execution in raw/*.stdout.txt or *.command.txt, not build.log.',files=supplement),indent=2)+'\n')
paths=sorted(set(paths+supplement))
(E/'discovery-command_R3.json').write_text(json.dumps(dict(argv=cmd,command=shlex.join(cmd),exitcode=r.returncode,excluded='own audit records, not original builds'),indent=2)+'\n')
(E/'discovered_files_R3.json').write_text(json.dumps(paths,indent=2)+'\n')
counts=collections.Counter(p.split('/')[2] for p in paths)
stats=dict(files=len(paths),bytes=sum(Path(p).stat().st_size for p in paths),rounds=dict(sorted(counts.items())),log_files=sum(p.endswith('build.log') for p in paths))
(E/'discovery_summary_R3.json').write_text(json.dumps(stats,indent=2)+'\n')
print(json.dumps(stats,indent=2))
for p in [p for p in paths if p.endswith('.json')][:12]:
 try:
  obj=json.loads(Path(p).read_text());print(p,type(obj).__name__,str(obj)[:320])
 except (ValueError,OSError) as ex: print(p,type(ex).__name__)
