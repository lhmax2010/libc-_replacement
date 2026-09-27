from common import *
# Low I/O priority is retained; allow a read-only hash audit to wait for I/O.
r=run('final_audit_long',['python3',OUT/'code/audit.py'],timeout=1200,check=False)
print(r['stdout']);print(r['stderr']);raise SystemExit(r['exit'])
