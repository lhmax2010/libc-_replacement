import json,csv,pathlib,collections,hashlib
E=pathlib.Path(__file__).resolve().parent
rs=json.loads((E/'RPM_INVENTORY.json').read_text())
unique={str(pathlib.Path(r['path']).resolve()):r for r in rs}
rs=list(unique.values()); packages=['llvm','libcxx-runtimes','abseil-cpp','boost','icu','jsoncpp','libsigc++','pcre','taglib','tensorflow2','bcc-tools','bpftrace']
rows=[]; chosen=[]
for pkg in packages:
    for arch in ('armv7l','aarch64','x86_64'):
        prs=[r for r in rs if r['source']==pkg and r['arch'] in (arch,'noarch')]
        groups=collections.defaultdict(list)
        for r in prs:groups[(str(pathlib.Path(r['path']).resolve().parent),r['version'])].append(r)
        good=[]
        for k,gr in groups.items():
            req=[v for r in gr for v in r['Requires']]
            positive=any('libc++.so.1' in v for v in req)
            if pkg=='libcxx-runtimes':positive={'libc++','libc++abi','libc++-devel','libc++abi-devel'}<={r['name'] for r in gr}
            if positive and not any('libstdc++.so.6' in v for v in req):good.append((k,gr))
        if pkg=='bpftrace' and arch=='x86_64':status='NOT_APPLICABLE_EXCLUSIVEARCH';pick=[]
        elif good:
            _,pick=sorted(good,key=lambda kg:(len(kg[1]),'/local/repos/' in kg[0][0]),reverse=True)[0]
            status='RETAINED_LIBCXX_RPMS'
            chosen.extend(pick)
        else:status='NOT_AVAILABLE_LIBCXX_INPUT';pick=[]
        rows.append([pkg,arch,status,len({r['sha256'] for r in prs}),len(pick),';'.join(r['path'] for r in pick),'rpm Requires + historical validation/recipe audit; not a new ELF validation' if pick else ('ExclusiveArch excludes x86_64' if pkg=='bpftrace' and arch=='x86_64' else 'No matching libc++ RPM set observed in recorded local output/retention paths')])
def tsv(name,head,rows):
    with (E/name).open('w') as f:w=csv.writer(f,delimiter='\t');w.writerow(head);w.writerows(rows)
tsv('BASE_INPUT_SUMMARY.tsv',['source','arch','status','observed_distinct_RPMs','selected_RPMs','paths','basis'],rows)
tsv('RETAINED_LIBCXX_INPUTS.tsv',['source','name','version','arch','path','sha256','bytes'],[[r[k] for k in ('source','name','version','arch','path','sha256','bytes')] for r in chosen])
gates={a:[r[0] for r in rows if r[1]==a and r[2]=='NOT_AVAILABLE_LIBCXX_INPUT'] for a in ('armv7l','aarch64','x86_64')}
assert all(gates.values()),gates
(E/'ARCH_INPUT_GATE.json').write_text(json.dumps({'result':'NO_ELIGIBLE_ARCHITECTURE','missing':gates,'rule':'Task item 3: architectures with missing required local libc++ inputs are not built; no substitution or Base rebuild authorized.'},ensure_ascii=False,indent=2)+'\n')
providers=json.loads((E/'INPUT_IDENTITIES.json').read_text())['providers']
tsv('RESULTS.tsv',['package','arch','status','exitcode','elapsed_seconds','reason'],[[p,a,'NOT_OBSERVED','NOT_OBSERVED','NOT_OBSERVED','Base input gate: '+','.join(gates[a])] for p in providers for a in gates])
with (E/'INPUT_SUMMARY.md').open('w') as f:
    f.write('# 本地 Base 输入盘点\n\n仅盘点记录所列输出/保留路径；NOT_AVAILABLE 不是全磁盘不存在证明。SHA 与实际包头见 RPM_INVENTORY.tsv；候选选择不是本轮 ELF 复验。\n\n| 源码包 | armv7l | aarch64 | x86_64 |\n|---|---|---|---|\n')
    for p in packages:f.write('| '+p+' | '+' | '.join(next(r[2] for r in rows if r[0]==p and r[1]==a) for a in gates)+' |\n')
    f.write('\nRETAINED_LIBCXX_RPMS 表示找到历史已验证配方对应的保留输入；不表示全部依赖可满足。RPM 同名副本按路径保留，摘要去重数只用于盘点，不作包版本优先级。\n')
print(json.dumps(gates,ensure_ascii=False,indent=2))
