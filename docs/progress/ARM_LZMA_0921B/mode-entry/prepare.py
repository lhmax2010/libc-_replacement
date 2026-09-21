import hashlib,json,pathlib,shlex,subprocess
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
base=root/'progress/ARM_LZMA_MODE_0921'
old=root/'progress/ARM_LZMA_0921B/cell_commands/gdb-preserved.json'
argv=json.loads(old.read_text())
new=list(argv)
index=new.index('-o')+1
new[index]=str(base/'cells/gdb-mode-entry/host-failed.strace')
assert [(i,a,b) for i,(a,b) in enumerate(zip(argv,new)) if a!=b]==[(index,argv[index],new[index])]
(base/'cell_commands').mkdir(exist_ok=True)
(base/'cell_commands/gdb-mode-entry.json').write_text(json.dumps(new,indent=2)+'\n')
(base/'ARGV_DIFF.json').write_text(json.dumps(dict(previous=str(old),changes=[dict(index=index,old=argv[index],new=new[index])],rpm_argv_identical=True,strace_filter_identical=True),indent=2)+'\n')
(base/'COMMAND.txt').write_text(shlex.join(new)+'\n')
paths=['tmp/WEEKEND_0918/specs/llvm.spec']
r='tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/'
paths += [r+x for x in ('usr/bin/rpmbuild','usr/lib/librpmio.so.3.1.0','usr/lib/librpm.so.3.1.0','usr/lib/librpmbuild.so.3.1.0','usr/lib/liblzma.so.5','usr/bin/qemu-arm-static')]
sha={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths}
(base/'INPUT_SHA256.json').write_text(json.dumps(sha,indent=2)+'\n')
assert sha[paths[0]]=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
for name in ('gdb_observer.py','guard.py','capture_failure.py','preserve_previous.py'):
    compile((base/name).read_text(),str(base/name),'exec')
print(json.dumps(dict(argv_changes=1,only_trace_output_path_changed=True,sha256=sha,python_syntax='PASS'),indent=2))
