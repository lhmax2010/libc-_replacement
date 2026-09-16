from common import *
import re,json
def lines(stem):return (OUT/'raw'/f'{stem}.stdout').read_text().replace('\r','').splitlines()
a={l.split(' ',3)[-1]:l for l in lines('050_root_diagnostic_inventory') if re.match(r'\d{4}-\d\d-\d\d ',l)}
b={l.split(' ',3)[-1]:l for l in lines('056_root_inventory_after') if re.match(r'\d{4}-\d\d-\d\d ',l)}
deleted=sorted(a.keys()-b.keys());added=sorted(b.keys()-a.keys());changed=[p for p in a.keys()&b.keys()if a[p]!=b[p]]
assert deleted==['/opt/usr/share/crash/dump/callback_gnu_8998_20260916202320.zip']
assert not added and not changed
before=lines('048_owner_before_cleanup')[0];after=lines('058_owner_restored')[0]
assert before==after and 'uid=5001(owner)' in after
assert 'R119_ABSENT=0' in lines('058_owner_restored')
save(OUT/'W0/ROOT_CLEANUP.json',dict(before_files=len(a),after_files=len(b),deleted=deleted,added=added,other_path_size_mtime_changes=changed,owner_identity_restored=True,task_directory_absent=True,backup=str(TMP/'callback_gnu_8998_20260916202320.zip'),root_records=['049_sdb_root_on','050_root_diagnostic_inventory','051_root_target_hash','052_root_pull_target','055_root_delete_verified_diagnostic','056_root_inventory_after','057_sdb_root_off'],limits='仅比较清理前后诊断文件路径/大小/mtime；不声称其他文件逐字节相同或动态系统全部进程不变'))
print('deleted',deleted,'others_unchanged',len(b),'owner_restored',True)
