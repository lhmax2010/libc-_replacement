import collections,json,pathlib,re
b=pathlib.Path('progress/ALIGN_0920');source=pathlib.Path('tmp/ARM_RPM_DIAG/guest-rpmbuild.strace')
counts=collections.Counter();examples={};matches=[];markers=[];prev=None;pairs=0
for n,line in enumerate(source.open(errors='replace'),1):
    text=line.rstrip('\n');calls=re.findall(r'\b(\d+) ([A-Za-z_][A-Za-z_0-9]*)\(',text)
    if 'openat(' in text and '/RPMS/armv7l/' in text and 'O_CREAT' in text:markers.append({'line':n,'text':text})
    statx='statx(' in text and 'errno=38 (Function not implemented)' in text
    if statx:
        key='statx_enosys_single_call_line' if len(calls)==1 and calls[0][1]=='statx' and re.fullmatch(r'\d+ statx\(.*\) = -1 errno=38 \(Function not implemented\)',text) else 'statx_enosys_interleaved_or_partial_line'
        counts[key]+=1;examples.setdefault(key,{'first':{'line':n,'text':text}})['last']={'line':n,'text':text}
        matches.append((n,key,text));prev=(n,calls[0][0] if len(calls)==1 else None)
    elif prev:
        if n==prev[0]+1 and prev[1] and len(calls)==1 and calls[0]==(prev[1],'fstatat64') and re.fullmatch(re.escape(prev[1])+r' fstatat64\(.*\) = 0',text):pairs+=1
        prev=None
    if 'set_robust_list(' in text and 'errno=38 ' in text:counts['set_robust_list_enosys_matching_lines']+=1
first=min(m['line'] for m in markers)
nineteen=next(m['line'] for m in markers if '/lldb-debuginfo-' in m['text'])
twenty=next(m['line'] for m in markers if '/lldb-devel-debuginfo-' in m['text'])
windows={'before_first_rpm':(0,first),'rpm_1_to_18':(first,nineteen),'rpm_19_interval':(nineteen,twenty),'after_rpm_19_interval':(twenty,n+1)}
summary={name:dict(collections.Counter(k for i,k,t in matches if lo<=i<hi)) for name,(lo,hi) in windows.items()}
with (b/'statx-enosys-locations.tsv').open('w') as f:
    f.write('line\tclassification\tactual_text\n')
    for i,k,t in matches:f.write(f'{i}\t{k}\t{t}\n')
result={'source':str(source),'total_lines':n,'counts':dict(counts),'immediately_followed_by_same_pid_fstatat64_success':pairs,'examples':examples,'rpm_open_markers':markers,'window_line_bounds':windows,'window_counts':summary,'historical_packaging_failure_in_this_run':False}
(b/'trace-counts.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
