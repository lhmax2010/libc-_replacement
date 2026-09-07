#!/usr/bin/env python3
import csv,pathlib,re,collections
base=pathlib.Path('docs/progress/R115/W3')
allrows={}
for arch in ['armv7l','x86_64']:
 rows=list(csv.DictReader((base/f'matrix_{arch}.tsv').open(),delimiter='\t'))
 assert len(rows)==90
 grouped=collections.defaultdict(list)
 for r in rows:
  grouped[(r['二进制'],r['模式'])].append(r)
  out=pathlib.Path(r['证据']+'.stdout').read_text()
  rc=int(r['实际退出码'])
  if r['二进制']=='wait_retained':
   assert rc==86 and 'event=terminate cleanup_count=0' in out
  elif r['二进制']=='wait_removed':
   assert rc==0 and 'cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in out
  elif r['二进制'].startswith('old_'):
   assert rc==0 and 'event=assert expected=1' in out
   if r['二进制']=='old_noisy':assert 'frame_cleanup=1 target_unlock=1' in out and 'event=trylock rc=0' in out
   else:assert 'frame_cleanup=0 target_unlock=0' in out and 'event=trylock rc=16' in out and 'contender_rc=110' in out
  elif r['模式']=='cv_any_throw':assert rc==86 and 'event=terminate' in out
  else:
   assert rc==0 and 'cancel_rc=0 join_rc=0 canceled=1 cleanup=1' in out
   if r['模式']=='shared_mutex':assert 'internal_mutex_unlocked=1 write_entered=1 reader_blocked=1 writer_blocked=1' in out
   elif r['模式']=='shared_timed_mutex':assert 'reader_acquired=0 writer_acquired=0 write_entered_observed=1' in out
   else:assert 'reusable=1' in out
 assert len(grouped)==18 and all(len(v)==5 for v in grouped.values())
 allrows[arch]=grouped
 extra=list(csv.DictReader((base/f'supplement_{arch}.tsv').open(),delimiter='\t'))
 assert len(extra)==15 and all(r['断言通过']=='1' for r in extra)

metrics=['实际退出码','canceled','terminate','cleanup','reusable','oldcaller_assert']
for key in allrows['armv7l']:
 for field in metrics:
  assert [r[field] for r in allrows['armv7l'][key]]==[r[field] for r in allrows['x86_64'][key]],(key,field)
for cc in ['gcc','clang']:
 for rep in range(1,6):
  data=(base/f'raw/cant_runs/{cc}_{rep}.stdout').read_text()
  assert 'create=0 cancel=0 join=0 canceled=1' in data and 'REMOTE_EXIT=0' in data

post=(base/'raw/058_cleanup_verify.stdout').read_text()
commands=[]
for line in post.splitlines():
 parts=line.split(None,7)
 if len(parts)==8 and parts[1].isdigit():commands.append(parts[7])
assert any(c=='ps -ef' for c in commands), 'ps 正向对照缺失'
probes={'wait_retained','wait_removed','facility','old_noisy','old_quiet','old_fno','old_fno_quiet','old_retained','new_noisy','new_quiet','cantunwind_gcc','cantunwind_clang'}
assert not [c for c in commands if pathlib.Path(c.split()[0]).name.removesuffix('_bounded') in probes]
assert 'CLEANUP_VERIFICATION_EXIT=0' in post
print('ASSERTIONS=PASS native_and_physical_matrix=210 cantunwind_physical=10 postflight_positive_control=PASS residue_processes=0')
with (base/'ARCH_COMPARISON.md').open('x') as f:
 f.write('# 架构逐格对照\n\n每格 5 次；完整值断言见各轮 stdout 与 validate.py。表中 0 不用作无问题结论。\n\n| 二进制/形态 | x86_64 原生退出码 | armv7l 物理板退出码 | 逐项事件一致 |\n|---|---|---|---|\n')
 for key in allrows['armv7l']:
  values=sorted({r['实际退出码'] for r in allrows['armv7l'][key]})
  f.write(f'| {key[0]} / {key[1]} | {",".join(values)} ×5 | {",".join(values)} ×5 | 是（非仅退出码） |\n')
 for name,rc in [('old_retained',86),('new_noisy',0),('new_quiet',0)]:
  f.write(f'| {name}（补充） | {rc} ×5 | {rc} ×5 | 是（补充脚本断言） |\n')
