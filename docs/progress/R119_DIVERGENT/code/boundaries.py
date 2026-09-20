#!/usr/bin/env python3
"""具体函数身份查询；记录正向控制，候选命中不自动成为新边。"""
from common import *
import sqlite3,collections
gate('boundary_gate');db=ROOT/'tmp/R119_batch1/full_elf/symbols.sqlite3'
con=sqlite3.connect('file:'+str(db)+'?mode=ro',uri=True)
elves={i:dict(metadata=json.loads(meta),sha256=digest) for i,meta,digest in con.execute('select id,metadata,sha256 from elf')}
control_path=ROOT/'docs/progress/R119_batch1/W1/positive/bundle_add.json';control=json.loads(control_path.read_text());name=control['elf_matches'][0]['symbol']['demangled']
found=list(con.execute('select elf_id,ndx from normalized_symbol where name=?',(name,)))
assert any(n=='UND' for i,n in found) and any(n!='UND' for i,n in found)
save(OUT/'BOUNDARY_POSITIVE_CONTROL.json',dict(identity=name,hits=found,passed=True,prior_proof=str(control_path.relative_to(ROOT)),sha256=sha(control_path)))
queries=['ChecksumStream::open(', 'ChecksumStream::flags(', 'PdfInputDevice::Tell(', 'PdfInputDevice::Seek(', 'zypp::InputStream::', 'LineReader::lineStart(', 'WaitUntil(', 'FromChrono(', 'ToChronoSeconds(', 'GetUtilityExpressionTimeout(', 'GetInterruptTimeout(', 'waitForUnlockFor(', 'CreateDirectoryReloaderCrlProvider(', 'getLastModified(', 'mapped_file::open(', 'file_descriptor::open(', 'zypp::xml::Reader::Reader(', 'onert::ir::Coordinates::begin(', 'onert::ir::Coordinates::end(', 'cv::dnn::', 'UnregisterSignal(']
queries += ['FileLoader::ReadFile(', 'FileLoader::GetFileSize(', 'FontFileManager::FindFontFile(', 'FontFileManager::CacheFontFile(', 'get_unichar_from_std_iterator(', 'ArchGetMainThreadId(', 'RCSResourceObject::getLockOwner(', 'RCSResourceObject::setLockOwner(', 'wrapped_exception::wrapped_exception(', 'AsyncPromise::setException(', 'PdfInputDevice::Clear(']
reports=json.loads((OUT/'BOUNDARY_FUNCTION_QUERIES.json').read_text()) if (OUT/'BOUNDARY_FUNCTION_QUERIES.json').exists() else []
done={r['query'] for r in reports}
for token in queries:
 if token in done:continue
 defs=list(con.execute("select elf_id,name,ndx from normalized_symbol where instr(name,?)>0 and ndx!='UND'",(token,)))
 pairs=[]
 for eid,name,ndx in defs:
  for (cid,) in con.execute("select distinct elf_id from normalized_symbol where name=? and ndx='UND'",(name,)):
   if elves[eid]['metadata']['sourcerpm']!=elves[cid]['metadata']['sourcerpm']:
    pairs.append(dict(provider_id=eid,consumer_id=cid,symbol=name,provider=elves[eid],consumer=elves[cid]))
 reports.append(dict(query=token,definitions=[dict(id=eid,name=name,elf=elves[eid]) for eid,name,ndx in defs],other_source_links=pairs))
 print(token,'defs',len(defs),'cross-source',len(pairs),flush=True)
 for pair in pairs:print(json.dumps(pair,ensure_ascii=False),flush=True)
save(OUT/'BOUNDARY_FUNCTION_QUERIES.json',reports)
print('rows',len(reports))
