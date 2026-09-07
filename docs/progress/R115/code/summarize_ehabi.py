#!/usr/bin/env python3
import pathlib,re,csv,subprocess,sys
rows=[]
for path in sorted(pathlib.Path('tmp/R115-W3/angle-extracted/usr/lib/driver').glob('*_angle.so')):
 stem='docs/progress/R115/W3/raw/angle/'+path.name
 subprocess.run([sys.executable,'docs/progress/R114/code/record.py',stem,'readelf','-uW',str(path)],check=True)
 data=pathlib.Path(stem+'.stdout').read_text()
 entries=sum(map(int,re.findall(r'contains (\d+) entries',data)))
 cant=data.count('[cantunwind]')
 rows.append([str(path),entries,cant,entries-cant,stem])
with open('docs/progress/R115/W3/angle_exidx.tsv','x') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['ELF','exidx条数','CANTUNWIND条数','其余条数','证据']);w.writerows(rows)
for row in rows:print(*row,sep='\t')
