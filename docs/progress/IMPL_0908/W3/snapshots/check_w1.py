#!/usr/bin/env python3
"""源码范围、正常通知规格与成功构建日志的正/负向核查。"""
import pathlib,re,subprocess,hashlib,json
r=pathlib.Path('docs/progress/IMPL_0908');src=pathlib.Path('tmp/IMPL_0908/source');base='c3f8578a4db871d9d6de96d751f4c2ea7b1638fa'
for rel in ['libcxx/include/__condition_variable/condition_variable.h','libcxx/include/__cxx03/__condition_variable/condition_variable.h']:
 text=(src/rel).read_text();prior=subprocess.check_output(['git','-C',str(src),'show',base+':'+rel]).decode()
 assert 'void wait(unique_lock<mutex>& __lk) _NOEXCEPT;' in prior
 assert 'void wait(unique_lock<mutex>& __lk);' in text
 assert 'void notify_one() _NOEXCEPT;' in text and 'void notify_all() _NOEXCEPT;' in text
 pattern=r'__do_timed_wait\([^;{]*_NOEXCEPT'
 assert len(re.findall(pattern,prior))==5
 assert not re.findall(pattern,text)
 print('WAIT_SPEC_AND_POSITIVE_CONTROL=PASS',rel)
text=(src/'libcxx/include/__locale_dir/wbuffer_convert.h').read_text()
prior=subprocess.check_output(['git','-C',str(src),'show',base+':libcxx/include/__locale_dir/wbuffer_convert.h']).decode()
assert re.search(r'^  __close\(\);$',prior,re.M) and not re.search(r'^  __close\(\);$',text,re.M)
assert '::__close() {' in text and '&& sync()' in text
print('DESTRUCTOR_CALL_REMOVED_HELPER_RETAINED=PASS POSITIVE_CONTROL=PASS')
assert 'clang++: warning:' in (r/'W1/raw/024_build_baseline.stdout').read_text()
print('WARNING_SCAN_POSITIVE_CONTROL=PASS')
for stem in ['030_build_strict','032_baseline_strict']:
 p=r/'W1/raw'/stem
 assert pathlib.Path(str(p)+'.exitcode').read_text().strip()=='0'
 text=pathlib.Path(str(p)+'.stdout').read_text()+pathlib.Path(str(p)+'.stderr').read_text()
 assert not re.search(r'\b(?:warning|error):',text),stem
 print('COMPILER_WARNINGS=0 BUILD=PASS',stem)
for build in ['build-native','build-baseline-native']:
 cache=pathlib.Path('tmp/IMPL_0908',build,'CMakeCache.txt').read_text()
 assert 'CXX_SUPPORTS_WERROR_FLAG:INTERNAL=1' in cache
 print('WERROR_ACCEPTED=PASS',build)
print('W1_STATIC_BUILD_GATE=PASS')
