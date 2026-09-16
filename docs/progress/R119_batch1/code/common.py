"""复用已记录工具链；所有新输出隔离到本任务。"""
import pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT/'docs/progress/R118/code'))
import run as inherited
inherited.OUT = ROOT/'docs/progress/R119_batch1'
from run import configuration, record, checked, digest, save, OLD, CC, SDK
OUT = inherited.OUT
TMP = ROOT/'tmp/R119_batch1'
SOURCE = OLD/'tmp/corpus/extracted/lightweight-web-engine-1.4.0-1.src.rpm/unpacked/lightweight-web-engine-1.4.0.tar.gz/lightweight-web-engine-1.4.0'

def excerpt(relative, start, end):
    p=SOURCE/relative
    print(f'FILE {p} SHA256 {digest(p)}')
    for n,line in enumerate(p.read_text().splitlines(),1):
        if start<=n<=end: print(f'{n}: {line}')
