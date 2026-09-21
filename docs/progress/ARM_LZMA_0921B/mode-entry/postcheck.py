import collections,hashlib,json,pathlib
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
base=root/'progress/ARM_LZMA_MODE_0921'
out=base/'cells/gdb-mode-entry'
rows=[json.loads(s) for s in (out/'gdb-observations.jsonl').read_text().splitlines()]
entries=[r for r in rows if r['event']=='lzopen_entry']
checks=[]
for e in entries:
    raw=bytes.fromhex(e['raw_hex'])
    assert len(raw)==16 and e['close_count']==18
    first=raw.find(b'\0')
    second=raw.find(b'\0',first+1) if first>=0 else -1
    tail=raw[first+1:second if second>=0 else 16] if first>=0 else b''
    assert first==e['first_nul'] and second==e['next_nul']
    assert tail.hex(' ')==e['after_first_nul_hex']
    assert (114 in tail)==e['contains_r_in_segment']
    checks.append(dict(mode_pointer=e['mode_pointer'],raw_hex=e['raw_hex'],raw_printable=e['raw_printable'],first_nul=first,next_nul=second,after_first_nul_hex=tail.hex(' '),after_first_nul_printable=e['after_first_nul_printable'],contains_r=114 in tail,segment_complete=second>=0))
inputs=json.loads((base/'INPUT_SHA256.json').read_text())
for p,h in inputs.items():
    assert hashlib.sha256((root/p).read_bytes()).hexdigest()==h, p
lines=(out/'build.log').read_text(errors='replace').splitlines()
old=(root/'progress/ARM_LZMA_0921B/cells/gdb-preserved/build.log').read_text(errors='replace').splitlines()
rc=int((out/'exitcode').read_text())
summary=dict(rpmbuild_exitcode=rc,gdb_exitcode=int((out/'gdb.exitcode').read_text()),counts=dict(collections.Counter(r['event'] for r in rows)),entry_checks=checks,wrote=sum(s.startswith('Wrote:') for s in lines),last_wrote=[s for s in lines if s.startswith('Wrote:')][-1:],archive_errors=[dict(line=i+1,text=s) for i,s in enumerate(lines) if 'create archive failed' in s],original_line_7559_7560_identical=lines[7558:7560]==old[7558:7560],inputs_unchanged=True)
for name in ('gdb_observer.py','guard.py','capture_failure.py'):
    (out/('executed-'+name)).write_bytes((base/name).read_bytes())
summary['files']={}
for p in [out/'build.log',out/'gdb.log',out/'gdb-observations.jsonl',out/'host-failed.strace',base/'gdb_observer.py',base/'guard.py']:
    h=hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda:f.read(1024*1024),b''):h.update(chunk)
    summary['files'][str(p.relative_to(base))]=dict(bytes=p.stat().st_size,sha256=h.hexdigest())
(base/'RESULT_CHECK.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(summary,ensure_ascii=False,indent=2))
