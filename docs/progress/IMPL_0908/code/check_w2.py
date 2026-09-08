#!/usr/bin/env python3
"""检查真实 DSO 的版本默认值、helper 隐藏及编译警告。"""
import pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W2/raw')
s=(r/'006_dynsyms.stdout').read_text();v=(r/'005_versions.stdout').read_text()
for name in ['4wait','15__do_timed_wait']:
 lines=[x for x in s.splitlines() if 'condition_variable'+name in x]
 assert len(lines)==2 and any('@@LLVM_22_TIZEN_1' in x for x in lines) and any('@LLVM_22\n' in x+'\n' for x in lines)
 print('TWO_VERSIONS_NEW_DEFAULT=PASS',name)
assert 'Name: LLVM_22_TIZEN_1' in v and 'Parent 1: LLVM_22' in v
pattern=r'__libcpp_tizen_cv_\w+'
source=pathlib.Path('tmp/IMPL_0908/source/libcxx/src/condition_variable.cpp').read_text()
assert len(set(re.findall(pattern,source)))==4
assert not re.findall(pattern,s)
assert 'condition_variable10notify_one' in s
print('HELPER_DYNAMIC_EXPORTS=0 POSITIVE_CONTROL_SOURCE_NAMES=4 EXPORTED_NOTIFY=PASS')
build=(r/'004_build.stdout').read_text()+(r/'004_build.stderr').read_text()
assert (r/'004_build.exitcode').read_text().strip()=='0'
assert not re.search(r'\b(?:warning|error):',build)
control=pathlib.Path('docs/progress/IMPL_0908/W1/raw/024_build_baseline.stdout').read_text()
assert re.search(r'\bwarning:',control)
print('BUILD_COMPILER_WARNINGS=0 POSITIVE_CONTROL=PASS')
