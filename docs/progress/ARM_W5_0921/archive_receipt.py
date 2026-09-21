import pathlib,shutil
root=pathlib.Path('/home/toolchain/development/libc++_replacement')
source=root/'progress/ARM_LZMA_0921B/raw'
dest=root/'docs/progress/ARM_W5_0921/raw'
for n in range(150,156):
    files=sorted(source.glob(f'{n}_*'))
    assert files
    for p in files:
        q=dest/p.name
        assert not q.exists()
        shutil.copy2(p,q)
        assert p.read_bytes()==q.read_bytes()
print('Publication operation records copied and byte-verified.')
