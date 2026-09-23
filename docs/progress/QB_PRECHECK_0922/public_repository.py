"""Read the public Base-Toolchain reference index; no authentication/config changes."""
import hashlib,json,shlex,subprocess
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urljoin
P=Path.cwd();E=P/'progress/QB_PRECHECK_0922/public-repository';E.mkdir(exist_ok=False)
url='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/'
argv=['curl','-sS','--max-time','30','-L','-D',str(E/'reference.headers'),'-o',str(E/'reference.html'),'-w','%{http_code} %{url_effective}\n',url]
r=subprocess.run(argv,capture_output=True,text=True);(E/'request.json').write_text(json.dumps(dict(command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));print(r.returncode,r.stdout)
class Links(HTMLParser):
 def __init__(self):super().__init__();self.links=[]
 def handle_starttag(self,tag,attrs):
  if tag=='a':
   h=dict(attrs).get('href','')
   if h:self.links.append(urljoin(url,h))
if r.returncode==0:
 data=(E/'reference.html').read_bytes();x=Links();x.feed(data.decode(errors='replace'));(E/'links.json').write_text(json.dumps(x.links,indent=2));print('\n'.join(x.links));(E/'sha256').write_text(hashlib.sha256(data).hexdigest()+'\n')
