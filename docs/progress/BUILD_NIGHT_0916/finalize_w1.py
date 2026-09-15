"""Verify the remote fast-forward and publish derived package status, not old records."""
import csv
import json
from pathlib import Path

p = Path('progress/BUILD_NIGHT_0916')
def refs(name):
    return dict(line.split()[::-1] for line in (p/'raw'/name).read_text().splitlines())
before = refs('004_remote_bpf_before.stdout.txt')
after = refs('017_remote_bpf_after.stdout.txt')
target = 'refs/heads/sandbox/lhmax2025/libcxx-migration'
commit = 'f895f8c0373d224847fc7d3ecbeaac3bf926a1a1'
assert (p/'raw/016_push_bpftrace.exitcode').read_text().strip() == '0'
assert before.keys() == after.keys()
assert before[target] == '30e51cd665360f85b665308dc13ba27df0c5f739'
assert after[target] == commit
assert all(after[k] == before[k] for k in before if k != target)
with (p/'bpftrace_branch_comparison.tsv').open('w') as f:
    w = csv.writer(f, delimiter='\t')
    w.writerow(['branch','before','after','status'])
    for k in sorted(before):
        w.writerow([k,before[k],after[k],'AUTHORIZED_FAST_FORWARD' if k==target else 'UNCHANGED'])
rows = list(csv.DictReader(Path('progress/BUILD_W2C_0915/PACKAGE_STATUS.tsv').open(),delimiter='\t'))
assert len(rows) == 11
for row in rows:
    if row['package'] == 'bpftrace':
        row['adaptation_commit'] = commit
        row['validation'] = 'armv7l/aarch64 × libc++/GCC 源码四格完整通过；static 未重编、内嵌标准库 NOT_OBSERVED，按本轮人工批准单列；x86_64 不在 ExclusiveArch'
        row['current_status'] = '已完成并推送（源码适配）；static 资产替代另列待处理'
with (p/'PACKAGE_STATUS.tsv').open('w') as f:
    w = csv.DictWriter(f, delimiter='\t', fieldnames=rows[0].keys())
    w.writeheader(); w.writerows(rows)
md = '# 11 包源码适配状态：11/11 已推送\n\nstatic 资产单列，不等于其替代构建与运行时验证已完成。其余十包状态与既有记录不变。\n\n'
headers = list(rows[0])
md += '| ' + ' | '.join(headers) + ' |\n| ' + ' | '.join(['---']*len(headers)) + ' |\n'
for row in rows:
    md += '| ' + ' | '.join(row[k].replace('|','\\|') for k in headers) + ' |\n'
(p/'PACKAGE_STATUS.md').write_text(md)
result = dict(status='W1_PUSH_VERIFIED', commit=commit, branches=len(before),
              other_branches_unchanged=len(before)-1, new_branches=0,
              push_finished=(p/'raw/016_push_bpftrace.finished.txt').read_text().strip(),
              package_source_adaptations_pushed=11, static_replacement='NOT_COMPLETED')
(p/'w1_result.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(result,ensure_ascii=False,indent=2))
