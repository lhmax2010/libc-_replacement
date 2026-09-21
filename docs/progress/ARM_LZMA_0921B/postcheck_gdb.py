"""Read-only integrity checks after the observed failure; no additional trial."""
import datetime,hashlib,json,pathlib
base=pathlib.Path('progress/ARM_LZMA_0921B');cell=base/'cells/gdb-preserved'
root=pathlib.Path('tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0')
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
identities=json.loads((base/'SYMBOL_IDENTITY.json').read_text())
verified=[]
for row in identities:
    p=pathlib.Path(row['elf']);actual=sha(p);assert actual==row['elf_sha256']
    verified.append(dict(path=str(p),sha256=actual,unchanged=True))
for p,expected in [
    (root/'usr/bin/rpmbuild','1b3dbf19fed47cd7d30f6ed68ebf8a050f6b09204f086180d04bc43f91976578'),
    (root/'usr/lib/liblzma.so.5.6.3','d29fa8718e3a5ab66836df7553328e3241b7221aeaf2f8b151b97ed4423bc10e'),
    (pathlib.Path('tmp/WEEKEND_0918/specs/llvm.spec'),'cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68')]:
    if not p.exists() and p.name.startswith('liblzma'):p=root/'usr/lib/liblzma.so.5'
    actual=sha(p);assert actual==expected,(str(p),actual)
    verified.append(dict(path=str(p),sha256=actual,unchanged=True))
obs=[json.loads(s) for s in (cell/'gdb-observations.jsonl').read_text().splitlines()]
closes=[r for r in obs if r['event']=='lzclose_counter']
assert [r['close_count'] for r in closes]==list(range(1,20))
assert all(r['encoding']==1 for r in closes[:18]) and closes[18]['encoding']==0
points=[r for r in obs if r['event'] in ('lzopen_return','cpio_header_entry','lzwrite_entry')]
assert len(points)==3 and all(r['close_count']==18 for r in points)
assert points[0]['pointer']==points[1]['fp']==points[2]['pointer']=='0x9a4958'
assert points[0]['encoding']==points[2]['encoding']==0
assert points[1]['ioname']=='xzdio' and points[1]['name']=='xz'
assert not any(r['event']=='observer_error' for r in obs)
events=[json.loads(s) for s in (cell/'events.jsonl').read_text().splitlines()]
start=next(r for r in events if r['event']=='start')
end=next(r for r in events if r['event']=='finished')
dbg=next(r for r in events if r['event']=='gdb_started')
assert start['script_sha256']==sha(base/'guard.py')
assert dbg['observer_sha256']==sha(base/'gdb_observer.py')
old=pathlib.Path('progress/ARM_LZMA_0921/cells/preload-original/build.log').read_text().splitlines()
new=(cell/'build.log').read_text().splitlines()
extract=lambda lines:[{'line':n,'text':s} for n,s in enumerate(lines,1) if 'error:' in s or 'cpio:' in s]
assert old[7558:7560]==new[7558:7560]
result=dict(verified_inputs=verified,original_error_pair_identical=True,
    old_errors=extract(old),new_errors=extract(new),
    all_observations_gate_18=True,three_pointer_difference_bytes=0,
    valid_run_seconds=(datetime.datetime.fromisoformat(end['time'])-datetime.datetime.fromisoformat(start['time'])).total_seconds(),
    rpmbuild_exitcode=end['exitcode'],gdb_exitcode=int((cell/'gdb.exitcode').read_text()),
    no_extra_return_breakpoints_activated=not any(r['event']=='branch_b_return_breakpoints_enabled' for r in obs),
    files={str(p.relative_to(base)):dict(bytes=p.stat().st_size,sha256=sha(p)) for p in [cell/'build.log',cell/'gdb.log',cell/'gdb-observations.jsonl',cell/'host-failed.strace',cell/'failure-file-metadata.json',base/'gdb_observer.py',base/'guard.py']})
(base/'POSTCHECK_GDB.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False,indent=2))
