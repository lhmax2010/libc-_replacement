import hashlib,json,pathlib,subprocess,sys
b=pathlib.Path('progress/ARM_LZMA_0921');work=pathlib.Path('tmp/ARM_LZMA_0921');rows=[]
tag=sys.argv[1] if len(sys.argv)>1 else ''
assert tag in ('','monotonic-')
for mode in ('normal','memerror','progerror'):
    observations=[]
    for instrumented in (False,True):
        name=tag+mode+('-tap' if instrumented else '-plain')
        tail=['/tmp/lzma-diag/tap_calibration',mode,'/tmp/lzma-diag/'+name+'.xz']
        if instrumented:tail=['/usr/bin/env','LD_PRELOAD=/tmp/lzma-diag/lzma_tap.so','LZMA_TAP_PREFIX=/tmp/lzma-diag/'+name]+tail
        cmd=['python3',str(b/'guest.py')]+tail
        r=subprocess.run(cmd,capture_output=True,text=True)
        (b/(name+'.command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
        (b/(name+'.stdout')).write_text(r.stdout);(b/(name+'.stderr')).write_text(r.stderr)
        summary=[s for s in r.stdout.splitlines() if s.startswith(('init=','call='))]
        p=work/(name+'.xz');sha=hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
        row={'name':name,'exitcode':r.returncode,'command':cmd,'summary':summary,'payload_sha256':sha}
        rows.append(row);observations.append(row)
        print(json.dumps(row),flush=True)
        if r.returncode:break
    if len(observations)!=2 or any(x['exitcode'] for x in observations):break
    assert observations[0]['summary']==observations[1]['summary']
    assert observations[0]['payload_sha256']==observations[1]['payload_sha256']
(b/('CALIBRATION_MONOTONIC.json' if tag else 'CALIBRATION.json')).write_text(json.dumps(rows,indent=2)+'\n')
assert len(rows)==6 and all(x['exitcode']==0 for x in rows)
print('CALIBRATION_PASSED: returns/errno/stream counters and payload SHA match in all three controlled cases')
