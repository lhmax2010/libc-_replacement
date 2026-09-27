from common import *
inventory=json.loads((OUT/'INVENTORY.json').read_text())
details={x['edge']:x for x in json.loads((OUT/'DETAILS.json').read_text())}
setup={1:('app-core-cpp',['usr/include/appcore_cpp','usr/include/aul']),2:('bundle',[]),3:('bundle',[]),4:('scim_imengine',['usr/include/scim-1.0']),5:('jsoncpp',['usr/include/jsoncpp']),6:('absl_spinlock_wait,absl_raw_logging_internal,absl_base',[]),9:('gtest',[]),13:('dali2-core',[]),14:('dali2-core',[]),15:('dali2-core',[]),17:('gtest',[]),19:('zypp',[]),20:('zypp',[]),21:('zypp',[]),22:('dali2-core',[]),23:('zypp',['usr/include/libxml2'])}
rows=[]
setup[10]=('delta-manifest-handlers,manifest-parser',[])
setup[8]=('dali2-core,dali2-adaptor,glib-2.0',['usr/include/glib-2.0','usr/lib64/glib-2.0/include'])
setup[15]=('dali2-core,dali2-adaptor',[])
setup[12]=('dali2-toolkit,dali2-adaptor,dali2-core,glib-2.0',['usr/include/glib-2.0','usr/lib64/glib-2.0/include'])
for r in inventory:
 e=r['edge'];p=(ROOT/r['provider']).relative_to(TMP/'root')
 row=dict(edge=e,provider='/'+str(p))
 if e in setup:
  lib,inc=setup[e];row.update(fixture=f'fixtures/edge{e:02}.cpp',libs=lib.split(','),includes=inc)
 else:row['reason']=details[e]['missing']
 if e==23:row['libs'].append('xml2')
 if e in (8,12,15):row['headless']=True
 rows.append(row)
save('fixtures.json',rows)
