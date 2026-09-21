#!/usr/bin/env python3
"""仅汇编冻结证据；不编译、不运行探针、不扫描源码或 ELF。"""
import csv
import hashlib
import io
import json
import os
from pathlib import Path
import re

OUT = Path(__file__).resolve().parents[1]
ROOT = OUT.parents[2]
assert (ROOT / '.git').exists()
inputs = {}

def read(path):
    p = ROOT / path
    data = p.read_bytes()
    inputs[str(p.relative_to(ROOT))] = hashlib.sha256(data).hexdigest()
    return data.decode()

def jread(path):
    return json.loads(read(path))

def tread(path):
    return list(csv.DictReader(io.StringIO(read(path)), delimiter='\t'))

def write(name, text):
    p = OUT / name
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text)

def jout(name, obj):
    write(name, json.dumps(obj, ensure_ascii=False, indent=2) + '\n')

def tout(name, rows):
    s = io.StringIO()
    writer = csv.DictWriter(s, fieldnames=list(rows[0]), delimiter='\t', lineterminator='\n')
    writer.writeheader()
    writer.writerows(rows)
    write(name, s.getvalue())

def esc(x):
    # 模板实参在非 code 单元格里不能被 Markdown 当成 HTML 标签吞掉。
    bits=re.split(r'(`[^`]*`)',str(x))
    text=''.join(b if b.startswith('`') else b.replace('<','&lt;').replace('>','&gt;') for b in bits)
    return text.replace('|', '\\|').replace('\n', '<br>')

def table(headers, rows):
    return '| ' + ' | '.join(headers) + ' |\n| ' + ' | '.join(['---'] * len(headers)) + ' |\n' + ''.join('| ' + ' | '.join(map(esc, r)) + ' |\n' for r in rows)

sources = {
    'base-map': 'R119_RECLASSIFY/CROSS_CANDIDATE_EDGE_MAP.tsv',
    'base-count': 'R119_RECLASSIFY/FINAL.md',
    'scope': 'R119_RECLASSIFY/SCOPE.md',
    'edges': 'R119_DIVERGENT/CROSS_SYMBOL_EDGES_UPDATED.tsv',
    'extra-proofs': 'R119_DIVERGENT/CONFIRMED_EDGE_PROOFS.json',
    'structure': 'R119_DIVERGENT/EDGE_STRUCTURAL_HEADERS.json',
    'arm-elf': 'R119_ARM_RETEST/ACTUAL_ARM_EDGES.json',
    'arm-signatures': 'R119_ARM_RETEST/SIGNATURES.json',
    'arm-count': 'R119_ARM_RETEST/COUNTING_SCOPE.json',
    'arm-final': 'R119_ARM_RETEST/FINAL.md',
    'arm-config': 'R119_ARM_RETEST/CONFIGURATION.json',
    'aarch64-config': 'R119_ARM_RETEST/AARCH64_CONFIGURATION.json',
    'rpm-binding': 'R119_ARM_RETEST/RPM_BINDINGS.json',
    'clock': 'R119_RECLASSIFY/items/121.json',
    'clock-gnu': 'R119_RECLASSIFY/A_DEFINITIONS_gnu.json',
    'clock-cxx': 'R119_RECLASSIFY/A_DEFINITIONS_cxx.json',
    'types-x86': 'R119_DIVERGENT/BUILTIN_DIVERGENT.tsv',
    'types-arm': 'R119_ARM_RETEST/ARMV7L_TYPES.tsv',
    'types-aarch64': 'R119_ARM_RETEST/AARCH64_TYPES.tsv',
    'declarations': 'R119_DIVERGENT/PUBLIC_DECLARATIONS.tsv',
    'boundary-control': 'R119_DIVERGENT/BOUNDARY_POSITIVE_CONTROL.json',
    'pending': 'R119_RECLASSIFY/CD_GAPS_62.tsv',
    'projection': 'R119_PROJECTION/FINAL.md',
    'projection-count': 'R119_DIVERGENT/RECLASSIFIED_COUNTS.json',
    'measurement': 'R119_DIVERGENT/MEASUREMENT_SUMMARY.json',
    'mechanisms': 'R119_DIVERGENT/MECHANISM_COUNTS.json',
    'template': 'R118/TEMPLATE.md',
    'timings': 'R118/TIMINGS.json',
}
sources = {k: 'docs/progress/' + v for k, v in sources.items()}
for path in sources.values():
    read(path)

