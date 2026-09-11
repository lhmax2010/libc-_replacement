#!/usr/bin/env python3
"""按源文本 typed catch 句法点重数；非实例数、非保护站点数、不求条件编译可达性。"""
import collections,csv,hashlib,pathlib,re,subprocess
out=pathlib.Path('docs/progress/P4_0909')
pattern=re.compile(r'\b(?:__catch|catch)\s*\([^)]*\b__forced_unwind\b[^)]*\)',re.S)
strip=re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)
def clean(s):return strip.sub(lambda m:''.join('\n' if c=='\n' else ' ' for c in m.group()),s)
control='__catch (\n const __cxxabiv1::__forced_unwind &e) {}\ncatch(abi::__forced_unwind&){}\n// catch(abi::__forced_unwind&)\n"catch(abi::__forced_unwind&)"'
assert len(list(pattern.finditer(clean(control))))==2
print('POSITIVE_CONTROL_MULTILINE_MACRO_AND_PLAIN=2; COMMENT_STRING_NEGATIVE=PASS')
rows=[]; inputs=[]
for label,root,parts in [('gcc','codes/gcc',['libstdc++-v3/include','libstdc++-v3/src','libstdc++-v3/libsupc++']),
                         ('llvm_base','codes/llvm',['libcxx/include','libcxx/src'])]:
    files=subprocess.check_output(['git','-C',root,'ls-files','-z','--',*parts]).decode().split('\0')
    for name in filter(None,files):
        p=pathlib.Path(root)/name
        if not p.is_file():continue
        data=p.read_bytes(); inputs.append([label,str(p),hashlib.sha256(data).hexdigest()])
        text=data.decode(errors='replace')
        for m in pattern.finditer(clean(text)):
            rows.append([label,str(p),text[:m.start()].count('\n')+1,' '.join(text[m.start():m.end()].split())])
    print(label,'HEAD',subprocess.check_output(['git','-C',root,'rev-parse','HEAD']).decode().strip())
    subset=[r for r in rows if r[0]==label]
    print(label,'HANDLER_SYNTAX_POINTS',len(subset),'FILES',len({r[1] for r in subset}))
    print('BY_SOURCE_SUBTREE',dict(collections.Counter('/'.join(r[1].split('/')[3:5]) for r in subset)))
for name,header,data in [('HANDLER_SITES.tsv',['tree','file','line','catch_clause'],rows),
                         ('HANDLER_INPUTS.tsv',['tree','file','sha256'],inputs)]:
    with (out/name).open('w',newline='') as f:
        w=csv.writer(f,delimiter='\t');w.writerow(header);w.writerows(data)
print('SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
