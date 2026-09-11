#!/usr/bin/env python3
import pathlib,sys,json
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[1]))
from common import ROOT as r,record
old=r/'tmp/API_0911/W1_CONT';t=r/'tmp/ADAPTOR_0912/W1';src=t/'source/chromium-efl-1.1.144';b=r/'tmp/IMPL_0908/build-native'
incs=['-I'+str(x) for x in [src,old/'chromium_extension_source/chromium-efl-1.1.144',old/'chromium_source/chromium-efl-1.1.144',src/'tizen_src/ewk/efl_integration',old/'chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration']]
incs+=['-I'+p for p in sorted({str(pathlib.Path(x['path']).parent) for x in json.loads((r/'docs/progress/API_0911/W1_CONT/ewk_header_identity.json').read_text())})]
cc=['/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++','-std=c++20','-fPIC','-O0','-nostdinc++','-I'+str(b/'include/c++/v1'),*incs]
files=[old/'chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc',old/'chromium_extension_source/chromium-efl-1.1.144/tizen_src/chromium_impl/components/xwalk_extensions/browser/xwalk_extension.cc']
for f in files:record('W1','component_'+f.stem,[*cc,'-c',f,'-o',t/(f.stem+'.o')])
record('W1','023_build_graph',['rg','-n','ewk_cookie_parser|source_set\(|shared_library\(|static_library\(|use_custom_libcxx|is_component_build|xwalk_extensions|//net|//base|mojom',src/'tizen_src/ewk',src/'tizen_src/chromium_impl/components/xwalk_extensions',src/'build/config/c++',src/'net/BUILD.gn'])
record('W1','024_offscreen_impl',['bash','-c','nl -ba /home/toolchain/development/libc++_replacement/tmp/corpus/extracted/dali2-adaptor-2.5.26-1.src.rpm/unpacked/dali2-adaptor-2.5.26.tar.gz/dali2-adaptor-2.5.26/dali/internal/offscreen/common/offscreen-application-impl.cpp | sed -n "35,250p"'])
