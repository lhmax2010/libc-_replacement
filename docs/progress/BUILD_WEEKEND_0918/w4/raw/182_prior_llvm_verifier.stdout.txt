"""Verify actual LLVM RPMs, including the corrected compression thread flag."""
import json
import runpy
import shlex
import subprocess
import sys
from pathlib import Path

assert sys.argv[1:] == ['armv7l']
out=Path('progress/BUILD_STATIC_0917B/resume_0917')
runpy.run_path(str(out/'verify_llvm_rpms.executed.py'),run_name='__main__')
result=json.loads((out/'verify_armv7l/result.json').read_text())
rows=[]
for record in result['rpms']:
    cmd=['rpm','-qp','--qf','%{NAME}\t%{PAYLOADCOMPRESSOR}\t%{PAYLOADFLAGS}',record['path']]
    r=subprocess.run(cmd,capture_output=True,text=True)
    rows.append({'command':shlex.join(cmd),'exitcode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
    (out/'verify_armv7l/compression_flags.json').write_text(json.dumps(rows,indent=2))
    assert r.returncode==0 and r.stdout.split('\t')[1:]==['xz','5T1']
print('PASS: all final RPM payload headers record xz/5T1; no stopped-attempt RPM used.')
