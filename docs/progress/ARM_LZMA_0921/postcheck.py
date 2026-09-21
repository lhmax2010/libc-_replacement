"""Only inspect saved trace, call log and RPM headers; no reproduction."""
import collections,csv,hashlib,json,pathlib,re,subprocess
b=pathlib.Path('progress/ARM_LZMA_0921');r=json.loads((b/'RESULT.json').read_text())
with (b/'instrument-output/rpmbuild-lzma.calls.tsv').open() as f:
 next(f);calls=list(csv.DictReader(f,delimiter='\t'))
ends=[x for x in calls if x['ret']=='1']
counts=collections.Counter();samples=[]
trace=b/'host-instrumented.strace'
with trace.open(errors='replace') as f:
 for number,line in enumerate(f,1):
  m=re.search(r'\b(write|pwrite64)\(',line)
  if m and re.search(r'= -1\b',line):
   counts[m[1]]+=1
   if len(samples)<30:samples.append({'line':number,'text':line.rstrip()})
  if re.search(r'\b(write|pwrite64) resumed>',line) and re.search(r'= -1\b',line):
   counts['resumed_write_or_pwrite64']+=1
   if len(samples)<30:samples.append({'line':number,'text':line.rstrip()})
archive=[]
for ix,item in enumerate(r['wrote']):
 path=pathlib.Path('tmp/WEEKEND_0918/rpm-arm-llvm/RPMS/armv7l')/item['name']
 cmd=['rpm','-qp','--qf','%{NAME}\t%{ARCHIVESIZE}\n',str(path)]
 c=subprocess.run(cmd,capture_output=True,text=True)
 record={'command':cmd,'exitcode':c.returncode,'stdout':c.stdout,'stderr':c.stderr,'wrote':item}
 if ix<len(ends):
  record['stream_end_in_same_order']=ends[ix]
  if c.returncode==0:
   record['archive_size_equals_stream_total_in']=int(c.stdout.strip().split('\t')[1])==int(ends[ix]['total_in'])
 archive.append(record)
trace_hash=hashlib.sha256()
with trace.open('rb') as f:
 for chunk in iter(lambda:f.read(1024*1024),b''):trace_hash.update(chunk)
out={'failed_host_write_counts':dict(counts),'failed_host_write_samples':samples,'stream_ends':len(ends),'archive_comparison':archive,
 'last_two_calls':calls[-2:], 'errno_pairs':dict(collections.Counter(x['errno_before']+'/'+x['errno_after'] for x in calls)),
 'trace_sha256':trace_hash.hexdigest(),
 'limitations':'Host failure-only strace cannot prove absence of guest libc fwrite short returns; only negative syscall results in its selected scope are counted.'}
(b/'POSTCHECK.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:out[k] for k in ('failed_host_write_counts','failed_host_write_samples','stream_ends','last_two_calls','errno_pairs','trace_sha256')},indent=2))
print('Archive-size matches:',sum(x.get('archive_size_equals_stream_total_in',False) for x in archive),'/',len(archive))
