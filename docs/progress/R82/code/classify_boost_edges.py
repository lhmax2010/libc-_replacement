#!/usr/bin/env python3
import csv
import subprocess
from collections import Counter
from pathlib import Path

source = Path("progress/R82/tables/boost_edges_90.tsv")
target = Path("progress/R82/tables/boost_edges_90_classified.tsv")

def demangle_many(names):
    if not names:
        return []
    run = subprocess.run(
        ["c++filt"], input="\n".join(names) + "\n", text=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True,
    )
    return run.stdout.splitlines()

with source.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))

output = []
for row in rows:
    mangled = list(filter(None, row["cpp_symbols"].split(";")))
    demangled = demangle_many(mangled)
    direct_std = any("std::" in symbol for symbol in demangled)
    provider = row["provider_binary"]
    if provider == "boost-filesystem":
        hidden = "boost::filesystem::path embeds std::basic_string; iterators carry path/pimpl state"
        payload = "Boost.Filesystem path/iterator/error objects; path hides standard string layout"
    elif provider == "boost-iostreams":
        hidden = "basic_mapped_file_params<detail::path>; detail::path embeds std::string and std::wstring"
        payload = "Boost.Iostreams mapped-file object/params; params hide standard strings"
    elif provider == "boost-program-options":
        hidden = "options/parsed-options/variables-map objects embed std::string, std::vector or std::map"
        payload = "direct standard strings/vectors/ostream plus Boost wrappers hiding standard containers"
    else:
        hidden = "NONE_PROVEN"
        if provider == "boost-test":
            payload = "Boost.Test types, builtins/function pointers; some edges directly pass std::basic_ostream"
        elif provider == "boost-thread":
            payload = "boost::once_flag reference; platform POSIX definition uses integer/atomic state"
        else:
            payload = "NOT_CLASSIFIED"

    if hidden != "NONE_PROVEN":
        risk = "HIDDEN_STD_LAYOUT" + ("+DIRECT_STD_SIGNATURE" if direct_std else "")
    elif direct_std:
        risk = "DIRECT_STD_SIGNATURE"
    else:
        risk = "NO_STD_LAYOUT_EVIDENCE"
    record = dict(row)
    record.update({
        "payload_class": payload,
        "direct_std_in_demangled_signature": "YES" if direct_std else "NO",
        "hidden_std_layout_evidence": hidden,
        "classification": risk,
        "demangled_symbols": ";".join(demangled),
    })
    output.append(record)

fields = list(rows[0]) + [
    "payload_class", "direct_std_in_demangled_signature",
    "hidden_std_layout_evidence", "classification", "demangled_symbols",
]
with target.open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(output)

counts = Counter(row["classification"] for row in output)
for key, value in sorted(counts.items()):
    print(f"classification[{key}]={value}")
hidden_consumers = sorted({row["consumer_source_rpm"] for row in output if row["classification"].startswith("HIDDEN_STD_LAYOUT")})
direct_consumers = sorted({row["consumer_source_rpm"] for row in output if row["direct_std_in_demangled_signature"] == "YES"})
no_std_consumers = sorted({row["consumer_source_rpm"] for row in output if row["classification"] == "NO_STD_LAYOUT_EVIDENCE"})
print(f"hidden_layout_consumer_packages={len(hidden_consumers)}:" + ";".join(hidden_consumers))
print(f"direct_std_consumer_packages={len(direct_consumers)}:" + ";".join(direct_consumers))
print(f"no_std_evidence_consumer_packages={len(no_std_consumers)}:" + ";".join(no_std_consumers))
