#!/usr/bin/env python3
"""逐字比较当前与产生全部既有有效记录的编译/链接配置，核对选择集合。"""
import csv,pathlib,re,hashlib
r=pathlib.Path.cwd();o=r/'docs/progress/IMPL_0909/W2';old=r/'docs/progress/IMPL_0908/W3'
def flags(text):
    return {(a,b):c for a,b,c in re.findall(r'\(([^)]+)\) Using (%\{[^}]+\}) substitution: (.*)',text) if b in ('%{cxx}','%{flags}','%{compile_flags}','%{link_flags}')}
current=(o/'raw/028_current_lit_configuration.stderr').read_text()
assert (o/'raw/028_current_lit_configuration.exitcode').read_text().strip()=='0'
base=flags((old/'raw/lit_armv7l_new_full/stderr').read_text())
assert len(base)==8 and flags(current)==base,('CURRENT_COMPILATION_CONFIGURATION_DIFF_STOP',flags(current),base)
assert 'clang-22.1.8' in current and 'optimization=none' in current
with (o/'RESUME_CURRENT_CONFIG.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['suite','field','value','comparison'])
    w.writerows([a,b,c,'EXACT_MATCH'] for (a,b),c in sorted(base.items()))
names=[s.strip() for s in (o/'raw/028_current_lit_configuration.stdout').read_text().splitlines() if ' :: ' in s]
assert len(names)==len(set(names))==11402
rx=re.compile((o/'resume.args').read_text().splitlines()[1]);chosen=[s for s in names if not rx.search(s)]
def key(name):
    suite='libcxxabi' if name.startswith('llvm-libc++abi') else 'libcxx'
    test=name.split(' :: ',1)[1]
    if '/test/' in test:test=test.split('/test/',1)[1]
    return suite,test
expected={(x['suite'],x['test']) for x in csv.DictReader((old/'ARM_UNOBSERVED.tsv').open(),delimiter='\t')}
assert len(chosen)==3554 and set(map(key,chosen))==expected
with (o/'RESUME_SELECTED.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['suite','test','lit_name']);w.writerows([*key(x),x] for x in chosen)
print('CURRENT_VS_PREVIOUS_COMPILER_STANDARD_OPTIMIZATION_LINK_FLAGS=EXACT_MATCH')
print('DISCOVERED=11402 EXCLUDED_VALID=7848 SELECTED_EXACT_MISSING=3554; no suite test executed in discovery')
print('DISCOVERY_FEATURE_PROBES=YES; these are configuration probes, not reruns of the 7848 tests')
print('SCRIPT_SHA256='+hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
