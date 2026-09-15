#!/usr/bin/env python3
"""在空构建目录复现反向机制；依赖清单中的真实 SDK/RPM 产物必须先存在。"""
import argparse,json,os,pathlib,shutil,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--workspace',required=True);p.add_argument('--platform-workspace',required=True);p.add_argument('--output',required=True);a=p.parse_args()
root=pathlib.Path(a.workspace).resolve();work=pathlib.Path(a.output).resolve();allowed=root/'tmp/P12'
assert allowed in work.parents,'Only a fresh descendant of workspace/tmp/P12 is allowed'
work.mkdir(parents=True,exist_ok=False)
source=pathlib.Path(__file__).resolve().parent
for f in source.glob('*.py'):shutil.copyfile(f,work/f.name)
for directory in ['src','upstream','testconfig']:shutil.copytree(source/directory,work/directory)
env=dict(os.environ,P12_WORKSPACE_ROOT=str(root),P12_PLATFORM_WORKSPACE=a.platform_workspace,P12_OUT_DIR=str(work/'evidence'))
rec=root/'docs/progress/API_0911/record.py'
def run(tag,args,cwd=work):
 cmd=[sys.executable,str(rec),str(work/'evidence/raw'/tag),*map(str,args)]
 subprocess.run(cmd,env=env,cwd=cwd,check=True)
run('01_prepare',[sys.executable,work/'prepare.py'])
down=work/'forwarder_source/tizen_src/downloadable'
run('02_generate',[sys.executable,down/'ewk_api_wrapper_generator.py','--out-cpp='+str(work/'forwarder_source/ewk_api_wrapper_generated.cc')],work/'forwarder_source')
for n,script in enumerate(['build.py','run_ewk.py','run_vectors.py','run_boundary.py','run_units.py','release_check.py','allocation_check.py'],3):
 run('%02d_%s'%(n,script),[sys.executable,work/script])
print('Reverse mechanism and error-contract evidence:',work/'evidence')
print('Allocation failure remains a release blocker; a completed script is not a production approval.')
