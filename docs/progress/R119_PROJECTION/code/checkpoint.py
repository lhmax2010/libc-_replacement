#!/usr/bin/env python3
from common import *
end=int(sys.argv[1]);info=json.loads((OUT/f'CHECKPOINT_{end:02}.json').read_text());objs=json.loads((OUT/'STDLIB_OBJECT_OCCURRENCES.json').read_text())
(OUT/'STATUS.md').write_text(f'''# 投影结果审计状态

已处理第 1–{end} 个 / 65 个 COMPILE_TIME_ONLY 候选；未修改原重判记录或 plan.json。

- 候选汇总：{json.dumps(info['candidate_results'],ensure_ascii=False)}。
- 逐处结果：{json.dumps(info['occurrence_results'],ensure_ascii=False)}。
- 含 std 对象投影的候选：{info['candidates_with_object']}；包边审查尚未结束，不能把对象投影计为新包边。
- 全量范围与数量：COVERAGE_INVENTORY.tsv；逐处实际实参、来源及结果：occurrences/ 与 results/。
- 编译测量使用从平台原文提取的封闭表达式；约束投影若固定结果类型，单列存在性未观测。对依赖名称、using 导入、模板模板实参等保留具体原因，不填任意 int 代入。
- 两库结构对照、五次输出一致性、源文件 SHA 核验保持；没有重判 BUILTIN_ALIAS / EMPTY_TYPE 的既有 24 项。
- 资源 light、串行 nice 19 / ionice 3、RLIMIT_AS 30%；不用板子，不构建平台包。
''')
scripts=list((OUT/'code').glob('*.py'))+list((OUT/'probes').glob('*.cpp'))+[REC,ROOT/'docs/progress/R119_batch1/code/resume_limited.py']
save(OUT/'SCRIPT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in scripts})
for path,digest in json.loads((OUT/'FROZEN_INPUT_SHA256.json').read_text()).items():
 if sha(ROOT/path)!=digest:raise RuntimeError('PROTECTED_INPUT_CHANGED '+path)
print(json.dumps(info,ensure_ascii=False))
for h in objs:
 print('OBJECT',h['id'],h['decision']['result_type'],h['references'][0]['installed_path'],h['line']);print(h['context'])
