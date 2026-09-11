#!/usr/bin/env python3
import pathlib,subprocess,sys,json,hashlib
out=pathlib.Path('docs/progress/API_0911/W2'); dest=out/'sources';dest.mkdir(exist_ok=True)
sources=[
 ('clang_toolchain','https://clang.llvm.org/docs/Toolchain.html'),
 ('gcc_dual_abi','https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html'),
 ('cmake_objects','https://cmake.org/cmake/help/latest/command/add_library.html'),
 ('cmake_options','https://cmake.org/cmake/help/latest/command/target_compile_options.html'),
 ('ld_versions','https://sourceware.org/binutils/docs/ld/VERSION.html'),
 ('llvm_coexistence','https://lists.llvm.org/pipermail/cfe-dev/2016-August/050278.html'),
 ('libcxxrt','https://raw.githubusercontent.com/libcxxrt/libcxxrt/master/README.md'),
 ('boost_convert','https://raw.githubusercontent.com/boostorg/program_options/boost-1.83.0/src/convert.cpp')]
index=[]
for name,url in sources:
    p=dest/(name+'.txt')
    rc=subprocess.run([sys.executable,'docs/progress/API_0911/record.py',str(out/'raw'/('url_'+name)),'curl','--fail','--location','--max-time','40','--output',str(p),'--write-out','%{http_code} %{url_effective}\n',url]).returncode
    index.append(dict(id=name,url=url,exit=rc,sha256=hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else 'NOT_AVAILABLE'))
(out/'sources.json').write_text(json.dumps(index,indent=2)+'\n')
print(json.dumps(index,indent=2))
