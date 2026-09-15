#!/usr/bin/env python3
"""只读冻结发布 ELF 的动态符号；不加载目标、不修改平台。"""
import concurrent.futures
import csv
import hashlib
import json
import pathlib
import struct
import time

OUT = pathlib.Path(__file__).resolve().parent
CORPUS = pathlib.Path('/home/toolchain/development/libc++_replacement')
INVENTORY = CORPUS / 'progress/R11/tables/elf_inventory.tsv'
PACKAGES = CORPUS / 'progress/R11/tables/binary_package_records.tsv'
TARGET = b'ewk_parse_cookie'

def sha(p):
    h = hashlib.sha256()
    with p.open('rb') as f:
        for part in iter(lambda: f.read(4 * 1024 * 1024), b''):
            h.update(part)
    return h.hexdigest()

def symbols(path, target=TARGET):
    with path.open('rb') as f:
        ident = f.read(16)
        if ident[:4] != b'\x7fELF' or ident[4] not in (1, 2) or ident[5] not in (1, 2):
            raise ValueError('不支持的 ELF 标识')
        endian = '<' if ident[5] == 1 else '>'
        wide = ident[4] == 2
        fmt = endian + ('HHIQQQIHHHHHH' if wide else 'HHIIIIIHHHHHH')
        header = struct.unpack(fmt, f.read(struct.calcsize(fmt)))
        offset, entsize, count = header[5], header[10], header[11]
        sfmt = endian + ('IIQQQQIIQQ' if wide else 'IIIIIIIIII')
        ssize = struct.calcsize(sfmt)
        if not offset:
            raise ValueError('无 section headers，需其他方法，不记零命中')
        if entsize < ssize:
            raise ValueError('section header 大小异常')
        f.seek(offset)
        first = struct.unpack(sfmt, f.read(ssize))
        if count == 0:
            count = first[5]
        sections = []
        for i in range(count):
            f.seek(offset + i * entsize)
            sections.append(struct.unpack(sfmt, f.read(ssize)))
        result = []
        dyn_tables = 0
        for index, sec in enumerate(sections):
            if sec[1] != 11:  # SHT_DYNSYM
                continue
            dyn_tables += 1
            strings = sections[sec[6]]
            f.seek(strings[4])
            data = f.read(strings[5])
            positions = set()
            at = 0
            while True:
                pos = data.find(target + b'\0', at)
                if pos < 0:
                    break
                # ELF 字符串表允许后缀共享，不能要求前一字节也是 NUL。
                positions.add(pos)
                at = pos + 1
            if not positions:
                continue
            syfmt = endian + ('IBBHQQ' if wide else 'IIIBBH')
            symsize = struct.calcsize(syfmt)
            if sec[9] < symsize:
                raise ValueError('dynamic symbol 大小异常')
            version_sec = next((s for s in sections if s[1] == 0x6fffffff and s[6] == index), None)
            version_data = None
            if version_sec:
                f.seek(version_sec[4])
                version_data = f.read(version_sec[5])
            f.seek(sec[4])
            dynamic_data = f.read(sec[5])
            for symindex in range(sec[5] // sec[9]):
                values = struct.unpack_from(syfmt, dynamic_data, symindex * sec[9])
                if values[0] not in positions:
                    continue
                ndx = values[3] if wide else values[5]
                if ndx == 0xffff:
                    raise ValueError('目标动态符号使用 SHN_XINDEX，需单独核查')
                versym = struct.unpack_from(endian + 'H', version_data, 2 * symindex)[0] if version_data else None
                result.append({'symbol': target.decode(), 'dynsym_index': symindex,
                               'role': 'UND' if ndx == 0 else 'DEFINED',
                               'section_index': ndx, 'versym_raw': versym,
                               'version_index': (versym & 0x7fff) if versym is not None else None,
                               'version_hidden': bool(versym & 0x8000) if versym is not None else None})
        return dyn_tables, result

def scan(row):
    path = CORPUS / 'tmp/R11/extracted' / row['rpm_sha256'][:2] / row['rpm_sha256'] / row['path'].lstrip('/')
    common = {k: row[k] for k in ['repo_id', 'name', 'arch', 'sourcerpm', 'rpm_sha256', 'path', 'runtime_elf']}
    common['local_path'] = str(path)
    try:
        tables, found = symbols(path)
        return dict(common, status='SCANNED', dynsym_tables=tables, hits=found)
    except Exception as e:
        return dict(common, status='NOT_AVAILABLE', error=type(e).__name__ + ': ' + str(e), hits=[])

def main():
    start = time.time()
    rows = list(csv.DictReader(INVENTORY.open(), delimiter='\t'))
    identity = {str(p): {'sha256': sha(p), 'bytes': p.stat().st_size} for p in (INVENTORY, PACKAGES)}
    (OUT / 'INPUT_IDENTITY.json').write_text(json.dumps(identity, indent=2) + '\n')
    results = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
        for n, result in enumerate(pool.map(scan, rows), 1):
            results.append(result)
            if result['hits']:
                print('HIT', result['name'], result['arch'], result['path'], result['hits'], flush=True)
            if n % 2000 == 0:
                print('已检查', n, '/', len(rows), 'elapsed', round(time.time()-start, 1), flush=True)
    with (OUT / 'SCAN.jsonl').open('w') as out:
        for r in results:
            out.write(json.dumps(r, ensure_ascii=False) + '\n')
    matches = [r for r in results if r['hits']]
    missing = [r for r in results if r['status'] != 'SCANNED']
    summary = {'inventory_rows': len(rows), 'scanned': len(rows)-len(missing),
               'not_available': len(missing),
               'runtime_rows': sum(r['runtime_elf']=='YES' for r in results),
               'target_undefined_elfs': sum(any(s['role']=='UND' for s in r['hits']) for r in results),
               'target_defined_elfs': sum(any(s['role']=='DEFINED' for s in r['hits']) for r in results),
               'elapsed_seconds': time.time()-start}
    (OUT / 'MATCHES.json').write_text(json.dumps(matches, indent=2, ensure_ascii=False) + '\n')
    (OUT / 'UNAVAILABLE.json').write_text(json.dumps(missing, indent=2, ensure_ascii=False) + '\n')
    (OUT / 'SCAN_SUMMARY.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary, indent=2), flush=True)

if __name__ == '__main__':
    main()
