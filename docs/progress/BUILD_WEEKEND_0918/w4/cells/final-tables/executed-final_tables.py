"""Generate final published-package and observed-spec tables without changing decisions."""
import csv,json
from pathlib import Path
out=Path('progress/BUILD_WEEKEND_0918')
audit=json.loads((out/'spec-audit-result-v2.json').read_text())
remote={x['package']:x for x in json.loads((out/'package-head-results.json').read_text())}
assert audit['status']=='PASS' and len(audit['packages'])==11
headers=['package','branch','commit','published_status','prior_validation_reused','spec_guard_check','remote_head_match','candidate_notes']
rows=[]
for row in audit['packages']:
    name=row['package']; assert remote[name]['relation']=='MATCH'
    note='不涉及本轮 static 候选；历史构建结果沿用，不是本轮重编'
    if name=='bpftrace': note='源码适配已发布；新static替换未获验收：ARM libc++ RPM未完成，aarch64多出libbpf动态依赖，板上功能未验；GCC/未定义四格完整RPM已核验'
    if name=='llvm': note='已发布适配状态不变；本轮新增ARM静态开发包整轮退出1，18份RPM单独核验通过，不能标完整构建'
    rows.append([name,row['branch'],row['revision'],row['status'],row['prior_validation'],'PASS（字面条件审计）','MATCH',note])
with (out/'PACKAGE_STATUS.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t'); w.writerow(headers); w.writerows(rows)
def md(v): return str(v).replace('|','\\|').replace('\n',' ')
(out/'PACKAGE_STATUS.md').write_text('# 11 包最终状态\n\n11/11 已发布源码适配；本轮未推包仓。历史验证沿用，不重计为本轮新构建。bpftrace static 候选仍不可作为已验收替换件。\n\n| '+' | '.join(headers)+' |\n| '+' | '.join(['---']*len(headers))+' |\n'+''.join('| '+' | '.join(map(md,row))+' |\n' for row in rows))
with (out/'SPEC_GUARD_AUDIT.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t'); w.writerow(['package','revision','spec_sha256','kind','line','actual_code','conditions','check'])
    for row in audit['packages']:
        for kind,items in [('enable_definition',row['definitions']),('libcxx_option_or_dependency',row['options'])]:
            for item in items:
                w.writerow([row['package'],row['revision'],row['sha256'],kind,item['line'],item['text'],' / '.join(item['context']),item.get('valid',item.get('guarded'))])
print('11 package rows; exact option and enabling-condition lines exported')
