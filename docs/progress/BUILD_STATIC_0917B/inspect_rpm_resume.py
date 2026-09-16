import native_equivalence as e

e.run('native-resume-rpmbuild-help',e.base()+['--','rpmbuild','--help'])
e.run('native-resume-clang-elf',['readelf','-d',str(e.EMUL/'usr/bin/clang-22')])
e.run('native-resume-ninja-file',['file',str(e.EMUL/'usr/bin/ninja')])
