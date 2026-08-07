#!/usr/bin/env python3
import csv
import hashlib
import re
import subprocess
from datetime import datetime, timezone
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/"progress/R19b/tables"
OUT.mkdir(parents=True,exist_ok=True)

def read(rel):
    with (ROOT/rel).open(encoding="utf-8",newline="") as f: return list(csv.DictReader(f,delimiter="\t"))
def write(name,fields,rows):
    with (OUT/name).open("w",encoding="utf-8",newline="") as f:
        w=csv.DictWriter(f,fieldnames=fields,delimiter="\t",lineterminator="\n",extrasaction="ignore"); w.writeheader(); w.writerows(rows)
def sha(p):
    h=hashlib.sha256()
    with p.open("rb") as f:
        while b:=f.read(4*1024*1024): h.update(b)
    return h.hexdigest()

inputs=[
 "progress/R11/tables/binary_download_status.tsv",
 "progress/R11/tables/binary_package_records.tsv",
 "progress/R11/tables/elf_inventory.tsv",
 "progress/R11/tables/snapshot_identity.tsv",
 "progress/R11/tables/repodata_inventory.tsv",
 "progress/R19_21/R19/tables/chromium_runtime_elf.tsv",
 "progress/R19_21/R19/tables/chromium_targeted_nm_commands.tsv",
 "progress/R19_21/R19/tables/chromium_totals.tsv",
]
identity=[]
for rel in inputs:
    p=ROOT/rel
    identity.append({"path":rel,"size_bytes":p.stat().st_size,"sha256":sha(p),"status":"PASS"})
write("input_identity.tsv",list(identity[0]),identity)

downloads=[r for r in read("progress/R11/tables/binary_download_status.tsv") if r["name"].startswith("chromium-efl")]
elfs=[r for r in read("progress/R11/tables/elf_inventory.tsv") if r["name"].startswith("chromium-efl") and r["runtime_elf"]=="YES"]
elf_count={}
for r in elfs: elf_count[(r["name"],r["arch"],r["rpm_sha256"])]=elf_count.get((r["name"],r["arch"],r["rpm_sha256"]),0)+1
snapshot=next(r for r in read("progress/R11/tables/snapshot_identity.tsv") if r["repo_id"]=="unified_binary")
repodata=[r for r in read("progress/R11/tables/repodata_inventory.tsv") if r["repo_id"]=="unified_binary"]
primary=next(r for r in repodata if r["data_type"]=="primary")

rpms=[]
for r in sorted(downloads,key=lambda x:(x["arch"],x["name"])):
    p=Path(r["target"])
    st=p.stat()
    b=subprocess.run(["stat","-c","%w",str(p)],text=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    actual=sha(p)
    rpms.append({
      "name":r["name"],"epoch":r["epoch"],"version":r["version"],"release":r["release"],"arch":r["arch"],
      "nevra_with_epoch":f"{r['name']}-{r['epoch']}:{r['version']}-{r['release']}.{r['arch']}",
      "nevra_without_zero_epoch":f"{r['name']}-{r['version']}-{r['release']}.{r['arch']}",
      "sha256_recorded":r["actual_sha256"],"sha256_reverified":actual,"sha_match":"MATCH" if actual==r["actual_sha256"]==r["checksum"] else "MISMATCH",
      "local_path":r["target"],"source_repository":"Tizen-Unified-Toolchain/reference/repos/standard/packages",
      "r11_repo_id":r["repo_id"],"snapshot_identity_type":snapshot["snapshot_id_type"],"repomd_revision":snapshot["repomd_revision"],"repomd_sha256":snapshot["repomd_sha256"],
      "primary_metadata_sha256":primary["declared_sha256"],"download_url":r["url"],
      "local_file_birth_time":b.stdout.strip() if b.returncode==0 else "NOT_AVAILABLE",
      "local_file_mtime_download_completion_proxy":datetime.fromtimestamp(st.st_mtime,timezone.utc).astimezone().isoformat(),
      "download_time_status":"FILESYSTEM_TIMES_ONLY_R11_HAS_NO_PER_PACKAGE_WALLCLOCK_FIELD",
      "R19_runtime_ELF_count":elf_count.get((r["name"],r["arch"],r["actual_sha256"]),0),
    })
write("analyzed_rpm_origin.tsv",list(rpms[0]),rpms)
write("r11_unified_snapshot.tsv",list(snapshot),[snapshot])
write("r11_unified_repodata.tsv",list(repodata[0]),repodata)

# 解析已经逐字落盘的板端 rpm -qa 输出，不发起额外命令。
log=(ROOT/"progress/R19b/commands/004_board_rpm_query.log").read_text(encoding="utf-8",errors="replace")
board=[]
inside=False
for line in log.splitlines():
    if line=="--- stdout+stderr ---": inside=True; continue
    if line=="--- end ---": inside=False; continue
    if inside and line.startswith("chromium-efl"):
        p=line.rstrip("\r").split("\t")
        if len(p)==5:
            board.append({"name":p[0],"epoch":p[1],"version":p[2],"release":p[3],"arch":p[4],"nevra_with_epoch":f"{p[0]}-{p[1]}:{p[2]}-{p[3]}.{p[4]}","query_carrier":"sdb 192.168.108.25:26101","status":"INSTALLED"})
write("board_installed_chromium.tsv",list(board[0]) if board else ["name","epoch","version","release","arch","nevra_with_epoch","query_carrier","status"],board)

board_key={(r["name"],r["arch"]):r for r in board}
compare=[]
for r in rpms:
    b=board_key.get((r["name"],r["arch"]))
    if r["arch"]!="armv7l": status="NOT_APPLICABLE_BOARD_ARCH_ARMV7L"
    elif b is None: status="NOT_INSTALLED_ON_BOARD"
    elif (r["epoch"],r["version"],r["release"])==(b["epoch"],b["version"],b["release"]): status="NEVRA_MATCH"
    else: status="VERSION_MISMATCH"
    compare.append({"repository_nevra":r["nevra_with_epoch"],"board_nevra":b["nevra_with_epoch"] if b else "NOT_PRESENT","comparison":status,"payload_byte_identity":"NOT_CHECKED_ON_BOARD"})
write("board_nevra_comparison.tsv",list(compare[0]),compare)

scope=[
 {"scope_item":"R19 repository scope","declaration":f"Tizen Unified Toolchain standard binary repository component revision {snapshot['repomd_revision']}, repomd SHA256 {snapshot['repomd_sha256']}"},
 {"scope_item":"R19 RPM scope","declaration":"15 RPM instances listed in analyzed_rpm_origin.tsv; the 42 runtime ELF come from chromium-efl and chromium-efl-browser across aarch64/armv7l/x86_64"},
 {"scope_item":"Board installed scope","declaration":"Only chromium-efl-0:1.1.144-1.armv7l was observed installed; its NEVRA matches the analyzed armv7l main RPM"},
 {"scope_item":"Board byte identity boundary","declaration":"Installed payload SHA256 was not queried; byte-for-byte identity is NOT_OBSERVED"},
 {"scope_item":"Applicability","declaration":"R19 conclusions strictly apply to the 15 repository RPM identities and their 42 analyzed runtime ELF. At NEVRA level the armv7l chromium-efl main package matches the board; uninstalled subpackages and payload byte identity are outside the board observation."},
]
write("applicability_scope.tsv",list(scope[0]),scope)
