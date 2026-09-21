"""Read existing instrumentation evidence only; never launch guest code."""
import collections,csv,hashlib,io,json,pathlib,re
b=pathlib.Path('progress/ARM_LZMA_0921B');old=pathlib.Path('progress/ARM_LZMA_0921')
path=old/'instrument-output/rpmbuild-lzma.calls.tsv';data=path.read_bytes()
lines=data.splitlines(keepends=True);header=lines[1].decode().rstrip('\n').split('\t')
rows=list(csv.DictReader(io.StringIO(b''.join(lines[1:]).decode()),delimiter='\t'))
seq=[int(x['seq']) for x in rows];ns=[int(x['monotonic_sec'])*10**9+int(x['monotonic_nsec']) for x in rows]
bad=[i for i,line in enumerate(lines[2:],3) if not line.endswith(b'\n') or len(line.rstrip(b'\n').split(b'\t'))!=14]
snap=json.loads((old/'rpmbuild-host-snapshot-125730.json').read_text())
tap_pid=int(re.search(rb'\bpid=(\d+)',lines[0])[1])
post=json.loads((old/'POSTCHECK.json').read_text())
trace=old/'host-instrumented.strace';trace_counts=collections.Counter();exits=[];last_main=[];possible_log_failures=[]
with trace.open(errors='replace') as f:
 for number,line in enumerate(f,1):
  if re.search(r'\b(write|pwrite64)(?:\(| resumed>)',line) and re.search(r'= -1\b',line):
   trace_counts['negative_write_or_pwrite64']+=1
  if str(tap_pid)==line.split(' ',1)[0]:
   last_main.append({'line':number,'text':line.rstrip()});last_main=last_main[-12:]
  if re.search(r'^'+str(tap_pid)+r'\s+\+\+\+ exited',line):exits.append({'line':number,'text':line.rstrip()})
  if 'rpmbuild-lzma.calls.tsv' in line:
   possible_log_failures.append({'line':number,'text':line.rstrip()})
r={'input':str(path),'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest(),'data_rows':len(rows),'columns':len(header),
 'ends_with_LF':data.endswith(b'\n'),'malformed_line_numbers':bad,'sequence_exactly_1_to_N':seq==list(range(1,len(rows)+1)),
 'monotonic_nondecreasing':ns==sorted(ns),'largest_data_line_bytes':max(map(len,lines[2:])),'last_line':lines[-1].decode(),
 'last_row':rows[-1],'return_counts':dict(collections.Counter(x['ret'] for x in rows)),
 'stream_end_actions':dict(collections.Counter(x['action'] for x in rows if x['ret']=='1')),
 'distinct_stream_addresses':sorted({x['stream'] for x in rows}),
 'tap_pid':tap_pid,'host_snapshot_pid':snap['pid'],'pid_matches':tap_pid==snap['pid'],
 'host_snapshot_command':snap['command'],'main_exit_records':exits,'last_main_trace_records':last_main,
 'negative_host_write_count':trace_counts['negative_write_or_pwrite64'],'trace_log_path_records':possible_log_failures,
 'archive_size_matches':sum(x['archive_size_equals_stream_total_in'] for x in post['archive_comparison']),
 'coverage_limitations':['return-side records only','write_all silently returns for write<=0 except EINTR','no lzma init/end hooks','preload environment removed before exec of children','per-call PID/TID and log completion marker absent','failure-only host strace does not prove success of every log write']}
assert r['pid_matches'] and not bad and r['sequence_exactly_1_to_N'] and r['monotonic_nondecreasing'] and r['ends_with_LF']
(b/'LOG_INTEGRITY.json').write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(r,ensure_ascii=False,indent=2))