def cite(key, lang='zh'):
    # 每个引注的目标同时包含仓内路径和完整 SHA256。
    return f'[{"证据" if lang == "zh" else "Evidence"}: {key}](EVIDENCE.md#{key})'

edges = tread(sources['edges'])
base = tread(sources['base-map'])
arm = jread(sources['arm-elf'])
signatures = jread(sources['arm-signatures'])
gap = tread(sources['pending'])
types = tread(sources['types-x86'])
armtypes = {r['id']: r for r in tread(sources['types-arm'])}
aarchtypes = {r['id']: r for r in tread(sources['types-aarch64'])}
planning = jread(str((OUT / 'code/editorial.json').relative_to(ROOT)))
assert len(edges) == 23 and len(base) == 44 and len(gap) == 62 and len(types) == 13
assert len({(r['consumer_package'], r['provider_package']) for r in edges}) == 18
assert len({(r['consumer_package'], r['provider_package']) for r in base}) == 16
assert len({r['raw_symbol'] for r in base}) == 21
assert sum(r['primary_cause'] == 'C' for r in gap) == 23
assert sum(r['primary_cause'] == 'D' for r in gap) == 39
assert len(planning['edges']) == 23
assert len(planning['gaps_en']) == 62

base_lookup = {}
for row in base:
    key = (row['consumer_package'], row['provider_package'], row['raw_symbol'])
    base_lookup.setdefault(key, []).append(row)

edge_rows = []
expanded = []
next_rows = []
for n, (e, p) in enumerate(zip(edges, planning['edges']), 1):
    assert p['consumer'] == e['consumer_package'] and p['provider'] == e['provider_package']
    key = (e['consumer_package'], e['provider_package'], e['raw_symbol'])
    related = base_lookup.get(key, [])
    proof_path = e['evidence'].split('#')[0]
    proof = jread(proof_path)
    matches = proof['elf_matches'] if isinstance(proof, dict) else proof
    def symbols(m):
        value = m.get('raw_symbol') or m.get('symbol', {}).get('raw_symbol')
        return value if isinstance(value, list) else [value]
    matches = [m for m in matches if m['provider']['sha256'] == e['provider_sha256']
               and m['consumer']['sha256'] == e['consumer_sha256']
               and e['raw_symbol'] in symbols(m)]
    assert matches, (n, 'Missing exact frozen UND/DEF pair')
    pair = matches[0]
    a = next((x for x in arm if x['consumer']['package']['name'] == e['consumer_package'] and x['provider']['package']['name'] == e['provider_package']), None)
    row = dict(edge=n, consumer=e['consumer_package'], provider=e['provider_package'], interface=p['interface'],
        category_zh=p['category_zh'], category_en=p['category_en'],
        shape_zh=p['shape_zh'], shape_en=p['shape_en'],
        ownership_zh=p['ownership_zh'], ownership_en=p['ownership_en'],
        original_candidates=';'.join(r['candidate'] for r in related),
        additional_recorded_candidates=e['candidates'] if not related else '',
        x86_64='ELF_UND_DEF_CONFIRMED', armv7l='ELF_UND_DEF_CONFIRMED' if a else 'NOT_OBSERVED',
        aarch64='NOT_OBSERVED',
        x86_consumer_elf=e['consumer_elf'], x86_consumer_sha256=e['consumer_sha256'],
        x86_provider_elf=e['provider_elf'], x86_provider_sha256=e['provider_sha256'],
        x86_raw_symbol=e['raw_symbol'], x86_demangled=e['demangled'],
        x86_consumer_raw_lines='\n'.join(pair.get('consumer_lines', pair['consumer'].get('lines', []))),
        x86_provider_raw_lines='\n'.join(pair.get('provider_lines', pair['provider'].get('lines', []))),
        x86_proof=e['evidence'], x86_proof_sha256=inputs[proof_path],
        arm_consumer_elf=a['consumer']['path'] if a else '', arm_consumer_sha256=a['consumer']['sha256'] if a else '',
        arm_provider_elf=a['provider']['path'] if a else '', arm_provider_sha256=a['provider']['sha256'] if a else '',
        arm_raw_symbol=a['symbol'] if a else '',
        arm_consumer_raw_lines='\n'.join(a['consumer']['symbol_lines']) if a else '',
        arm_provider_raw_lines='\n'.join(a['provider']['symbol_lines']) if a else '',
        arm_proof=sources['arm-elf'] if a else '', arm_proof_sha256=inputs[sources['arm-elf']] if a else '')
    edge_rows.append(row)
    for r in related:
        expanded.append(dict(edge=n,candidate=r['candidate'],original_rank=r['rank'],consumer=r['consumer_package'],provider=r['provider_package'],shape=r['shape'],ownership=r['ownership'],proof=r['proof'],map_sha256=inputs[sources['base-map']]))
    next_rows.append(dict(edge=n,consumer=e['consumer_package'],libcxx_provider_package=e['provider_package'],
        provider_elf_target=e['provider_elf'],interface=p['interface'],
        prerequisite_zh=p['prerequisite_zh'],prerequisite_en=p['prerequisite_en'],
        assertion_zh=p['assertion_zh'],assertion_en=p['assertion_en'],
        estimate=p['estimate'],estimate_kind='EDITORIAL_PLANNING_NOT_MEASURED_PER_EDGE_PER_ARCH_EXCLUDES_BUILD_AND_WAIT',
        x86_64='CAN_START_WHEN_PROVIDER_AND_CONTROL_FIXTURE_READY',
        armv7l='PRODUCT_VALIDATION_PLANNED_ON_PHYSICAL_BOARD; CURRENT_SERVICE_OR_HARDWARE_REQUIREMENT_NOT_OBSERVED',
        current_provider_availability='NOT_RECHECKED',evidence=e['evidence'],evidence_sha256=inputs[proof_path]))

