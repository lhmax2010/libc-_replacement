"""Cross-reference queried binary RPM sources against the unchanged R101 universe."""
import csv,hashlib,json
from pathlib import Path
P=Path.cwd();E=P/'progress/QB_PRECHECK_0922';F=P/'docs/progress/R101/tables/base_all_source_packages.tsv'
data=list(csv.DictReader(F.open(),delimiter='\t'));by={r['源码包名']:(i+2,r) for i,r in enumerate(data)};rows=[]
for arch in ('armv7l','aarch64'):
 for line in (E/f'roots/{arch}-static-packages.out').read_text().splitlines():
  name,vr,a,src,vendor,distro,btime=line.split('\t');source=src.removesuffix('.src.rpm').rsplit('-',2)[0]
  n,r=by.get(source,(None,{}));assert source in by
  rows.append(dict(arch=arch,binary=name,version_release=vr,source_rpm=src,source=source,r101_line=n,r101_decision=r.get('判定结果','NOT_AVAILABLE'),current_status='ADAPTED_PUSHED' if source=='bcc-tools' else 'NO_CHANGE_NEEDED' if r.get('判定结果')=='不需要改' else 'NEEDS_REVIEW',vendor=vendor,distribution=distro,repository_limit='RPM metadata only; current OBS availability NOT_OBSERVED'))
(E/'STATIC_DEPENDENCIES.json').write_text(json.dumps(dict(r101_sha256=hashlib.sha256(F.read_bytes()).hexdigest(),rows=rows),ensure_ascii=False,indent=2))
with (E/'STATIC_DEPENDENCIES.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=rows[0].keys(),delimiter='\t');w.writeheader();w.writerows(rows)
print(json.dumps(rows,ensure_ascii=False,indent=2))
