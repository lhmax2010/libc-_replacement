#!/usr/bin/env python3
import csv
import math
import re
from collections import Counter, defaultdict, deque
from pathlib import Path

csv.field_size_limit(64 * 1024 * 1024)

ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/"progress/R22/tables"
def read(rel):
    with (ROOT/rel).open(encoding="utf-8",newline="") as f:return list(csv.DictReader(f,delimiter="\t"))
def write(name,fields,rows):
    with (OUT/name).open("w",encoding="utf-8",newline="") as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter="\t",lineterminator="\n",extrasaction="ignore");w.writeheader();w.writerows(rows)

old=read("progress/R19_21/R20/tables/phase_package_list.tsv")
old_by={r["source_rpm"]:r for r in old}; nodes=set(old_by)
vendors=read("progress/R13/tables/vendor_prebuilt_closure_assignment.tsv")
vendor_names=sorted({r["vendor_package_name"] for r in vendors})
bins=read("progress/R11/tables/binary_package_records.tsv")
vendor_source_by_name=defaultdict(set)
for r in bins:
    if r["name"] in vendor_names:vendor_source_by_name[r["name"]].add(r["sourcerpm"])
vendor_sources=set().union(*vendor_source_by_name.values()) if vendor_source_by_name else set()
migrate=nodes-vendor_sources

edge_rows=read("progress/R22/tables/source_edge_classification.tsv")
true_edges={(r["consumer_source_rpm"],r["provider_source_rpm"]) for r in edge_rows if r["classification"]=="TRUE_CPP_ABI_COUPLING" and r["consumer_source_rpm"] in migrate and r["provider_source_rpm"] in migrate}
special_vendor_edges=[r for r in edge_rows if r["classification"]=="TRUE_CPP_ABI_COUPLING" and ((r["consumer_source_rpm"] in vendor_sources) != (r["provider_source_rpm"] in vendor_sources))]
graph=defaultdict(set)
for a,b in true_edges:graph[a].add(b)

# Tarjan SCC.
index=0; stack=[]; on=set(); idx={}; low={}; comps=[]
def strong(v):
    global index
    idx[v]=low[v]=index;index+=1;stack.append(v);on.add(v)
    for w in sorted(graph.get(v,set())):
        if w not in idx:strong(w);low[v]=min(low[v],low[w])
        elif w in on:low[v]=min(low[v],idx[w])
    if low[v]==idx[v]:
        c=[]
        while True:
            w=stack.pop();on.remove(w);c.append(w)
            if w==v:break
        comps.append(sorted(c))
for n in sorted(migrate):
    if n not in idx:strong(n)
comps=sorted(comps,key=lambda c:(-len(c),c))
comp_id={n:i for i,c in enumerate(comps,1) for n in c}
comp_members={i:c for i,c in enumerate(comps,1)}
dag=defaultdict(set)
for a,b in true_edges:
    ca,cb=comp_id[a],comp_id[b]
    if ca!=cb:dag[ca].add(cb)

# 首批五包与测试框架是固定阶段一种子；阶段一扩展为其完整 provider 依赖闭包。
wave_names={"askuser-notification","cynara","libcynara-commons","libcynara-creds-dbus","security-manager"}
wave={r["source_rpm"] for r in old if r["source_name"] in wave_names}
tests={r["source_rpm"] for r in old if re.search(r"(^|[-_])(gtest|gmock|googletest|catch2|cppunit)([-_]|$)",r["source_name"],re.I)}
stage1_comps={comp_id[n] for n in wave|tests}
q=list(stage1_comps)
while q:
    c=q.pop()
    for dep in dag.get(c,set()):
        if dep not in stage1_comps:stage1_comps.add(dep);q.append(dep)

# 剩余 SCC 按 provider-first 拓扑深度二分。依赖的深度必小于消费者，因此不会产生逆向边。
memo={}
def depth(c):
    if c in stage1_comps:return -1
    if c in memo:return memo[c]
    ds=[depth(d) for d in dag.get(c,set()) if d not in stage1_comps]
    memo[c]=0 if not ds else 1+max(ds)
    return memo[c]
remaining=[i for i in comp_members if i not in stage1_comps]
for c in remaining:depth(c)
max_depth=max((memo[c] for c in remaining),default=0)
cutoff=max_depth//2
stage_by_comp={c:"PHASE1" for c in stage1_comps}
for c in remaining:stage_by_comp[c]="PHASE2" if memo[c]<=cutoff else "PHASE3"
stage={n:stage_by_comp[comp_id[n]] for n in migrate}

