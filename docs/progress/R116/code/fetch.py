#!/usr/bin/env python3
"""公开资料获取；保留 URL、HTTP 结果、内容 SHA 和实际 curl 退出码。"""
import base64,hashlib,pathlib,subprocess,sys
name,url,kind=sys.argv[1:]; root=pathlib.Path('docs/progress/R116'); dest=root/'sources';dest.mkdir(exist_ok=True)
raw=dest/(name+'.raw'); output=dest/(name+'.txt')
if raw.exists() or output.exists():raise SystemExit('refuse overwrite')
rc=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(root/'raw/fetch'/name),
 'curl','--fail-with-body','--location','--max-time','45','--retry','1','--output',str(raw),
 '--write-out','HTTP_CODE=%{http_code}\nEFFECTIVE_URL=%{url_effective}\n',url]).returncode
if rc:raise SystemExit(rc)
data=raw.read_bytes()
if kind=='base64':data=base64.b64decode(data)
output.write_bytes(data)
print('SOURCE_URL='+url);print('SHA256='+hashlib.sha256(data).hexdigest());print('FILE='+str(output))
