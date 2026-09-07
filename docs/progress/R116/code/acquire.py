#!/usr/bin/env python3
"""固定已观测 Chromium revision 获取配置及其 DEPS 选定的 libc++ 源码。"""
import json,pathlib,subprocess,sys
root=pathlib.Path('docs/progress/R116');head=json.loads((root/'sources/chromium_head.txt').read_text()[4:])['commit']
urls=[]
for name,path in [('cpp_config','build/config/c++/c++.gni'),('cpp_build','build/config/c++/BUILD.gn'),('unwind_config','build/config/unwind.gni'),('libcxx_build','third_party/libc++/BUILD.gn'),('libcxx_config','third_party/libc++/__config_site'),('libcxx_readme','third_party/libc++/README.chromium'),('abi_build','third_party/libc++abi/BUILD.gn'),('unwind_build','third_party/libunwind/BUILD.gn'),('thread','base/threading/thread.cc'),('style','styleguide/c++/c++11.md'),('deps_pinned','DEPS')]:
 urls.append((name,f'https://chromium.googlesource.com/chromium/src/+/{head}/{path}?format=TEXT','base64'))
rev='97b436da4c33663581d394f4ee0a5977fc38c2f4'
for name,path in [('pinned_shared','src/shared_mutex.cpp'),('pinned_shared_header','include/shared_mutex'),('pinned_cv','src/condition_variable.cpp'),('pinned_cv_header','include/__condition_variable/condition_variable.h'),('pinned_wbuffer','include/__locale_dir/wbuffer_convert.h')]:
 urls.append((name,f'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/{rev}/{path}?format=TEXT','base64'))
urls += [('cros_ebuild','https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/refs/heads/main/sys-libs/llvm-libunwind/llvm-libunwind-9999.ebuild?format=TEXT','base64'),
 ('cros_review','https://reviews.llvm.org/D106703?id=361316','text'),
 ('starfish_readme','https://raw.githubusercontent.com/Samsung/Starfish/master/README.md','text'),
 ('starfish_worker','https://raw.githubusercontent.com/Samsung/Starfish/master/src/core/modules/worker/WorkerThread.cpp','text')]
for name,url,kind in urls:
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(root/'raw/acquire'/name),sys.executable,str(root/'code/fetch.py'),name,url,kind])
