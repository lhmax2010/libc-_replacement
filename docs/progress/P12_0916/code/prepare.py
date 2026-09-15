#!/usr/bin/env python3
"""复制真实转发源码与 SDK 声明，在副本机械应用 cookie 路由改动并生成 diff。"""
import difflib,hashlib,json,pathlib,shutil,os
ROOT=pathlib.Path(os.environ.get('P12_WORKSPACE_ROOT',str(pathlib.Path(__file__).resolve().parents[2])))
TMP=pathlib.Path(__file__).resolve().parent
OUT=pathlib.Path(os.environ.get('P12_OUT_DIR',str(ROOT/'docs/progress/P12_0916')))
OUT.mkdir(parents=True,exist_ok=True)
up=TMP/'upstream/chromium-efl-1.1.144'
work=TMP/'forwarder_source'
source=up/'tizen_src/downloadable'
dest=work/'tizen_src/downloadable'
shutil.copytree(source,dest,dirs_exist_ok=True)
ids=json.loads((ROOT/'docs/progress/API_0911/W1_CONT/ewk_header_identity.json').read_text())
header=pathlib.Path(ids[0]['path'])
assert hashlib.sha256(header.read_bytes()).hexdigest()==ids[0]['sha256']
pub=work/'tizen_src/ewk/efl_integration/public'
shutil.copytree(header.parent,pub,dirs_exist_ok=True)
shutil.copyfile(TMP/'src/ewk_adaptor_route.h',dest/'ewk_adaptor_route.h')
p=dest/'ewk_api_wrapper_generator.py'
original=p.read_text()
text=original.replace('DL_MACRO = """','''text_file.write('#ifdef EWK_COOKIE_ADAPTOR\\n#include "ewk_adaptor_route.h"\\n#endif\\n')

DL_MACRO = """
static void* p12_resolve_ewk_function(const char* name) {
#ifdef EWK_COOKIE_ADAPTOR
  if (strcmp(name, "ewk_parse_cookie") == 0)
    return reinterpret_cast<void*>(&p12_ewk_parse_cookie);
#endif
  if (!ewk_interface::g_impl_lib_handle && !ewk_interface::open_library())
    return nullptr;
  return dlsym(ewk_interface::g_impl_lib_handle, name);
}
''')
assert text!=original
lines=text.splitlines(keepends=True)
index=next(i for i,s in enumerate(lines) if 'if (!g_impl_lib_handle && !open_library())' in s)
assert 'return static_cast<return_type>(0);' in lines[index+1]
del lines[index:index+2]
text=''.join(lines)
text=text.replace('dlsym(g_impl_lib_handle, #func_name)','p12_resolve_ewk_function(#func_name)')
assert 'if (!g_impl_lib_handle && !open_library())' not in text
assert text.count('p12_resolve_ewk_function(#func_name)')==1
p.write_text(text)
patch=''.join(difflib.unified_diff(original.splitlines(True),text.splitlines(True),
    fromfile='a/tizen_src/downloadable/ewk_api_wrapper_generator.py',tofile='b/tizen_src/downloadable/ewk_api_wrapper_generator.py'))
(OUT/'patches').mkdir(exist_ok=True)
(OUT/'patches/0001-route-cookie-to-explicit-adaptor.patch').write_text(patch)
print('REAL_SOURCE',source)
print('PATCHED_COPY',dest)
print('PATCH',OUT/'patches/0001-route-cookie-to-explicit-adaptor.patch')
