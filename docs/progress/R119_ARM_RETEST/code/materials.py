#!/usr/bin/env python3
from common import *
import xml.etree.ElementTree as ET
gate('materials_gate')
base=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
wanted={'dali2-integration-devel','dali2-devel','dali2','dali2-ui-foundation','libzypp-devel','libzypp','zypper','boost-devel','libxml2-devel'}
rows=[]
for repo in ['base_binary','unified_binary']:
 for event,p in ET.iterparse(gzip.open(base/'repodata'/f'{repo}.primary.xml.gz','rb'),events=['end']):
  if p.tag!='{http://linux.duke.edu/metadata/common}package':continue
  name=p.findtext('{*}name');arch=p.findtext('{*}arch')
  if name in wanted and arch in ['armv7l','aarch64']:
   digest=p.findtext('{*}checksum');v=p.find('{*}version');loc=p.find('{*}location');root=base/'extracted'/digest[:2]/digest
   r=dict(repo=repo,name=name,arch=arch,version=v.attrib,rpm_sha256=digest,location=loc.attrib['href'],extracted=str(root),exists=root.is_dir());rows.append(r);print(json.dumps(r))
  p.clear()
save(OUT/'TARGET_PACKAGE_MATERIALS.json',rows)
