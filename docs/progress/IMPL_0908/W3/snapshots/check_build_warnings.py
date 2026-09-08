#!/usr/bin/env python3
"""匹配完整构建日志，不把 requested WERROR 等同于 accepted。"""
import collections,pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3/raw')
def warnings(stem):
 assert (r/(stem+'.exitcode')).read_text().strip()=='0'
 text=(r/(stem+'.stdout')).read_text()+(r/(stem+'.stderr')).read_text()
 return collections.Counter(re.findall(r'^.*\bwarning:.*$',text,re.M))
baseline=warnings('013_build_baseline_arm');initial=warnings('005_build_arm');fixed=warnings('027_arm_alias_fix_build')
assert baseline # positive control for the same warning-line detector
assert set(initial)<=set(baseline) and set(fixed)<=set(baseline)
print('ARM_BASELINE_WARNINGS',dict(baseline));print('ARM_INITIAL_WARNINGS',dict(initial));print('ARM_FIX_WARNINGS',dict(fixed))
print('ARM_NEW_WARNING_TEXTS=0 POSITIVE_CONTROL=BASELINE_WARNING_LINES')
native=warnings('028_native_alias_fix_build');assert not native
print('NATIVE_FIX_COMPILER_WARNINGS=0')
for kind in ['build-native','build-armv7l','build-baseline-armv7l']:
 text=pathlib.Path('tmp/IMPL_0908',kind,'CMakeCache.txt').read_text()
 accepted='CXX_SUPPORTS_WERROR_FLAG:INTERNAL=1' in text
 print(kind,'WERROR_ACCEPTED',accepted)
 if kind=='build-native':assert accepted
