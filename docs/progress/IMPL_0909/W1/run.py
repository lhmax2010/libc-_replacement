#!/usr/bin/env python3
"""三套既有运行库，文件流析构/显式同步各五次；只构建小探针。"""
import csv, hashlib, os, pathlib, re, subprocess, sys
r = pathlib.Path.cwd()
out = r/'docs/progress/IMPL_0909/W1'
tmp = r/'tmp/IMPL_0909/W1'
tmp.mkdir(parents=True, exist_ok=True)
rec = r/'docs/progress/R114/code/record.py'
compiler = '/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
def record(name, command):
    return subprocess.run([sys.executable, str(rec), str(out/'raw'/name), *command]).returncode
gnu = pathlib.Path(subprocess.check_output(['g++','-print-file-name=libstdc++.so.6'],text=True).strip()).resolve()
assert record('002_compilers', ['bash','-lc', 'g++ --version; g++ -print-file-name=libstdc++.so.6; '+compiler+' --version']) == 0
rows = []
for kind, build in [('new','build-native'),('old','build-baseline-native'),('gnu',None)]:
    exe = tmp/kind
    source = out/'overflow_minimal.cpp'
    if build:
        b = r/'tmp/IMPL_0908'/build
        runtime = b/'lib'
        cmd = [compiler, '-std=c++14','-O0','-g','-nostdinc++','-I',str(b/'include/c++/v1'),str(source),
               '-nostdlib++','-L',str(runtime),'-Wl,-rpath-link,'+str(runtime),'-lc++','-lc++abi','-pthread','-o',str(exe)]
    else:
        runtime = gnu.parent
        cmd = ['g++','-std=c++14','-O0','-g',str(source),'-o',str(exe)]
    assert record('build_'+kind, ['nice','-n','15','ionice','-c','3',*cmd]) == 0
    assert record('elf_'+kind, ['readelf','-d',str(exe)]) == 0
    for mode in ['destructor','explicit']:
        for repeat in range(1,6):
            name = f'run_{kind}_{mode}_{repeat}'
            data = tmp/(name+'.dat')
            assert not data.exists()
            code = record(name, ['nice','-n','15','ionice','-c','3','timeout','20','env',
                'LD_LIBRARY_PATH='+str(runtime),str(exe),str(data),mode,str(runtime),'gnu' if kind=='gnu' else 'cxx'])
            text = (out/'raw'/(name+'.stdout')).read_text()
            assert 'RUNTIME_IDENTITY=PASS' in text
            m = re.search(r'mode=(\w+) bytes=(\d+) exact_utf8=(\d) hex=([0-9a-f]*)',text)
            assert m, text
            actual = data.read_bytes()
            assert len(actual)==int(m[2]) and actual.hex()==m[4]
            assert code == (0 if m[3]=='1' else 1)
            if mode=='explicit': assert actual==bytes.fromhex('e4b991e4b992e4b993') and code==0
            rows.append([kind,'x86_64 native',mode,repeat,len(actual),actual.hex(),m[3],code,'raw/'+name+'.stdout'])
    for f in [exe, *(list(runtime.glob('libc++.so.1'))+list(runtime.glob('libc++abi.so.1')) if build else [gnu])]:
        print('SHA256',hashlib.sha256(f.read_bytes()).hexdigest(),f,flush=True)
with (out/'matrix.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['library','architecture','mode','repeat','bytes','hex','exact_utf8','exit','evidence']);w.writerows(rows)
for kind in ['new','old','gnu']:
    for mode in ['destructor','explicit']:
        selected=[x for x in rows if x[0]==kind and x[2]==mode]
        assert len(selected)==5 and len({(x[4],x[5],x[7]) for x in selected})==1
        print('RESULT',kind,mode,'5/5',selected[0][4:8],flush=True)
