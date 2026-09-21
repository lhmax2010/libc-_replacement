"""Cross-check the executed static candidate link command against the saved CMake command."""
import hashlib,json,sys
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; arch=sys.argv[1]
assert arch in ('armv7l','aarch64')
cell=out/f'cells/bpf-{arch}-libcxx'
assert (cell/'exitcode').read_text().strip()=='0'
f=p/f'tmp/BPF_W1_0921/rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2/build-static/src/CMakeFiles/bpftrace.dir/link.txt'
data=f.read_bytes(); command=data.decode().strip()
log=(cell/'build.log').read_text(errors='replace')
matches=[line for line in log.splitlines() if command in line]
checks=dict(executed_command_exact_match=bool(matches),shared_libgcc_option='-shared-libgcc' in command,static_libgcc_option_absent='-static-libgcc' not in command,explicit_libgcc_eh_absent='libgcc_eh.a' not in command)
(out/f'static-link-{arch}.txt').write_bytes(data)
result=dict(arch=arch,status='PASS' if all(checks.values()) else 'NOT_CLOSED',source=str(f),sha256=hashlib.sha256(data).hexdigest(),actual_log_matches=matches,checks=checks,scope='driver invocation and matching build log; not a new cancellation experiment')
(out/f'static-link-{arch}.json').write_text(json.dumps(result,indent=2))
print(arch,result['status'],json.dumps(checks))
raise SystemExit(0 if result['status']=='PASS' else 1)
