#!/usr/bin/env python3
"""只读验证安装副本是否恰好等于删除 RUNPATH 的构建物；不写二进制。"""
import hashlib,pathlib,struct
p=pathlib.Path('tmp/IMPL_0908/build-armv7l')
a=(p/'lib/libc++.so.1').read_bytes();b=(p/'libcxx/test-suite-install/lib/libc++.so.1').read_bytes()
assert a[:6]==b'\x7fELF\x01\x01' and len(a)==len(b)
phoff=struct.unpack_from('<I',a,28)[0];phsize,phnum=struct.unpack_from('<HH',a,42)
segments=[struct.unpack_from('<8I',a,phoff+i*phsize) for i in range(phnum)]
dyn=next(x for x in segments if x[0]==2);off,size=dyn[1],dyn[4]
entries=[]
for i in range(off,off+size,8):
    tag,val=struct.unpack_from('<II',a,i)
    entries.append((tag,val,i))
    if tag==0:break
run=next(x for x in entries if x[0]==29)
straddr=next(x[1] for x in entries if x[0]==5)
load=next(x for x in segments if x[0]==1 and x[2]<=straddr<x[2]+x[4])
start=load[1]+straddr-load[2]+run[1];end=a.index(b'\0',start)
expected=bytearray(a);expected[start:end]=b'\0'*(end-start)
expected[run[2]:entries[-1][2]]=a[run[2]+8:entries[-1][2]+8]
assert bytes(expected)==b,'DIFFERENCE_BEYOND_RUNPATH: STOP'
print('EXACT_BYTE_COMPARISON=PASS: only RUNPATH string zeroing and dynamic-entry removal')
print('REMOVED_RUNPATH='+a[start:end].decode())
print('BUILD_SHA256='+hashlib.sha256(a).hexdigest())
print('INSTALLED_SHA256='+hashlib.sha256(b).hexdigest())
print('BINARY_WRITES=NONE; this does not assert byte-identical files')
