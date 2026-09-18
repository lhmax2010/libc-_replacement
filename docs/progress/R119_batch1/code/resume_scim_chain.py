#!/usr/bin/env python3
"""保存已逐行核对的 SCIM 源码、安装头包含及别名链。"""
import hashlib
import json
import pathlib

out = pathlib.Path(__file__).resolve().parents[1]
proof = json.loads((out / 'W1/positive/scim_wstring.json').read_text())
root = pathlib.Path(proof['source_includes'][0]['root']) / 'unpacked'
sources = list(root.glob('*/ise-engine-anthy-*/src/scim_anthy_factory.cpp'))
assert len(sources) == 1, sources
header = pathlib.Path(proof['header_matches'][0]['metadata']['cache_path'])
spec = [(sources[0], ['#define Uses_SCIM_UTILITY', '#include <scim.h>']),
        (header.parent / 'scim.h', ['#ifdef Uses_SCIM_UTILITY', '#include <scim_utility.h>']),
        (header.parent / 'scim_types.h', ['typedef wchar_t ucs4_t;', 'typedef std::basic_string<ucs4_t> WideString;'])]
rows = []
for path, needles in spec:
    data = path.read_bytes()
    lines = data.decode().splitlines()
    for needle in needles:
        hits = [i for i, s in enumerate(lines) if s.strip() == needle]
        assert hits, (path, needle)
        for i in hits:
            rows.append(dict(path=str(path), sha256=hashlib.sha256(data).hexdigest(), line=i+1,
                             context='\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0, i-2), min(len(lines), i+3)))))
rows.append({'limit':'源码宏与包含链 + 真实 UND/DEF；wchar_t 实例由真实 ELF 确认，不据条件源码假定所有架构配置。'})
(out / 'W1/positive/scim_wstring_include_chain.json').write_text(json.dumps(rows, ensure_ascii=False, indent=2)+'\n')
print(json.dumps(rows, ensure_ascii=False, indent=2))
