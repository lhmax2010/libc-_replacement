#!/usr/bin/env python3
import csv, re, shlex, subprocess
from collections import defaultdict
from pathlib import Path

root=Path.cwd(); tables=root/"progress/R23/tables"
def read(name):
    with (tables/name).open(newline="",encoding="utf-8") as f:return list(csv.DictReader(f,delimiter="\t"))
def write(name,fields,rows):
    with (tables/name).open("w",newline="",encoding="utf-8") as f:
        w=csv.DictWriter(f,delimiter="\t",fieldnames=fields,lineterminator="\n");w.writeheader();w.writerows(rows)
scan=read("runtime_elf_scan_results.tsv"); fixed=read("fixed_point_symbols.tsv"); vendors=read("vendor_prebuilt_elf_scan.tsv")
targets=defaultdict(set)
for r in fixed:targets[r["arch"]].add(r["symbol"])
byarch=defaultdict(list)
for r in scan:byarch[r["arch"]].append(r)
keys={(r["arch"],r["package_nevra"],r["elf_path"]) for r in vendors}
for arch,rows in byarch.items():
    for i in sorted({0,len(rows)//2,len(rows)-1}):
        keys.add((arch,rows[i]["package_nevra"],rows[i]["elf_path"]))
selected=[r for r in scan if (r["arch"],r["package_nevra"],r["elf_path"]) in keys]
sym_re=re.compile(r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+\S+\s+\S+\s+(\S+)\s+(.+?)\s*$")
commands=[]; results=[]
for seq,r in enumerate(selected,1):
    cmd=["readelf","--dyn-syms","-W",r["local_elf_path"]]
    cp=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,errors="replace",check=False)
    commands.append({"sequence":seq,"command":" ".join(shlex.quote(x) for x in cmd),"exit_code":cp.returncode})
    und=set(); dyn_count=0
    for line in cp.stdout.splitlines():
        m=sym_re.match(line)
        if not m:continue
        dyn_count+=1
        if m.group(1)=="UND":
            raw=m.group(2).split()[0];base=raw.split("@",1)[0]
            if base in targets[r["arch"]]:und.add(base)
    parser_hits=set() if r["fixed_point_und_symbols"]=="NONE" else set(r["fixed_point_und_symbols"].split(","))
    # The ELF parser counts the mandatory index-0 null symbol; the line regex
    # intentionally requires a non-empty name, so GNU readelf's null line is
    # not counted.  Normalize that one-entry presentation difference here.
    parser_count=int(r["dynamic_symbol_count"])
    normalized_readelf_count=dyn_count + (1 if parser_count else 0)
    status="MATCH" if cp.returncode==0 and und==parser_hits and normalized_readelf_count==parser_count else "MISMATCH"
    results.append({"arch":r["arch"],"package_nevra":r["package_nevra"],"elf_path":r["elf_path"],
                    "readelf_exit_code":cp.returncode,"parser_dynamic_symbol_count":r["dynamic_symbol_count"],
                    "readelf_named_dynamic_symbol_count":dyn_count,"readelf_normalized_dynamic_symbol_count":normalized_readelf_count,
                    "parser_fixed_point_und":",".join(sorted(parser_hits)) or "NONE",
                    "readelf_fixed_point_und":",".join(sorted(und)) or "NONE","comparison":status})
write("elfparser_validation_commands.tsv",list(commands[0]),commands)
write("elfparser_validation_samples.tsv",list(results[0]),results)
print("samples",len(results),"match",sum(r["comparison"]=="MATCH" for r in results),"mismatch",sum(r["comparison"]!="MATCH" for r in results))
if any(r["comparison"]!="MATCH" for r in results):raise SystemExit(1)
