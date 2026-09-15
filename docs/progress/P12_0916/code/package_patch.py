#!/usr/bin/env python3
"""由真实 GN/spec 原文生成候选补丁；不应用到平台树。"""
import difflib,pathlib,shutil
base=pathlib.Path(__file__).resolve().parent;root=base.parents[1]
up=base/'upstream/chromium-efl-1.1.144';out=root/'docs/progress/P12_0916/patches'
def diff(path,old,new):return ''.join(difflib.unified_diff(old.splitlines(True),new.splitlines(True),fromfile='a/'+path,tofile='b/'+path))
gn='tizen_src/downloadable/BUILD.gn';old=(up/gn).read_text()
new=old.replace('declare_args() {','declare_args() {\n  ewk_cookie_adaptor_enabled = false\n  ewk_cookie_adaptor_link_inputs = []\n  ewk_cookie_adaptor_runtime_flags = []',1)
anchor='  if (is_samsung_next_browser) {'
insertion='''  if (ewk_cookie_adaptor_enabled) {
    assert(ewk_cookie_adaptor_link_inputs != [])
    defines += [ "EWK_COOKIE_ADAPTOR" ]
    inputs = ewk_cookie_adaptor_link_inputs
    libs += ewk_cookie_adaptor_link_inputs
    ldflags += ewk_cookie_adaptor_runtime_flags
  }
'''
assert new.count(anchor)==1;new=new.replace(anchor,insertion+anchor)
route='tizen_src/downloadable/ewk_adaptor_route.h'
(out/'0002-gn-explicit-link-inputs.patch').write_text(diff(gn,old,new)+diff(route,'',(base/'src/ewk_adaptor_route.h').read_text()))
spec='packaging/chromium-efl.spec';old=(up/spec).read_text()
for mode in ['separate','embedded']:
 new=old
 # The payload is deliberately an explicit external build input. No guessed
 # Source number or toolchain package name is inserted into the platform spec.
 anchor='ninja %{?_ninja_debug:-v -d stats -d explain} %{_smp_mflags} -C "%{OUTPUT_FOLDER}"'
 assert new.count(anchor)==1
 pre='''# Candidate adaptor payload/config must be supplied and reviewed by product packaging.
test -n "%{?ewk_adaptor_payload}" && test -n "%{?ewk_adaptor_config}" || exit 1
test "%{__enable_ewk_interface}" = "1" || exit 1
python3 "%{ewk_adaptor_payload}/package_build.py" "%{ewk_adaptor_config}" "$PWD/p12-adaptor" --mode MODE
cat "$PWD/p12-adaptor/gn.args" >> "%{OUTPUT_FOLDER}/args.gn"
# Ninja regenerates the existing GN graph, then builds this target only.
bash "%{ewk_adaptor_payload}/src/limit_build.sh" ninja -j1 -C "%{OUTPUT_FOLDER}" ewk-interface
'''.replace('MODE',mode)
 new=new.replace(anchor,pre+'\n'+anchor)
 if mode=='separate':
  anchor='    install -m 0644 "%{OUTPUT_FOLDER}"/libchromium-ewk.so    "%{buildroot}"%{_libdir}/'
  assert new.count(anchor)==1
  new=new.replace(anchor,anchor+'\n    install -m 0644 "$PWD/p12-adaptor/libewk-cookie-adaptor.so.1" "%{buildroot}"%{_libdir}/')
  anchor='  %{_libdir}/libchromium-ewk.so\n';assert new.count(anchor)==1
  new=new.replace(anchor,anchor+'  %{_libdir}/libewk-cookie-adaptor.so.1\n')
 (out/('0003-spec-'+mode+'-CANDIDATE.patch')).write_text(diff(spec,old,new))
print('Generated alternative GN/spec candidate diffs, not an RPM build or installation')
