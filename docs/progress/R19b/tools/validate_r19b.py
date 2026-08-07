#!/usr/bin/env python3
import csv
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
def read(rel):
    with (ROOT/rel).open(encoding="utf-8",newline="") as f:return list(csv.DictReader(f,delimiter="\t"))
checks=[]
def ck(name,ok,observed): checks.append((name,"PASS" if ok else "FAIL",str(observed)))
r=read("progress/R19b/tables/analyzed_rpm_origin.tsv")
b=read("progress/R19b/tables/board_installed_chromium.tsv")
c=read("progress/R19b/tables/board_nevra_comparison.tsv")
ck("RPM instances",len(r)==15,len(r))
ck("SHA triple match",all(x["sha_match"]=="MATCH" for x in r),sum(x["sha_match"]=="MATCH" for x in r))
ck("Unified source only",all(x["r11_repo_id"]=="unified_binary" for x in r),sorted(set(x["r11_repo_id"] for x in r)))
ck("R19 runtime ELF sum",sum(int(x["R19_runtime_ELF_count"]) for x in r)==42,sum(int(x["R19_runtime_ELF_count"]) for x in r))
ck("Board installed chromium rows",len(b)==1,len(b))
ck("Board matching NEVRA",sum(x["comparison"]=="NEVRA_MATCH" for x in c)==1,sum(x["comparison"]=="NEVRA_MATCH" for x in c))
ck("Board uninstalled arm subpackages",sum(x["comparison"]=="NOT_INSTALLED_ON_BOARD" for x in c)==4,sum(x["comparison"]=="NOT_INSTALLED_ON_BOARD" for x in c))
out=ROOT/"progress/R19b/validation.tsv"
with out.open("w",encoding="utf-8") as f:
    f.write("check\tstatus\tobserved\n")
    for x in checks:f.write("\t".join(x)+"\n")
raise SystemExit(0 if all(x[1]=="PASS" for x in checks) else 1)
