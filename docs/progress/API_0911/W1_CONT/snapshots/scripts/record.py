#!/usr/bin/env python3
"""保存命令原文、完整输出、退出码与 UTC 时间；不覆盖旧记录。"""
import datetime, json, pathlib, shlex, subprocess, sys
stem = pathlib.Path(sys.argv[1])
cmd = sys.argv[2:]
stem.parent.mkdir(parents=True, exist_ok=True)
def output(suffix):
    return pathlib.Path(str(stem) + suffix)
start = datetime.datetime.now(datetime.timezone.utc).isoformat()
with output('.command.txt').open('x') as f:
    f.write('cwd: ' + str(pathlib.Path.cwd()) + '\ncommand: ' + shlex.join(cmd) + '\n')
with output('.stdout').open('xb') as out, output('.stderr').open('xb') as err:
    result = subprocess.run(cmd, stdout=out, stderr=err)
output('.exitcode').write_text(str(result.returncode) + '\n')
output('.time.json').write_text(json.dumps({'start':start, 'end':datetime.datetime.now(datetime.timezone.utc).isoformat()}, indent=2)+'\n')
print(str(stem) + ': exit=' + str(result.returncode))
sys.exit(result.returncode if result.returncode >= 0 else 128-result.returncode)
