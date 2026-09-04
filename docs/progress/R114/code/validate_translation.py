#!/usr/bin/env python3
import csv
import re
import sys
from collections import Counter
from pathlib import Path

csv.field_size_limit(sys.maxsize)
SOURCE = Path("/tmp/r114_input_20260904/hq_20260904/base_packages_final.tsv")
OUTPUT = Path("/tmp/r114_output_20260904/hq_20260904/base_packages_final.tsv")

with SOURCE.open(encoding="utf-8", newline="") as stream:
    source = list(csv.DictReader(stream, delimiter="\t"))
with OUTPUT.open(encoding="utf-8", newline="") as stream:
    reader = csv.DictReader(stream, delimiter="\t")
    headers = list(reader.fieldnames or [])
    output = list(reader)

assert len(source) == len(output) == 254
assert len(headers) == 9
assert [r["源码包名"] for r in source] == [r["Source package"] for r in output]

decision_map = {
    "需要改": "requires adaptation",
    "不需要改": "does not require adaptation",
    "当前不能改": "currently cannot be adapted",
}
for zh, en in zip(source, output):
    assert en["Decision"] == decision_map[zh["判定结果"]], zh["源码包名"]

han = re.compile(r"[\u3400-\u9fff]")
chinese_cells = [(row["Source package"], h) for row in output for h, value in row.items() if han.search(value)]
assert not chinese_cells, chinese_cells[:20]

internal = re.compile(r"R(?:32|100|102|103|109)|N1_|C1_|NO_[A-Z]|ACTUAL_CPP_BUILD_LOG|CPP_RUNTIME_DEPENDENCY|CPP_HEADER_PAYLOAD|GLOBAL_DEMANGLED_CPP_IN_STATIC_ARCHIVE|CPP_ABI_NO_STD_LAYOUT_PROVEN|LAYOUT_SENSITIVE_STD_TYPE|PURE_C_INTERFACE|OTHER_NO_SYMBOL_INTERSECTION")
internal_hits = [(row["Source package"], h, internal.search(value).group(0)) for row in output for h, value in row.items() if internal.search(value)]
assert not internal_hits, internal_hits[:20]

sha = re.compile(r"\b[0-9a-f]{40}\b")
for zh, en in zip(source, output):
    assert Counter(sha.findall("\t".join(zh.values()))) == Counter(sha.findall("\t".join(en.values()))), (zh["源码包名"], "SHA")

def protected_evidence_tokens(value):
    tokens = []
    for part in value.replace("；", ";").split(";"):
        part = part.strip()
        if part.startswith("source=") or part.startswith("VCS=") or part.startswith("Summary="):
            tokens.append(part)
    return tokens

for zh, en in zip(source, output):
    if zh["源码包名"] not in {"gmp", "tensorflow2"}:
        for token in protected_evidence_tokens(zh["证据"]):
            assert token in en["Evidence"], (zh["源码包名"], token)

backticks = re.compile(r"`([^`]+)`")
code_differences = []
for zh, en in zip(source, output):
    z = Counter(backticks.findall(zh["理由"] + " " + zh["证据"]))
    e = Counter(backticks.findall(en["Rationale"] + " " + en["Evidence"]))
    if z != e:
        code_differences.append((zh["源码包名"], z - e, e - z))

number = re.compile(r"(?<![A-Za-z0-9_])\d+(?:[.,]\d+)*(?![A-Za-z0-9_])")
number_differences = []
for zh, en in zip(source, output):
    z = Counter(number.findall("\t".join(zh.values())))
    e = Counter(number.findall("\t".join(en.values())))
    if z != e:
        number_differences.append((zh["源码包名"], z - e, e - z))

print(f"rows={len(output)} headers={len(headers)}")
print("decision_counts=" + repr(Counter(row["Decision"] for row in output)))
print(f"chinese_cells={len(chinese_cells)}")
print(f"internal_marker_hits={len(internal_hits)}")
print(f"code_span_differences={len(code_differences)}")
for item in code_differences:
    print("CODE_DIFF", item)
print(f"numeric_token_differences={len(number_differences)}")
for item in number_differences:
    print("NUMBER_DIFF", item)

if code_differences or number_differences:
    sys.exit(3)
