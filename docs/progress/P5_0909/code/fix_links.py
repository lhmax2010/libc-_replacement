#!/usr/bin/env python3
"""仅执行 P4 M16/M17 表驱动机械链接替换；写明每处变更。"""
import collections
import csv
import pathlib

root = pathlib.Path('docs/progress/P5_0909')
p4 = pathlib.Path('docs/progress/P4_0909')
rows = list(csv.DictReader((p4 / 'RELATIVE_LINK_REPLACEMENTS.tsv').open(), delimiter='\t'))
fixes = list(csv.DictReader((p4 / 'CORRECTED_URLS.tsv').open(), delimiter='\t'))
assert len(rows) == 57 and len(fixes) == 5
ledger = []
for name, expected in [('chromium_research_zh.md', 28), ('chromium_research_en.md', 29)]:
    path = pathlib.Path('HQ_files') / name
    content = path.read_text()
    subset = [r for r in rows if r['report'] == name]
    assert len(subset) == expected
    mapping = {r['old_target']: r['suggested_target'] for r in subset}
    counts = collections.Counter(r['old_target'] for r in subset)
    for old, new in mapping.items():
        assert content.count('](' + old + ')') == counts[old], (name, old)
        content = content.replace('](' + old + ')', '](' + new + ')')
        ledger.append(['M16', name, str(counts[old]), old, new])
    for row in fixes:
        new = row['replacement_url']
        old = new.replace('llvm-project/libcxx/+/', 'llvm-project/+/')
        revision, tail = old.split('/+/')[1].split('/', 1)
        old = old.split('/+/')[0] + '/+/' + revision + '/libcxx/' + tail
        count = content.count(old)
        assert count == 1, (name, old, count)
        content = content.replace(old, new)
        ledger.append(['M17', name, '1', old, new])
    path.write_text(content)
with (root / 'LINK_EDITS.tsv').open('w') as stream:
    writer = csv.writer(stream, delimiter='\t', lineterminator='\n')
    writer.writerow(['item', 'file', 'occurrences', 'old_target', 'new_target'])
    writer.writerows(ledger)
print('M16: 57 occurrences; M17: 5 unique URLs, 10 occurrences; two reports updated')