tout('EDGES.tsv', edge_rows)
tout('CANDIDATE_MAP.tsv', expanded)
tout('NEXT_STAGE.tsv', next_rows)
assert len(expanded) == 44
assert sum(e['armv7l'] == 'ELF_UND_DEF_CONFIRMED' for e in edge_rows) == 2

type_rows=[]
for t in types:
    a,b = armtypes[t['id']],aarchtypes[t['id']]
    assert a['result']=='IDENTICAL' and b['result']=='DIVERGENT'
    rr={'expression':t['expression']}
    for arch,values in [('x86_64',t),('armv7l',a),('aarch64',b)]:
        for lib in ['gnu','cxx']:
            rr[f'{arch}_{lib}_type']=values[f'{lib}_type']
            rr[f'{arch}_{lib}_encoding']=values.get(f'{lib}_encoding',values.get(f'{lib}_direct'))
            rr[f'{arch}_{lib}_size']=values[f'{lib}_size']
    rr['x86_evidence']=sources['types-x86'];rr['x86_sha256']=inputs[sources['types-x86']]
    rr['arm_evidence']=sources['types-arm'];rr['arm_sha256']=inputs[sources['types-arm']]
    rr['aarch64_evidence']=sources['types-aarch64'];rr['aarch64_sha256']=inputs[sources['types-aarch64']]
    type_rows.append(rr)
tout('BUILTIN_TYPES.tsv',type_rows)

gap_rows=[]
for r in gap:
    en=planning['gaps_en'][r['rank']]
    read(r['original_evidence'])
    gap_rows.append(dict(candidate=r['candidate'],primary_cause=r['primary_cause'],other_causes=r['other_causes'],
        status='PAUSED_NOT_OBSERVED',missing_zh=r['specific_gap'],restart_zh=r['next_evidence_needed'],
        restart_en=en,self_resolvability=r['self_resolvability'],dependency_zh=r['dependency'],
        source=sources['pending'],source_sha256=inputs[sources['pending']],
        original_evidence=r['original_evidence'],original_evidence_sha256=inputs[r['original_evidence']]))
tout('BACKLOG.tsv',gap_rows)

