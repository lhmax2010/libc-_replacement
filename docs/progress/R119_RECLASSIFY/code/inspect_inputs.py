#!/usr/bin/env python3
"""只读列出冻结两批未定项，供逐项人工成因审读，不自动归类。"""
import csv
import json
import pathlib

for batch, filename in [(1, 'REVIEW_001_100.tsv'), (2, 'REVIEW_101_200.tsv')]:
    root = pathlib.Path(f'docs/progress/R119_batch{batch}/W1')
    for row in csv.DictReader((root / filename).open(), delimiter='\t'):
        if row['result'] != 'NOT_OBSERVED':
            continue
        dossier = json.loads((root / 'reviewed' / f"{int(row['rank']):03}.json").read_text())
        print(row['rank'], row['candidate'], json.dumps(dossier.get('review', dossier), ensure_ascii=False))
