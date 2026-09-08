#!/usr/bin/env python3
"""以各架构真实旧库成员名为对照，不用一份硬编码的目标名验证自己。"""
import pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3/raw')
for arch,old,new in [('x86_64','034_old_native_dynsyms','032_fixed_native_dynsyms'),('armv7l','024_arm_baseline_dynsyms','033_fixed_arm_dynsyms')]:
 before=(r/(old+'.stdout')).read_text();after=(r/(new+'.stdout')).read_text()
 for part in ['condition_variable4wait','condition_variable15__do_timed_wait']:
  base=[s.split()[-1].split('@')[0] for s in before.splitlines() if part in s]
  assert len(base)==1
  actual=[s.split()[-1] for s in after.splitlines() if part in s]
  assert set(actual)=={base[0]+'@LLVM_22',base[0]+'@@LLVM_22_TIZEN_1'},(arch,actual)
  print(arch,'EXACT_BASELINE_NAME_AND_NEW_DEFAULT=PASS',base[0])
 assert not re.search(r'__libcpp_tizen_cv_\w+',after)
 assert re.search(r'__libcpp_tizen_cv_\w+',pathlib.Path('tmp/IMPL_0908/source/libcxx/src/condition_variable.cpp').read_text())
 print(arch,'HELPER_DYNAMIC_EXPORTS=0 POSITIVE_CONTROL=PASS')
oldbad=(r/'011_arm_dynsyms.stdout').read_text()
assert 'ratioILl1ELl1000000000' in oldbad and 'ratioILx1ELx1000000000' in (r/'033_fixed_arm_dynsyms.stdout').read_text()
print('INITIAL_ARM_DEFECT_REPRODUCED_AND_CORRECTED=PASS')
