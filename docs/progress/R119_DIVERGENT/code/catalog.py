#!/usr/bin/env python3
"""从实际头定义中选取公开别名族，并登记有限具体实例；不声称穷尽模板参数空间。"""
from common import *
import re
gate('catalog_gate')
GNU=ROOT/'codes/gcc/libstdc++-v3/include';CXX=ROOT/'codes/llvm/libcxx/include'
families={
'ios':(['bits/postypes.h','bits/ios_base.h','bits/char_traits.h','std/iosfwd'],['__fwd/ios.h','__fwd/fstream.h','__fwd/sstream.h','__fwd/streambuf.h','ios','__ios/fpos.h','__string/char_traits.h']),
'integers':(['c_global/cstdint','c_global/cstddef'],['cstdint','cstddef','__cstddef/size_t.h','__cstddef/ptrdiff_t.h','__cstddef/max_align_t.h','__cstddef/nullptr_t.h']),
'string':(['bits/basic_string.h','bits/stringfwd.h','std/string_view'],['string','string_view','__fwd/string.h','__fwd/string_view.h']),
'vector':(['bits/stl_vector.h','bits/stl_bvector.h'],['vector','__vector/vector.h','__vector/vector_bool.h']),
'deque':(['bits/stl_deque.h'],['deque']),
'list':(['bits/stl_list.h','bits/forward_list.h'],['list','forward_list']),
'array':(['std/array'],['array']),
'ordered':(['bits/stl_map.h','bits/stl_multimap.h','bits/stl_set.h','bits/stl_multiset.h','bits/stl_tree.h'],['map','set','__tree']),
'unordered':(['bits/unordered_map.h','bits/unordered_set.h','bits/hashtable.h'],['unordered_map','unordered_set','__hash_table']),
'adaptors':(['bits/stl_queue.h','bits/stl_stack.h'],['queue','stack']),
'iterators':(['bits/stl_iterator.h','bits/stl_iterator_base_types.h','bits/stream_iterator.h','bits/streambuf_iterator.h'],['iterator','__iterator/iterator_traits.h','__iterator/reverse_iterator.h','__iterator/move_iterator.h','__iterator/back_insert_iterator.h','__iterator/front_insert_iterator.h','__iterator/insert_iterator.h','__iterator/istream_iterator.h','__iterator/ostream_iterator.h','__iterator/istreambuf_iterator.h','__iterator/ostreambuf_iterator.h']),
'chrono':(['bits/chrono.h'],['chrono','__chrono/duration.h','__chrono/system_clock.h','__chrono/steady_clock.h','__chrono/high_resolution_clock.h']),
'random':(['bits/random.h'],['random','__random/default_random_engine.h','__random/linear_congruential_engine.h','__random/mersenne_twister_engine.h','__random/subtract_with_carry_engine.h','__random/discard_block_engine.h','__random/shuffle_order_engine.h','__random/random_device.h']),
'threads':(['bits/std_thread.h','bits/std_mutex.h','std/mutex','std/shared_mutex','std/condition_variable'],['thread','__thread/thread.h','mutex','__mutex/mutex.h','__mutex/recursive_mutex.h','shared_mutex','condition_variable','__condition_variable/condition_variable.h']),
'filesystem':(['bits/fs_path.h','bits/fs_fwd.h','bits/fs_dir.h'],['filesystem','__filesystem/path.h','__filesystem/file_time_type.h','__filesystem/directory_iterator.h','__filesystem/recursive_directory_iterator.h']),
'functional':(['std/functional','bits/std_function.h'],['functional','__functional/bind.h','__functional/function.h','__functional/reference_wrapper.h']),
'memory':(['bits/unique_ptr.h','bits/shared_ptr.h','bits/alloc_traits.h','bits/ptr_traits.h','std/memory'],['memory','__memory/unique_ptr.h','__memory/shared_ptr.h','__memory/allocator_traits.h','__memory/pointer_traits.h']),
'atomic':(['std/atomic'],['atomic','__atomic/aliases.h','__atomic/atomic.h']),
'regex':(['bits/regex.h','bits/regex_constants.h'],['regex']),
'other':(['../libsupc++/exception_ptr.h','std/typeindex','../libsupc++/initializer_list','c_global/cmath'],['exception','__exception/exception_ptr.h','typeindex','initializer_list','cmath'])
}
files={};missing=[]
for family,(gnu,cxx) in families.items():
 files[family]={}
 for lib,root,rels in [('gnu',GNU,gnu),('cxx',CXX,cxx)]:
  files[family][lib]=[]
  for rel in rels:
   p=root/rel
   if not p.is_file():missing.append(str(p.relative_to(ROOT)));continue
   lines=p.read_text(errors='replace').splitlines();files[family][lib].append((p,lines))
