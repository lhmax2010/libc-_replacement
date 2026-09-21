"""Run a diagnostic command in the existing guest without modifying its RPM/tools."""
import json,pathlib,subprocess,sys
base=pathlib.Path('progress/ARM_LZMA_0921')
args=json.loads(pathlib.Path('progress/ARM_REPRO_0920/original-argv.json').read_text())
prefix=args[args.index('bwrap'):args.index('--')]
work=pathlib.Path('tmp/ARM_LZMA_0921').resolve();work.mkdir(parents=True,exist_ok=True)
cmd=prefix+['--bind',str(work),'/tmp/lzma-diag','--']+sys.argv[1:]
print(json.dumps({'command':cmd},ensure_ascii=False),flush=True)
r=subprocess.run(cmd)
print(json.dumps({'exitcode':r.returncode}),flush=True)
sys.exit(r.returncode)