# 中英文主体来自明确区分观察与编辑判断的静态文本。
for lang in ['zh','en']:
    z=lang=='zh'
    body=read(str((OUT/f'code/body_{lang}.md').relative_to(ROOT)))
    for key in sources:
        body=body.replace('{{'+key+'}}',cite(key,lang))
    edge_table=table(['序号','消费方 → 提供方','接口','跨界形态；所有权','ELF 架构','原始证据'] if z else ['Row','Consumer → provider','Interface','Form; ownership','ELF architecture','Raw evidence'],[
        [e['edge'],e['consumer']+' → '+e['provider'],e['interface'],e[f'shape_{lang}']+'; '+e[f'ownership_{lang}'],
         'x86_64; armv7l' if e['armv7l']=='ELF_UND_DEF_CONFIRMED' else 'x86_64; armv7l NOT_OBSERVED',
         f'[ELF / SHA / symbol](EDGES_{lang}.md#edge-{e["edge"]})'] for e in edge_rows])
    type_table=table(['类型表达式','x86_64 GNU / libc++','armv7l GNU / libc++','aarch64 GNU / libc++'] if z else ['Type expression','x86_64 GNU / libc++','armv7l GNU / libc++','aarch64 GNU / libc++'],[
        [r['expression']]+[f'{r[a+"_gnu_type"]} (`{r[a+"_gnu_encoding"]}`, {r[a+"_gnu_size"]} B) / {r[a+"_cxx_type"]} (`{r[a+"_cxx_encoding"]}`, {r[a+"_cxx_size"]} B)' for a in ['x86_64','armv7l','aarch64']] for r in type_rows])
    next_table=table(['序号','需 libc++ 构建的提供包','先决条件','具体值/销毁检查','单边单架构量级'] if z else ['Row','Required libc++ provider package','Prerequisites','Value / destruction checks','Per edge, per architecture'],[
        [r['edge'],r['libcxx_provider_package'],r[f'prerequisite_{lang}'],r[f'assertion_{lang}'],r['estimate']] for r in next_rows])
    body=body.replace('{{edge_table}}',edge_table).replace('{{type_table}}',type_table).replace('{{next_table}}',next_table)
    write(f'SUMMARY_{lang}.md',body)

    parts=['# '+('逐边证据附表' if z else 'Evidence for each package boundary'),
        ('这是既有静态证据的转录，不是本轮运行结果。序号与主表一致；消费方 UND 为未定义引用，提供方 DEF 为定义。每个证据索引条目提供完整 SHA256。' if z else 'Transcription of existing static evidence, not new execution. Row numbers match the main table. UND denotes a consumer reference; DEF denotes a provider definition. Evidence-index entries include full SHA256 values.'), cite('edges',lang),cite('arm-elf',lang)]
    for e in edge_rows:
        n=e['edge'];parts+= [f'<a id="edge-{n}"></a>',f'## {n}. {e["consumer"]} → {e["provider"]}: {e["interface"]}',e[f'category_{lang}'],e[f'shape_{lang}']+'; '+e[f'ownership_{lang}'],
            f'### x86_64',
            table(['Role','ELF','SHA256'],[['consumer',e['x86_consumer_elf'],e['x86_consumer_sha256']],['provider',e['x86_provider_elf'],e['x86_provider_sha256']]]),
            '```text\n'+e['x86_consumer_raw_lines']+'\n'+e['x86_provider_raw_lines']+'\n```',
            '```text\n'+e['x86_demangled']+'\n```',
            f'[{"引用/定义原记录" if z else "UND/DEF record"}](../../../{e["x86_proof"].split("#")[0].removeprefix("docs/")}) — SHA256 `{e["x86_proof_sha256"]}`']
        # 从输出目录到仓内 docs/progress 的稳健相对链接。
        parts[-1]=f'[{"引用/定义原记录" if z else "UND/DEF record"}]({os.path.relpath(ROOT/e["x86_proof"].split("#")[0],OUT)}) — SHA256 `{e["x86_proof_sha256"]}`'
        if e['armv7l']=='ELF_UND_DEF_CONFIRMED':
            parts+=['### armv7l',table(['Role','ELF','SHA256'],[['consumer',e['arm_consumer_elf'],e['arm_consumer_sha256']],['provider',e['arm_provider_elf'],e['arm_provider_sha256']]]),
                '```text\n'+e['arm_consumer_raw_lines']+'\n'+e['arm_provider_raw_lines']+'\n```',cite('arm-elf',lang)]
            interface = 'dali' if e['consumer']=='dali2-ui-foundation' else 'zypp'
            parts += [('以下是既有记录中从真实 ARM 头文件交叉编译的调用方对象符号；不是 libc++ 提供方产物，也不是运行结果。' if z else 'The following symbols are from consumer objects cross-compiled from the real ARM headers in the existing records. They are not libc++ provider artifacts or execution results.'),cite('arm-signatures',lang)]
            for s in signatures:
                if s['interface']==interface:
                    parts += [s['library'], '```text\n'+s['symbol']+'\n```']
        else:parts+=['armv7l: `NOT_OBSERVED`.']
        parts+=['aarch64: `NOT_OBSERVED`.',('ELF 条目证明 GNU 发布产物间引用；另列的头文件编译符号（若有）与 ELF 条目均不证明混合库成功运行。' if z else 'ELF entries establish references between released GNU artifacts. Neither those entries nor the separately listed header-compiled symbols, where present, prove successful mixed-library execution.')]
    write(f'EDGES_{lang}.md','\n\n'.join(parts)+'\n')

    gap_table=table(['候选','主因','缺口；重启条件','本线可解性','依据'] if z else ['Candidate','Primary cause','Missing evidence / restart condition','Locally resolvable','Evidence'],[
        [r['candidate'],r['primary_cause'],(r['missing_zh']+' '+r['restart_zh']) if z else r['restart_en'],r['self_resolvability'],cite('pending',lang)] for r in gap_rows])
    write(f'BACKLOG_{lang}.md',('# 暂停项逐项清单\n\n主因 C：别名、返回类型、派生/外层类型等使原名字面不能完成证明。主因 D：真实产物、调用链或配置缺失。两者可重叠；保留原判定，不重新定界。PARTLY 不是保证可闭合；YES_LOCAL_RESEARCH 只表示可继续本地取证。\n\n' if z else '# Paused items: individual restart conditions\n\nPrimary C: aliases, return types, derived/containing types or similar identity issues prevent a literal-name proof. Primary D: missing artifacts, call paths or configuration. Causes can overlap. Original classifications are retained, not re-investigated. PARTLY does not promise closure; YES_LOCAL_RESEARCH means local investigation can resume.\n\n')+gap_table)