catalog=[];seen=set();evidence={}
def add(expr,family,token,kind='type',note=''):
 if expr in seen:return
 refs={}
 for lib,srcs in files[family].items():
  hits=[]
  for p,lines in srcs:
   for i,line in enumerate(lines):
    if re.search(r'\b'+re.escape(token)+r'\b',line) and (re.search(r'\b(typedef|using|class|struct|enum|constexpr|static|const)\b',line) or token in ['fpos','npos']):
     hits.append(dict(path=str(p.relative_to(ROOT)),sha256=sha(p),line=i+1,text=line,context='\n'.join(lines[max(0,i-2):i+3])))
  refs[lib]=hits
 if not any(refs.values()):return
 idx=f'T{len(catalog)+1:04}';seen.add(expr)
 catalog.append(dict(id=idx,expression=expr,family=family,kind=kind,source_token=token,note=note,gnu_definition_hits=len(refs['gnu']),cxx_definition_hits=len(refs['cxx'])))
 evidence[idx]=refs
for n in ['streamoff','streamsize','streampos','wstreampos','u16streampos','u32streampos']:add('std::'+n,'ios',n)
add('std::fpos<std::mbstate_t>','ios','fpos',note='明确实例 mbstate_t；不枚举任意 StateT')
for n in ['fmtflags','iostate','openmode','seekdir']:add('std::ios_base::'+n,'ios',n)
for char in ['char','wchar_t','char16_t','char32_t']:
 for m in ['char_type','int_type','off_type','pos_type','state_type']:add(f'std::char_traits<{char}>::{m}','ios',m)
names=set()
for lib,srcs in files['integers'].items():
 for p,lines in srcs:
  for line in lines:
   names.update(re.findall(r'\b(?:u?int(?:(?:_least|_fast)?(?:8|16|32|64)|max|ptr)_t|size_t|ptrdiff_t|nullptr_t|max_align_t)\b',line))
for n in sorted(names):add('std::'+n,'integers',n)
containers={'string':['std::string','std::wstring','std::u16string','std::u32string','std::string_view','std::wstring_view'], 'vector':['std::vector<int>','std::vector<bool>'], 'deque':['std::deque<int>'], 'list':['std::list<int>','std::forward_list<int>'], 'array':['std::array<int,3>'], 'ordered':['std::map<int,int>','std::multimap<int,int>','std::set<int>','std::multiset<int>'], 'unordered':['std::unordered_map<int,int>','std::unordered_multimap<int,int>','std::unordered_set<int>','std::unordered_multiset<int>'], 'adaptors':['std::stack<int>','std::queue<int>','std::priority_queue<int>']}
members=['size_type','difference_type','pointer','const_pointer','reference','const_reference','value_type','iterator','const_iterator','reverse_iterator','const_reverse_iterator','local_iterator','const_local_iterator','node_type']
for family,instances in containers.items():
 for inst in instances:
  for member in members:add(inst+'::'+member,family,member,note='固定默认 allocator/comparator/hash 实例；不外推自定义模板参数')
  if family=='string':add('decltype('+inst+'::npos)',family,'npos','constant_type')
