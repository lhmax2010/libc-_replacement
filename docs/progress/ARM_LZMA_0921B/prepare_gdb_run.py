"""Record exact argv changes; preserve the original RPM arguments and host trace."""
import difflib,hashlib,json,pathlib,shlex
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
base=root/'progress/ARM_LZMA_0921B';out=base/'cells/gdb-preserved'
original=json.loads((root/'progress/ARM_REPRO_0920/original-argv.json').read_text())
cmd=original.copy();cmd[cmd.index('-o')+1]=str(out/'host-failed.strace')
entry=cmd.index('--')+1
assert cmd[entry]=='rpmbuild'
cmd[entry:entry+1]=['/usr/bin/qemu-arm-static','-g','49192','/bin/rpmbuild']
assert cmd[entry+4:]==original[original.index('--')+2:]
assert cmd[:cmd.index('-o')+1]==original[:original.index('-o')+1]
assert '-vv' not in cmd and not any('PRELOAD' in x for x in cmd)
(base/'cell_commands').mkdir(exist_ok=True)
(base/'cell_commands/gdb-preserved.json').write_text(json.dumps(cmd,indent=2)+'\n')
(base/'ARGV_DIFF_CORRECTED.txt').write_text(''.join(difflib.unified_diff([x+'\n' for x in original],[x+'\n' for x in cmd],fromfile='original argv',tofile='gdb argv (corrected guest argv0)')))
(base/'COMMAND_CORRECTED.txt').write_text(shlex.join(cmd)+'\n')
(base/'deadlines.json').write_text(json.dumps({'W2':'2026-09-21T20:42:54+08:00'},indent=2)+'\n')
files=[root/'tmp/WEEKEND_0918/specs/llvm.spec',root/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/usr/bin/qemu-arm']
rows=[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in files]
assert rows[0]['sha256']=='cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68'
(base/'RUN_INPUT_IDENTITY.json').write_text(json.dumps(rows,indent=2)+'\n')
print((base/'ARGV_DIFF_CORRECTED.txt').read_text());print(json.dumps(rows,indent=2))
