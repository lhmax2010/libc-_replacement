#!/usr/bin/env python3
"""Extract every C++ fenced code block from the frozen R74 report."""

from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path


REPORT = Path("docs/hq/libcxx_issues_report_20260827.md")
OUT = Path("progress/R74/extracted")


def slug(text: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_-]+", "_", text).strip("_")
    return value or "unlabelled"


def main() -> int:
    text = REPORT.read_text(encoding="utf-8")
    lines = text.splitlines(keepends=True)
    issue = "报告前言"
    heading = "报告前言"
    blocks: list[dict[str, object]] = []
    index = 0
    i = 0
    while i < len(lines):
        stripped = lines[i].rstrip("\r\n")
        if stripped.startswith("## "):
            heading = stripped[3:]
            if heading.startswith("问题"):
                issue = heading.split("：", 1)[0]
        elif stripped.startswith("### "):
            heading = stripped[4:]
        match = re.fullmatch(r"```(cpp|c\+\+|cc|cxx)", stripped, re.I)
        if not match:
            i += 1
            continue
        start = i + 2
        body: list[str] = []
        i += 1
        while i < len(lines) and lines[i].rstrip("\r\n") != "```":
            body.append(lines[i])
            i += 1
        if i == len(lines):
            raise RuntimeError(f"unterminated code block at report line {start - 1}")
        end = i
        index += 1
        name = f"{index:02d}_{slug(issue)}_{slug(heading)}.cpp"
        data = "".join(body).encode("utf-8")
        (OUT / name).write_bytes(data)
        blocks.append(
            {
                "index": index,
                "issue": issue,
                "heading": heading,
                "language": match.group(1),
                "report_start_line": start,
                "report_end_line": end,
                "file": str(OUT / name),
                "bytes": len(data),
                "sha256": hashlib.sha256(data).hexdigest(),
            }
        )
        i += 1
    (OUT / "blocks.json").write_text(
        json.dumps(blocks, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )
    with (OUT / "INDEX.tsv").open("w", encoding="utf-8") as stream:
        stream.write(
            "index\tissue\theading\treport_start_line\treport_end_line\tfile\tbytes\tsha256\n"
        )
        for block in blocks:
            stream.write(
                "{index}\t{issue}\t{heading}\t{report_start_line}\t{report_end_line}\t"
                "{file}\t{bytes}\t{sha256}\n".format(**block)
            )
    print(f"CPP_BLOCK_COUNT={len(blocks)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