for inst in ['std::iterator_traits<int*>','std::iterator_traits<std::vector<int>::iterator>','std::reverse_iterator<int*>','std::move_iterator<int*>','std::back_insert_iterator<std::vector<int>>','std::front_insert_iterator<std::list<int>>','std::insert_iterator<std::vector<int>>','std::istream_iterator<int>','std::ostream_iterator<int>','std::istreambuf_iterator<char>','std::ostreambuf_iterator<char>']:
 for member in ['value_type','difference_type','pointer','reference','iterator_category']:add(inst+'::'+member,'iterators',member)
for n in ['system_clock','steady_clock','high_resolution_clock']:
 for member in ['rep','period','duration','time_point']:add(f'std::chrono::{n}::{member}','chrono',member)
for n in ['nanoseconds','microseconds','milliseconds','seconds','minutes','hours']:
 add('std::chrono::'+n,'chrono',n)
 add('std::chrono::'+n+'::rep','chrono','rep')
for n in ['minstd_rand0','minstd_rand','mt19937','mt19937_64','ranlux24_base','ranlux48_base','ranlux24','ranlux48','knuth_b','default_random_engine']:
 add('std::'+n,'random',n);add('std::'+n+'::result_type','random','result_type')
add('std::random_device::result_type','random','result_type')
for inst in ['std::thread','std::mutex','std::recursive_mutex','std::timed_mutex','std::recursive_timed_mutex','std::shared_mutex','std::condition_variable']:
 add(inst+'::native_handle_type','threads','native_handle_type')
add('std::thread::id','threads','id')
for member in ['value_type','string_type','iterator','const_iterator']:add('std::filesystem::path::'+member,'filesystem',member)
add('std::filesystem::file_time_type','filesystem','file_time_type')
for i in range(1,11):add(f'typename std::decay<decltype(std::placeholders::_{i})>::type','functional',f'_{i}','constant_projection')
for inst in ['std::unique_ptr<int>','std::unique_ptr<int[]>','std::shared_ptr<int>','std::weak_ptr<int>','std::allocator_traits<std::allocator<int>>','std::pointer_traits<int*>']:
 for member in ['pointer','const_pointer','void_pointer','const_void_pointer','element_type','value_type','size_type','difference_type']:add(inst+'::'+member,'memory',member)
for lib,srcs in files['atomic'].items():
 for p,lines in srcs:
  for line in lines:
   for name in re.findall(r'\batomic_(?:[a-zA-Z0-9_]+)\b',line):
    if name in ['atomic_flag','atomic_ref','atomic_wait','atomic_notify_one','atomic_notify_all']:continue
    if ('typedef' in line or 'using' in line):add('std::'+name,'atomic',name)
for inst in ['std::regex','std::wregex','std::cmatch','std::smatch','std::csub_match','std::ssub_match']:
 for member in ['size_type','difference_type','value_type','string_type','iterator','const_iterator','char_type','flag_type']:add(inst+'::'+member,'regex',member)
for n in ['exception_ptr','type_index','float_t','double_t']:add('std::'+n,'other',n)
for member in ['value_type','reference','const_reference','size_type','iterator','const_iterator']:add('std::initializer_list<int>::'+member,'other',member)
save(OUT/'CATALOG.json',catalog);tsv(OUT/'CATALOG.tsv',catalog);save(OUT/'CATALOG_SOURCE_EVIDENCE.json',evidence)
save(OUT/'CATALOG_HEADER_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for family in files.values() for srcs in family.values() for p,lines in srcs})
save(OUT/'CATALOG_MISSING_HEADER_PATHS.json',missing)
print('catalog',len(catalog));print('missing header paths',missing)
print('families',dict(__import__('collections').Counter(r['family'] for r in catalog)))