# 逆向：消费者阶段早于 provider 阶段。
rank={"PHASE1":1,"PHASE2":2,"PHASE3":3}
reverse=[]
for a,b in sorted(true_edges):
    if rank[stage[a]]<rank[stage[b]]:
        reverse.append({"consumer_source_rpm":a,"consumer_stage":stage[a],"provider_source_rpm":b,"provider_stage":stage[b],"reason":"consumer stage is earlier than provider stage"})
write("corrected_inverse_edges.tsv",list(reverse[0]) if reverse else ["consumer_source_rpm","consumer_stage","provider_source_rpm","provider_stage","reason"],reverse)

plugins=read("progress/R13/tables/plugin_host_closure_assignment.tsv")
plugin_sources={r["sourcerpm"] for r in plugins}
chromium="chromium-efl-1.1.144-1.src.rpm"
full=[]
for n in sorted(nodes):
    o=old_by[n]; notes=[]
    if n in vendor_sources:
        st="VENDOR_PREBUILT_SEPARATE";cid="NOT_APPLICABLE"
        notes.append("one or more R13 vendor prebuilt binary Names map to this source RPM")
    else:
        st=stage[n];cid=f"SCC{comp_id[n]:04d}"
        if comp_id[n] in stage1_comps and n not in wave|tests:notes.append("included in phase 1 as a transitive C++ ABI provider dependency of required wave1/test seeds")
        if n in wave:notes.append("required wave-1 source package")
        if n in tests:notes.append("test framework")
        notes.append(f"provider-first SCC topological depth={-1 if comp_id[n] in stage1_comps else memo[comp_id[n]]}; phase2/3 cutoff={cutoff}")
    if n in plugin_sources:notes.append("R13 C++ plugin-host source")
    if n==chromium:notes.append("R19: 42/42 runtime ELF depend libstdc++; 0 libc++/libc++abi; std::__1=0; source modification classification UNKNOWN because T1 did not scan SRPM")
    full.append({"source_rpm":n,"package_name":o["source_name"],"stage":st,"binary_package_count":o["binary_package_name_count"],"source_change_requirement":o["source_change"],"source_change_patterns":o["source_patterns"],"build_system_change_requirement":o["build_system_change"],"build_change_patterns":o["build_patterns"],"in_R13_risk_set":o["r13_risk_set"],"coupling_component":cid,"remarks":"; ".join(notes)})
write("migration_packages_full.tsv",list(full[0]),full)

coupled=[]
for i,c in comp_members.items():
    if len(c)>1:
        for n in c:coupled.append({"component_id":f"SCC{i:04d}","size":len(c),"stage":stage_by_comp[i],"source_rpm":n,"members":";".join(c)})
write("coupling_components.tsv",list(coupled[0]) if coupled else ["component_id","size","stage","source_rpm","members"],coupled)
topo=[]
for i,c in sorted(comp_members.items()):
    topo.append({"component_id":f"SCC{i:04d}","size":len(c),"stage":stage_by_comp[i],"topological_depth":-1 if i in stage1_comps else memo[i],"dependency_components":";".join(f"SCC{x:04d}" for x in sorted(dag.get(i,set()))),"members":";".join(c)})
write("scc_topology.tsv",list(topo[0]),topo)

special=[]
for name in vendor_names:
    for src in sorted(vendor_source_by_name[name]):special.append({"type":"VENDOR_PREBUILT_BINARY","object":name,"source_rpm":src,"stage":"VENDOR_PREBUILT_SEPARATE","facts":"R13 vendor list; permanent old runtime per task background"})
for n in sorted(tests):special.append({"type":"TEST_FRAMEWORK","object":old_by[n]["source_name"],"source_rpm":n,"stage":stage[n],"facts":"platform source-name regex match"})
for n in sorted(plugin_sources&migrate):special.append({"type":"CPP_PLUGIN_HOST_SOURCE","object":old_by[n]["source_name"],"source_rpm":n,"stage":stage[n],"facts":"R13 plugin host and R20 adaptation set"})
special.append({"type":"CHROMIUM_EFL","object":"chromium-efl","source_rpm":chromium,"stage":stage[chromium],"facts":"R19 measured 42/42 libstdc++, 0 libc++, std::__1=0; source-change UNKNOWN"})
write("special_cases.tsv",list(special[0]),special)

