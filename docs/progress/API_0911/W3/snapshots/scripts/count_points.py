#!/usr/bin/env python3
"""仅计已定位原文中的指定变更点，不估算完整迁移工时或包数。"""
import pathlib,re,json,hashlib
base=pathlib.Path('docs/progress/P7_0909/stage2/snapshots/evidence')
specs=[('chromium-efl/55f75e1f273b_ewk_cookie_parser.h',r'ewk_parse_cookie\('),
('webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h',r'SetSyncBinaryReply'),
('webapi-plugins/e4597e6f32ca_extension.cc',r'void Instance::SendSyncBinaryReply|g_sync_messaging->SetSyncBinaryReply'),
('dali2-csharp-binder/b0487e765ad5_control-wrap.cpp',r'SWIGEXPORT.*(?:CSharp_Dali_View_New\(|CSharp_Dali_View_NewWithBehaviour\(|CSharp_Dali_delete_View\()')]
rows=[]
for name,pattern in specs:
    p=base/name;data=p.read_bytes();hits=[dict(line=n,text=line) for n,line in enumerate(data.decode().splitlines(),1) if re.search(pattern,line)]
    assert hits,'正向检索控制不通过'
    rows.append(dict(path=str(p),sha256=hashlib.sha256(data).hexdigest(),pattern=pattern,matched_lines=len(hits),hits=hits,unit='匹配行；不等于完整改造函数数'))
pathlib.Path('docs/progress/API_0911/W3/known_points.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(rows,ensure_ascii=False,indent=2))
