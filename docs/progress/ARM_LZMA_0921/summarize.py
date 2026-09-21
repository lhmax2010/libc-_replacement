"""Read completed run evidence. Never launches a build or changes its inputs."""
import collections,csv,datetime,hashlib,json,pathlib,re,shutil,subprocess
b=pathlib.Path('progress/ARM_LZMA_0921');w=pathlib.Path('tmp/ARM_LZMA_0921');cell=b/'cells/preload-original'
events=[json.loads(s) for s in (cell/'events.jsonl').read_text().splitlines()]
start=next(e for e in events if e['event']=='start')
end=next(e for e in reversed(events) if e['event']=='finished')
log=(cell/'build.log').read_text(errors='replace').splitlines()
tap=(w/'rpmbuild-lzma.calls.tsv').read_text().splitlines()
rows=[{k:(v if k=='stream' else int(v)) for k,v in r.items()} for r in csv.DictReader(tap[1:],delimiter='\t')]
seq=[r['seq'] for r in rows]
ns=[r['monotonic_sec']*1000000000+r['monotonic_nsec'] for r in rows]
integrity={'rows':len(rows),'sequences_exactly_1_to_N':seq==list(range(1,len(rows)+1)),'monotonic_nondecreasing':ns==sorted(ns)}
errors=[];last_ok=None;last_normal=None;previous=None
for r in rows:
    if r['ret'] not in (0,1) or (r['action']==0 and r['ret']==1):
        entry={'call':r,'previous_record':previous,'previous_LZMA_OK':last_ok,'previous_success_OK_or_STREAM_END':last_normal}
        if last_normal:
            entry['ns_since_previous_success_return']=((r['monotonic_sec']-last_normal['monotonic_sec'])*1000000000+r['monotonic_nsec']-last_normal['monotonic_nsec'])
        errors.append(entry)
    if r['ret']==0:last_ok=r
    if r['ret']==0 or (r['ret']==1 and r['action']!=0):last_normal=r
    previous=r
wrote=[{'line':i,'name':pathlib.Path(s[7:]).name,'text':s} for i,s in enumerate(log,1) if s.startswith('Wrote: ')]
error_lines=[{'line':i,'text':s} for i,s in enumerate(log,1) if 'cpio: write failed' in s or s.startswith('error:')]
target_error=any('cpio: write failed - Function not implemented' in x['text'] for x in error_lines)
if end['exitcode']==0:
    decision='STOP_INSTRUMENTED_FAILURE_DISAPPEARED'
elif not integrity['sequences_exactly_1_to_N'] or not integrity['monotonic_nondecreasing'] or not rows:
    decision='INSTRUMENT_EVIDENCE_INCOMPLETE'
elif target_error and any(x['call']['ret']==5 for x in errors):
    decision='TARGET_FAILURE_AND_LZMA_MEM_ERROR_OBSERVED'
elif target_error and errors:
    decision='TARGET_FAILURE_AND_OTHER_LZMA_RETURN_OBSERVED'
elif target_error:
    decision='TARGET_FAILURE_WITHOUT_LZMA_ERROR_RETURN_SHORT_FWRITE_NOT_YET_OBSERVED'
else:
    decision='OTHER_FAILURE_SHAPE'

def maps_summary(path):
    data=path.read_text();entries=[]
    for line in data.splitlines():
        m=re.match(r'([0-9a-f]+)-([0-9a-f]+)\s+(\S+)',line)
        if m:entries.append((int(m[1],16),int(m[2],16),m[3]))
    assert all(0<=a<c<=2**32 for a,c,_ in entries),'Not a guest 32-bit maps view'
    intervals=sorted((a,c) for a,c,_ in entries);cursor=0;holes=[]
    for a,c in intervals:
        if a>cursor:holes.append((cursor,a))
        cursor=max(cursor,c)
    if cursor<2**32:holes.append((cursor,2**32))
    holes.sort(key=lambda x:x[1]-x[0],reverse=True)
    return {'entries':len(entries),'mapped_bytes_sum':sum(c-a for a,c,_ in entries),'largest_hole':{'start':hex(holes[0][0]),'end':hex(holes[0][1]),'bytes':holes[0][1]-holes[0][0]} if holes else None,'top_holes':[{'start':hex(a),'end':hex(c),'bytes':c-a} for a,c in holes[:5]],'domain':'[0, 2^32); unmapped gaps only, not proof that an allocation of this size can succeed'}

va={}
for label in ('initial','first-non-ok-non-end'):
    maps=w/f'rpmbuild-lzma.{label}.maps';status=w/f'rpmbuild-lzma.{label}.status'
    if not maps.exists() or not status.exists():
        va[label]={'status':'NOT_OBSERVED'};continue
    metrics={}
    for line in status.read_text().splitlines():
        if line.startswith(('VmSize:','VmPeak:','VmRSS:')):
            k,v=line.split(':',1);metrics[k]=v.strip()
    va[label]={'status':'OBSERVED','proc_status_raw_metrics':metrics,'guest_maps':maps_summary(maps),'status_counter_caveat':'QEMU process /proc status metrics must not be equated with occupied guest virtual address bytes'}

inventory=[]
for p in sorted(pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l').glob('*.rpm')):
    cmd=['rpm','-K','--nosignature',str(p)];r=subprocess.run(cmd,capture_output=True,text=True);h=hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda:f.read(1024*1024),b''):h.update(chunk)
    inventory.append({'path':str(p),'wrote_in_this_run':any(x['name']==p.name for x in wrote),'bytes':p.stat().st_size,'sha256':h.hexdigest(),'command':cmd,'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})

raw=b/'instrument-output';raw.mkdir(exist_ok=True)
copies=[]
for p in sorted(w.glob('rpmbuild-lzma.*')):
    q=raw/p.name;assert not q.exists();shutil.copy2(p,q)
    assert hashlib.sha256(p.read_bytes()).digest()==hashlib.sha256(q.read_bytes()).digest()
    copies.append({'source':str(p),'copy':str(q),'bytes':q.stat().st_size,'sha256':hashlib.sha256(q.read_bytes()).hexdigest()})
result={'decision':decision,'start':start['time'],'finish':end['time'],'seconds':(datetime.datetime.fromisoformat(end['time'])-datetime.datetime.fromisoformat(start['time'])).total_seconds(),'exitcode':end['exitcode'],'wrote':wrote,'error_lines':error_lines,'instrument_header':tap[0],'integrity':integrity,'return_counts':dict(collections.Counter(r['ret'] for r in rows)),'error_returns':errors,'last_call':rows[-1] if rows else None,'va':va,'file_access_sequence':'NOT_OBSERVED_BY_USER_CONFIRMED_FILTER','rpm_inventory':inventory,'copied_evidence':copies}
(b/'RESULT.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ('decision','start','finish','exitcode','integrity','return_counts','error_returns','va')},ensure_ascii=False,indent=2))
print('Wrote',len(wrote),'RPM files',len(inventory),'rpm_digest_failures',sum(x['exitcode']!=0 for x in inventory))