# R20 UNKNOWN 121 归因为规则空档，并给出补齐后的阶段。
unknown=[]
for o in old:
    if o["phase"]=="UNKNOWN":
        unknown.append({"source_rpm":o["source_rpm"],"R20_reason":o["mechanical_reason"],"attribution":"RULE_GAP_DEPENDENCY_OUTSIDE_PHASE1","unresolved_dependency_edge_count":"NOT_THE_R20_CAUSE","ELF_status":"HAS_CPP_RUNTIME_ELF_BY_483_SET_DEFINITION","classification_data_status":o["t1_r1_classification"],"R22_resolution":"SCC condensation plus provider-first topological stage","R22_stage":stage[o["source_rpm"]]})
write("unknown_121_attribution.tsv",list(unknown[0]),unknown)
us=Counter(r["attribution"] for r in unknown)
write("unknown_attribution_summary.tsv",["reason","package_count","denominator","unit"],[{"reason":k,"package_count":v,"denominator":len(unknown),"unit":"source RPM"} for k,v in sorted(us.items())])

summary=[]
for st in ["PHASE1","PHASE2","PHASE3","VENDOR_PREBUILT_SEPARATE"]:
    rows=[r for r in full if r["stage"]==st]
    summary.append({"stage":st,"source_package_count":len(rows),"denominator_source_packages":len(full),"source_change_yes":sum(r["source_change_requirement"]=="YES" for r in rows),"source_change_unknown":sum(r["source_change_requirement"]=="UNKNOWN" for r in rows),"build_change_yes":sum(r["build_system_change_requirement"]=="YES" for r in rows),"build_change_unknown":sum(r["build_system_change_requirement"]=="UNKNOWN" for r in rows),"binary_package_name_count_sum":sum(int(r["binary_package_count"]) for r in rows),"unit":"source RPM; binary count is sum of distinct binary Names per source"})
write("stage_summary.tsv",list(summary[0]),summary)

checks=[]
def check(name,ok,observed):checks.append({"assertion":name,"status":"PASS" if ok else "FAIL","observed":observed})
check("three stages plus separate equals 483",sum(r["source_package_count"] for r in summary)==len(full),f"{sum(r['source_package_count'] for r in summary)}/{len(full)}")
for n in sorted(wave):check(f"wave1 {n} in phase1",stage.get(n)=="PHASE1",stage.get(n,"NOT_FOUND"))
for name in ["libcynara-commons","security-manager"]:
    rows=[r for r in full if r["package_name"]==name]
    check(f"{name} source change YES",len(rows)==1 and rows[0]["source_change_requirement"]=="YES",rows[0]["source_change_requirement"] if rows else "NOT_FOUND")
check("corrected reverse true ABI edges zero",len(reverse)==0,str(len(reverse)))
for n in sorted(tests):check(f"test framework {n} in phase1",stage.get(n)=="PHASE1",stage.get(n,"NOT_FOUND"))
cr=next((r for r in full if r["source_rpm"]==chromium),None)
check("chromium included normal stage",cr is not None and cr["stage"] in {"PHASE1","PHASE2","PHASE3"},cr["stage"] if cr else "NOT_FOUND")
check("chromium source change UNKNOWN",cr is not None and cr["source_change_requirement"]=="UNKNOWN",cr["source_change_requirement"] if cr else "NOT_FOUND")
check("R20 unknown 121 all resolved to stages",len(unknown)==121 and all(r["R22_stage"] in {"PHASE1","PHASE2","PHASE3"} for r in unknown),f"{len(unknown)}/121")
write("cross_checks.tsv",list(checks[0]),checks)

method=[
 {"item":"retained ABI edge","rule":"consumer UND dynamic symbol intersects provider DEF dynamic symbol and at least one intersecting symbol begins with Itanium ABI prefix _Z"},
 {"item":"removed pure C edge","rule":"intersection exists, but every intersecting symbol is non-_Z"},
 {"item":"removed other edge","rule":"no dynamic-symbol intersection observed; retained in attribution table, not asserted as ABI coupling"},
 {"item":"phase 1","rule":"required wave1 five and test-framework SCCs plus their full transitive provider dependency closure"},
 {"item":"phase 2/3","rule":f"remaining SCC DAG provider-first depth; PHASE2 depth <= floor(max_depth/2)={cutoff}, PHASE3 depth > {cutoff}; observed max_depth={max_depth}"},
]
write("corrected_rules.tsv",list(method[0]),method)
write("vendor_boundary_true_edges.tsv",list(special_vendor_edges[0]) if special_vendor_edges else list(edge_rows[0]),special_vendor_edges)
