#!/usr/bin/env python3
"""精确保留旧有效 7848 项，正向验证排除式不会排除缺失 3554 项。"""
import csv,pathlib,re
r=pathlib.Path.cwd();old=r/'docs/progress/IMPL_0908/W3';out=r/'docs/progress/IMPL_0909/W2'
good=[x['test'] for x in csv.DictReader((old/'ARM_RESULT_SELECTION.tsv').open(),delimiter='\t')]
missing=list(csv.DictReader((old/'ARM_UNOBSERVED.tsv').open(),delimiter='\t'))
assert len(set(good))==7848 and len(missing)==3554
pattern='^(?:'+'|'.join(map(re.escape,good))+')$';rx=re.compile(pattern)
assert all(rx.search(x) for x in good)
for x in missing:
    prefix='llvm-libc++-shared.cfg.in' if x['suite']=='libcxx' else 'llvm-libc++abi-shared.cfg.in'
    assert not rx.search(prefix+' :: '+x['test'])
(out/'resume.args').write_text('--filter-out\n'+pattern+'\n')
print('PRESERVE=7848 EXPECTED_REMAINDER=3554 EXCLUSION_POSITIVE_CONTROL=PASS')
