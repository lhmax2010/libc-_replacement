#!/usr/bin/env python3
"""仅归档文档输入并检查文档；不构建、不运行探针、不改既有证据。"""
import hashlib
import gzip
import json
import pathlib
import re
import shutil
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[3]
OUT = ROOT / 'docs/progress/P10_0915'
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

if sys.argv[1] == 'init':
    inputs = {
        'review.txt': 'review_libc++_libstdc++_adaptor.txt',
        'proposal_before.md': 'adaptor_proposal_for_review.md',
        'base_zh.md': 'docs/progress/ADAPTOR_0912/W4/adaptor_analysis_zh.md',
        'record.py': 'docs/progress/API_0911/record.py',
    }
    dest = OUT / 'inputs'
    dest.mkdir(exist_ok=False)
    for name, src in inputs.items():
        shutil.copyfile(ROOT / src, dest / name)
    tracked = subprocess.check_output(['git', 'ls-files', '-z', 'docs/progress'], cwd=ROOT).split(b'\0')
    manifest = {}
    for raw in tracked:
        if not raw:
            continue
        p = pathlib.Path(raw.decode())
        if 'P10_0915' not in p.parts and (ROOT / p).is_file():
            manifest[str(p)] = sha(ROOT / p)
    (OUT / 'evidence_before.json').write_text(json.dumps(manifest, indent=2, ensure_ascii=False) + '\n')
    (OUT / 'inputs/INDEX.json').write_text(json.dumps({name: {'source': src, 'sha256': sha(dest / name)} for name, src in inputs.items()}, indent=2, ensure_ascii=False) + '\n')
    print('输入快照完成；冻结既有受跟踪材料文件数:', len(manifest))
elif sys.argv[1] == 'check':
    with gzip.open(OUT / 'evidence_before.json.gz', 'rt') as f:
        manifest = json.load(f)
    changed = [p for p, digest in manifest.items() if not (ROOT / p).is_file() or sha(ROOT / p) != digest]
    print('既有材料变化:', changed)
    assert not changed
    zh = OUT / 'adaptor_proposal_for_review.md'
    en = OUT / 'adaptor_proposal_for_review_en.md'
    texts = [p.read_text() for p in (zh, en)]
    # 正向对照：确认零命中检查真的能识别禁止项，而非空扫描。
    assert re.search(r'[\u3400-\u9fff]', 'English 中文')
    assert re.search(r'\b(?:R\d{2,3}|P\d{1,2}|W[1-4]|D-\d+)\b', 'Example R110')
    assert not (OUT / '__positive_control_missing_link__').exists()
    print('英文中文残留、内部编号、失效本地链接检测的正向对照通过')
    for p, text in zip((zh, en), texts):
        assert text.count('```') % 2 == 0, p
        broken = []
        for target in re.findall(r'\]\(([^)]+)\)', text):
            if '://' not in target and not target.startswith('#'):
                local = target.split('#', 1)[0]
                if not (p.parent / local).exists():
                    broken.append(target)
        assert not broken, (p, broken)
        print(p.name, '链接目标及代码围栏检查通过')
    assert not re.search(r'[\u3400-\u9fff]', texts[1]), '英文正文含中文'
    for text in texts:
        body = re.sub(r'\]\([^)]*\)', '](LINK)', text)
        assert not re.search(r'\b(?:R\d{2,3}|P\d{1,2}|W[1-4]|D-\d+)\b', body), '对外正文含内部编号'
    # C 接口设计中英必须完全相同；不是对新设计进行编译验证。
    blocks = [re.findall(r'```c\n(.*?)```', text, re.S) for text in texts]
    assert blocks[0] == blocks[1] and len(blocks[0]) == 2
    options = (ROOT / 'docs/progress/API_0911/W3/OPTIONS.md').read_text()
    for block in blocks[0]:
        assert block in options, 'C 签名与原设计不同'
    matrix = json.loads((ROOT / 'docs/progress/ADAPTOR_0912/W2/summary.json').read_text())
    assert len(matrix) == 6 and sum(x['runs'] for x in matrix) == 30
    assert sum(x['case_observations'] for x in matrix) == 360
    for row in matrix:
        assert row['runs'] == 5 and row['case_observations'] == 60
        assert row['exact_case_observations'] == (20 if row['path'] == 'drop' else 60)
        assert row['destroyed_case_scopes'] == 60
    print('EWK 6 格/30 运行/360 场景及逐格具体值计数与原始汇总一致')
    disposition = (OUT / 'REVIEW_DISPOSITION.md').read_text()
    ids = re.findall(r'^\| ([OKCL]\d+) \|', disposition, re.M)
    assert len(ids) == len(set(ids)) == 120
    for prefix, count in [('O', 19), ('K', 44), ('C', 48), ('L', 9)]:
        assert [f'{prefix}{i:02}' for i in range(1, count+1)] == [i for i in ids if i.startswith(prefix)]
    print('120 条处置行编号唯一连续；这只是台账完整性，不替代人工语义复核')
    for p in (zh, en):
        root_version = ROOT / p.name
        expected = p.read_text().replace('](../', '](docs/progress/')
        assert root_version.read_text() == expected, '根目录副本不同步: ' + p.name
    print('中英 C 签名相同；英文无中文；正文无内部编号；根目录副本同步')
    print('SHA256:', sha(zh), sha(en))
elif sys.argv[1] == 'seal':
    snapshot = OUT / 'snapshots'
    snapshot.mkdir(exist_ok=True)
    shutil.copyfile(pathlib.Path(__file__), snapshot / 'audit.py')
    paths = [p for p in OUT.rglob('*') if p.is_file()
             and 'raw' not in p.relative_to(OUT).parts
             and 'delivery' not in p.relative_to(OUT).parts
             and p.name != 'SHA256SUMS']
    paths += [ROOT / 'adaptor_proposal_for_review.md', ROOT / 'adaptor_proposal_for_review_en.md']
    (OUT / 'SHA256SUMS').write_text(''.join(f'{sha(p)}  {p.relative_to(ROOT)}\n' for p in sorted(paths)))
    print('正文、处置、自检、脚本快照及输入已记 SHA256；raw 完整命令另存，不计入此内容摘要。')
else:
    raise SystemExit('使用 init、check 或 seal')
