#!/usr/bin/env python3
"""复核现有真实 EWK 构建断点；只改隔离输出目录，不改源码或既有记录。"""
import hashlib,json,pathlib,shlex,subprocess,sys
ROOT=pathlib.Path(__file__).resolve().parents[4]
OUT=pathlib.Path(__file__).resolve().parent
REC=ROOT/'docs/progress/API_0911/record.py'
prior=ROOT/'docs/progress/ADAPTOR_0912/W1/raw/component_ewk_cookie_parser.command.txt'
cmd=shlex.split(next(x[9:] for x in prior.read_text().splitlines() if x.startswith('command: ')))
old_output=cmd[cmd.index('-o')+1]
new_output=ROOT/'tmp/P11_0916/ewk_cookie_parser.o'
cmd[cmd.index('-o')+1]=str(new_output)
inputs=[]
for token in cmd:
    p=pathlib.Path(token)
    if token.endswith('.cc') and p.exists():
        inputs.append({'path':token,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
subprocess.run([sys.executable,str(REC),str(OUT/'raw/003_component_retry'),*cmd])
rc=int((OUT/'raw/003_component_retry.exitcode').read_text())
err=(OUT/'raw/003_component_retry.stderr').read_text()
result={'previous_command':str(prior),'only_command_change':{'old_output':old_output,'new_output':str(new_output)},
        'input_source_identity':inputs,'compiler_exit':rc,'stderr':err,'component_object_available':new_output.exists(),
        'complete_libcxx_ewk_provider':'NOT_AVAILABLE','reverse_matrix':'NOT_OBSERVED',
        'needed':'产品对应源码/生成头与 base/net 构建闭包，以及以 libc++ 构建并可加载的真实 EWK 实现库'}
assert rc!=0, '编译前置发生变化，应检查后续构建闭包，不沿用旧阻塞结论'
(OUT/'PREREQUISITE.json').write_text(json.dumps(result,indent=2,ensure_ascii=False)+'\n')
print(json.dumps(result,indent=2,ensure_ascii=False))