# 原始候选到边的完整44项映射；保持原证据中的形态与所有权文字。
write('CANDIDATE_MAP.md','# Candidate-to-edge mapping\n\n'+cite('base-map','en')+'\n\n'+table(['Candidate','Edge row','Consumer → provider'],[[r['candidate'],r['edge'],r['consumer']+' → '+r['provider']] for r in expanded]))

# 为所有输入文件提供完整路径与摘要；路径中保留历史编号以保证证据可定位。
index=['# Evidence index / 证据索引','SHA256 identifies the exact repository record, not a new runtime measurement. Historical identifiers are retained only in evidence paths.']
for key,path in sources.items():
    index += [f'<a id="{key}"></a>',f'## {key}',f'[{path}]({os.path.relpath(ROOT/path,OUT)})',f'SHA256: `{inputs[path]}`']
write('EVIDENCE.md','\n\n'.join(index)+'\n')
jout('INPUT_SHA256.json',inputs)
assert all(hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==h for p,h in inputs.items())
all_candidates=set(c for e in edges for c in e['candidates'].split(';'))
jout('CHECKS.json',dict(edges=23,package_pairs=18,original_candidates=44,original_pairs=16,original_symbols=21,
    additional_distinct_candidate_names=sorted(all_candidates-{r['candidate'] for r in base}),
    arm_elf_edges=2,arm_total_scope='NOT_OBSERVED',types=13,backlog=62,backlog_C=23,backlog_D=39,
    remaining_arm_items={'identical_x86':283,'namespace':114,'implementation':111,'enum':4,'not_available_x86':60},
    inputs_unchanged=True,new_measurements=False,board_used=False))
print('ASSEMBLED: 23 edges; 18 package pairs; 44 original candidate associations; 2 ARM ELF edges; 13 type rows; 62 backlog rows. Input SHA256 unchanged.')
