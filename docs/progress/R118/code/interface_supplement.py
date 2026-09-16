#!/usr/bin/env python3
import subprocess,sys
from run import ROOT,CODE
for typ,tests in [('string',['cxx23_from_range_extension','cxx23_append_range_extension','cxx23_common','gnu_overwrite_extension']),('vector',['cxx23_from_range_extension','cxx23_append_range_extension'])]:
 for name in tests:
  for arch in ['x86_64','armv7l']:
   subprocess.run([sys.executable,str(CODE/'run.py'),typ,'--arch',arch,'--only',name],cwd=ROOT,check=True)
