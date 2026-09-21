"""Verify separate symbols against the deployed ELF debuglink CRC, read-only."""
import hashlib,json,pathlib,struct,zlib
root=pathlib.Path('tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0')
debug=pathlib.Path('tmp/ARM_LZMA_0921B/debug-root/usr/lib/debug/usr/lib')
rows=[]
for name in ('librpmio.so.3.1.0','librpm.so.3.1.0','librpmbuild.so.3.1.0'):
    p=root/'usr/lib'/name; data=p.read_bytes()
    assert data[:6]==b'\x7fELF\x01\x01'
    shoff=struct.unpack_from('<I',data,32)[0]
    entsize,count,stridx=struct.unpack_from('<HHH',data,46)
    sections=[struct.unpack_from('<10I',data,shoff+i*entsize) for i in range(count)]
    st=sections[stridx];strings=data[st[4]:st[4]+st[5]]
    sec=next(s for s in sections if strings[s[0]:].split(b'\0',1)[0]==b'.gnu_debuglink')
    link=data[sec[4]:sec[4]+sec[5]]; filename=link.split(b'\0',1)[0]
    expected=struct.unpack_from('<I',link,(len(filename)+4)//4*4)[0]
    q=debug/filename.decode();actual=zlib.crc32(q.read_bytes())
    row=dict(elf=str(p),debug=str(q),expected_crc32=hex(expected),actual_crc32=hex(actual),matched=expected==actual,
             elf_sha256=hashlib.sha256(data).hexdigest(),debug_sha256=hashlib.sha256(q.read_bytes()).hexdigest())
    rows.append(row);assert expected==actual,row
out=pathlib.Path('progress/ARM_LZMA_0921B/SYMBOL_IDENTITY.json')
out.write_text(json.dumps(rows,indent=2)+'\n');print(out.read_text())
