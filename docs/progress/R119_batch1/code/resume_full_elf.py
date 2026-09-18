#!/usr/bin/env python3
"""串行扩大 x86_64 C++ ELF 符号覆盖；保留完整 stdout/退出码并可恢复。

不推断链接实际落点，不执行 ELF；同名 UND/DEF 仍须结合公开签名/包含链复核。
"""
import csv
import datetime
import gzip
import hashlib
import json
import pathlib
import shlex
import sqlite3
import subprocess
import sys
import time

root = pathlib.Path(__file__).resolve().parents[4]
old = root.parent / 'libc++_replacement'
out = root / 'docs/progress/R119_batch1/W1/full_elf'
out.mkdir(parents=True, exist_ok=True)
tmp = root / 'tmp/R119_batch1/full_elf'
tmp.mkdir(parents=True, exist_ok=True)
db = sqlite3.connect(tmp / 'symbols.sqlite3')
db.execute('PRAGMA cache_size=-4096')
db.execute('CREATE TABLE IF NOT EXISTS elf (id INTEGER PRIMARY KEY, metadata TEXT, status TEXT, sha256 TEXT, elapsed REAL)')
db.execute('CREATE TABLE IF NOT EXISTS symbol (elf_id INTEGER, name TEXT, ndx TEXT, binding TEXT, kind TEXT)')
with (old/'progress/R11/tables/elf_inventory.tsv').open() as f:
    inventory = [r for r in csv.DictReader(f, delimiter='\t') if r['runtime_elf']=='YES' and r['cpp_indicator']=='YES' and r['arch']=='x86_64']
(out/'INVENTORY.json').write_text(json.dumps(inventory, ensure_ascii=False, indent=2)+'\n')
max_count = int(sys.argv[1]) if len(sys.argv)>1 else len(inventory)
done = {r[0] for r in db.execute('SELECT id FROM elf')}
start = time.monotonic()
def gate():
    for attempt in range(6):
        cmd = ['tools/resource_gate.sh','--level','light']
        p = subprocess.run(cmd, capture_output=True, text=True)
        event = dict(command=shlex.join(cmd), exitcode=p.returncode, stdout=p.stdout, stderr=p.stderr,
                     time_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
        with (out/'gates.jsonl').open('a') as f:
            f.write(json.dumps(event)+'\n')
        print('GATE', attempt, p.returncode, flush=True)
        if p.returncode==0:
            return
        if p.returncode not in (20,21) or attempt==5:
            raise SystemExit(p.returncode)
        print('RESOURCE_WAIT_SECONDS=600; analysis paused', flush=True)
        for _ in range(40):
            time.sleep(15)
    raise AssertionError('unreachable')
gate()
for i,r in enumerate(inventory, 1):
    if i>max_count:
        break
    if i in done:
        continue
    if i%100==0:
        gate()
    path = old/'tmp/R11/extracted'/r['rpm_sha256'][:2]/r['rpm_sha256']/r['path'].lstrip('/')
    stem = out / f'{i:04d}'
    cmd = ['readelf','--dyn-syms','--wide','--demangle',str(path)]
    record = dict(command=shlex.join(cmd), start_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    t0=time.monotonic()
    h=hashlib.sha256()
    try:
        with path.open('rb') as f:
            for b in iter(lambda:f.read(1024*1024), b''):
                h.update(b)
    except OSError as e:
        record.update(exitcode='NOT_EXECUTED', error=str(e))
        stem.with_suffix('.json').write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
        db.execute('INSERT INTO elf VALUES (?,?,?,?,?)',(i,json.dumps(r),'NOT_AVAILABLE','',time.monotonic()-t0))
        db.commit()
        continue
    with stem.with_suffix('.stderr').open('wb') as err, gzip.open(stem.with_suffix('.stdout.gz'),'wt') as raw:
        p=subprocess.Popen(cmd,stdout=subprocess.PIPE,stderr=err,text=True,errors='replace')
        for line in p.stdout:
            raw.write(line)
            fields=line.split(None,7)
            if len(fields)!=8 or not fields[0].rstrip(':').isdigit():
                continue
            name=fields[7].strip()
            # 标准库实现和 RTTI 也保留完整原始输出，但候选查询不加载这些噪声。
            if '::' not in name or name.startswith(('std::','typeinfo ','vtable ','VTT ','guard variable ','non-virtual thunk to std::','virtual thunk to std::')):
                continue
            db.execute('INSERT INTO symbol VALUES (?,?,?,?,?)',(i,name,fields[6],fields[4],fields[3]))
        rc=p.wait()
    elapsed=time.monotonic()-t0
    record.update(exitcode=rc,sha256=h.hexdigest(),elapsed_seconds=elapsed,end_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    stem.with_suffix('.json').write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
    db.execute('INSERT INTO elf VALUES (?,?,?,?,?)',(i,json.dumps(r),'PASS' if rc==0 else 'READELF_ERROR',h.hexdigest(),elapsed))
    db.commit()
    if i%25==0:
        print('ELF_DONE',i,'/',len(inventory),'elapsed_seconds',round(time.monotonic()-start,1),flush=True)
    if elapsed>30:
        print('SLOW_IO_OR_PARSE_SECONDS',elapsed,'checkpoint retained; stopping for inspection',flush=True)
        raise SystemExit(22)
summary=dict(expected=len(inventory),completed=db.execute('SELECT COUNT(*) FROM elf').fetchone()[0],
             statuses=dict(db.execute('SELECT status,COUNT(*) FROM elf GROUP BY status')), symbols=db.execute('SELECT COUNT(*) FROM symbol').fetchone()[0],
             elapsed_this_invocation_seconds=time.monotonic()-start,
             limit='x86_64 冻结快照中 cpp_indicator=YES 的运行 ELF；不是全平台所有架构/外部产物；符号同名候选仍须复核真正包边。')
(out/'SUMMARY.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(summary,ensure_ascii=False),flush=True)
db.close()
