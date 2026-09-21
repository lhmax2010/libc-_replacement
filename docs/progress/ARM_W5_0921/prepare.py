import hashlib,json,pathlib,shlex,subprocess
root=pathlib.Path('/home/toolchain/development/libc++_replacement');base=root/'progress/ARM_W5_0921'
old=json.loads((root/'progress/ARM_REPRO_0920/original-argv.json').read_text())
argv=old.copy();i=argv.index('_binary_payload w5T1.xzdio');argv[i]='_binary_payload w5.xzdio'
diff=[dict(index=n,old=a,new=b) for n,(a,b) in enumerate(zip(old,argv)) if a!=b]
assert len(diff)==1 and diff[0]['index']==i
(base/'cell_commands').mkdir(exist_ok=True)
(base/'cell_commands/w5-original.json').write_text(json.dumps(argv,indent=2)+'\n')
(base/'ARGV_DIFF.json').write_text(json.dumps(dict(changes=diff,other_argv_byte_identical=True),indent=2)+'\n')
(base/'COMMAND.txt').write_text(shlex.join(argv)+'\n')
# Exact same bwrap environment, rpm macro source and command-line defines.
prefix=argv[argv.index('bwrap'):argv.index('rpmbuild')]
args=argv[argv.index('rpmbuild')+1:]
assert args[-4:]==['--noprep','--noclean','-bb','/tmp/task/specs/llvm.spec']
cmd=prefix+['rpm']+args[:-4]+['--eval','%{_binary_payload}']
r=subprocess.run(cmd,capture_output=True,text=True)
(base/'PAYLOAD_PRECHECK.json').write_text(json.dumps(dict(command=cmd,command_shell=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2)+'\n')
assert r.returncode==0 and r.stdout.strip()=='w5.xzdio'
paths=['tmp/WEEKEND_0918/specs/llvm.spec']
br='tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/'
paths += [br+p for p in ('usr/bin/rpmbuild','usr/bin/rpm','usr/lib/librpmio.so.3.1.0','usr/lib/librpm.so.3.1.0','usr/lib/librpmbuild.so.3.1.0','usr/lib/liblzma.so.5','usr/bin/qemu-arm-static')]
sha={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths}
(base/'INPUT_SHA256.json').write_text(json.dumps(sha,indent=2)+'\n')
assert sha[paths[0]]=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
for name in ('guard.py','thread_snapshot.py','capture_failure.py'):compile((base/name).read_text(),name,'exec')
print(json.dumps(dict(argv_changes=diff,payload=r.stdout.strip(),payload_command=shlex.join(cmd),syntax='PASS'),indent=2))
