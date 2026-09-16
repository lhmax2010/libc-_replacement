"""归档未验证的局部 GN 候选；只验证补丁可应用，不冒充构建成功。"""
from common import *
import shutil
paths=['net/BUILD.gn','tizen_src/ewk/efl_integration/BUILD.gn']
patch=[];identity=[]
for rel in paths:
 old=TMP/'baseline'/rel;new=TMP/'chromium-efl'/rel
 row=run('W2','diff_'+rel.replace('/','_'),['git','diff','--no-index','--no-prefix',old,new],allow=(1,))
 text=(ROOT/(row['raw']+'.stdout')).read_text()
 for old_prefix,new_prefix in [(str(TMP/'baseline'),'a'),(str(TMP/'chromium-efl'),'b')]:
  text=text.replace(old_prefix+'/',new_prefix+'/').replace(old_prefix.lstrip('/')+'/',new_prefix+'/')
 patch.append(text)
 identity.append({'path':rel,'before_sha256':hashlib.sha256(old.read_bytes()).hexdigest(),'after_sha256':hashlib.sha256(new.read_bytes()).hexdigest()})
 check=TMP/'patch_check'/rel;check.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(old,check)
 for label,source in [('baseline',old),('candidate',new)]:
  dst=OUT/'W2'/label/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source,dst)
file=OUT/'W2/provider-exception-cleanup.CANDIDATE.patch';file.write_text(''.join(patch))
run('W2','candidate_apply_check',['git','apply','--check','--directory=tmp/P14/patch_check',file])
run('W2','candidate_apply_copy',['git','apply','--directory=tmp/P14/patch_check',file])
for row in identity:assert hashlib.sha256((TMP/'patch_check'/row['path']).read_bytes()).hexdigest()==row['after_sha256']
save('W2','PATCH_IDENTITY.json',{'status':'DRAFT_NOT_BUILT_NOT_RUNTIME_VALIDATED','baseline':'chromium-efl-1.1.144-1.src.rpm','git_revision':'NOT_AVAILABLE: SRPM snapshot, no authenticated source repository commit observed','patch_sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'files':identity,'apply_check':'PASS; isolated copy only'})
