#!/usr/bin/env python3
from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
BASE = ROOT / "docs/progress/R110"
BACKGROUND = BASE / "DECISION_BACKGROUND.md"
CHECKLIST = BASE / "REVIEW_FACT_CHECKLIST.md"
REPORT = BASE / "W4_REPORT.md"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> int:
    background = BACKGROUND.read_text(encoding="utf-8")
    checklist = CHECKLIST.read_text(encoding="utf-8")
    report = REPORT.read_text(encoding="utf-8")

    decision_headings = [
        "## 是否允许条件变量的普通错误作为异常传播",
        "## 若改变等待契约，如何处理已经编译的调用方",
        "## `wbuffer_convert` 析构是否继续自动同步",
        "## 如何处理 Boost 组件之间的跨 C++ 运行库语义边界",
    ]
    for heading in decision_headings:
        require(background.count(heading) == 1, f"missing or duplicate heading: {heading}")

    for label in ("实测", "静态核查", "推断", "NOT_OBSERVED", "NOT_AVAILABLE"):
        require(label in background, f"missing evidence label: {label}")

    for heading in ("## 必须先知道的共同事实", "## 证据覆盖与限制", "## 可复核材料"):
        require(heading in background, f"missing cross-cutting section: {heading}")

    require("49/90" in background and "41/90" in background, "Boost accounting missing")
    require("33/63" in background and "16/27" in background, "Boost class accounting missing")
    require("1,450" in background and "39,166" in background, "coverage denominator missing")
    require("不作推荐" in background and "不排列优先级" in background, "decision neutrality missing")

    for target in re.findall(r"\]\(([^)]+\.md)\)", background):
        require((BACKGROUND.parent / target).resolve().is_file(), f"broken evidence link: {target}")

    require(checklist.count("- [ ]") >= 60, "review checklist is unexpectedly short")
    require("不作推荐" in report and "不作推荐" in background, "non-decision statement missing")

    print(
        "ASSERTIONS=PASS "
        f"decision_sections={len(decision_headings)} "
        f"checklist_items={checklist.count('- [ ]')} "
        "links=PASS arithmetic=PASS scope_labels=PASS"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (AssertionError, OSError, UnicodeError) as exc:
        print(f"ASSERTIONS=FAIL reason={exc}", file=sys.stderr)
        raise SystemExit(1)
