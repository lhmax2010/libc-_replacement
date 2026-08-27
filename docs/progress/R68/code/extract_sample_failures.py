#!/usr/bin/env python3
"""Extract complete per-test lit output for the four armv7l timing failures."""

import hashlib
import json
from pathlib import Path


WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
SAMPLE = WORKSPACE / "progress/R68/raw/capability/patched/armv7l/sample_actual_after_decode_fix"
INPUT = SAMPLE / "libcxx.json"
OUTPUT = SAMPLE / "failures"


def main() -> None:
    data = json.loads(INPUT.read_text(encoding="utf-8"))
    failures = [test for test in data["tests"] if test["code"] == "FAIL"]
    OUTPUT.mkdir(parents=True, exist_ok=True)

    manifest_lines = ["index\tname\toutput_file\tsha256"]
    for index, test in enumerate(failures, start=1):
        name = test["name"]
        basename = name.rsplit("/", 1)[-1].replace(" ", "_")
        output_name = f"{index:02d}-{basename}.txt"
        output_path = OUTPUT / output_name
        content = (
            f"name={name}\n"
            f"code={test['code']}\n"
            f"elapsed={test['elapsed']}\n"
            "output_begin\n"
            f"{test['output']}"
        )
        output_path.write_text(content, encoding="utf-8")
        digest = hashlib.sha256(output_path.read_bytes()).hexdigest()
        manifest_lines.append(f"{index}\t{name}\t{output_name}\t{digest}")

    (OUTPUT / "INDEX.tsv").write_text(
        "\n".join(manifest_lines) + "\n", encoding="utf-8"
    )

    if len(failures) != 4:
        raise RuntimeError(f"expected exactly 4 failures, observed {len(failures)}")


if __name__ == "__main__":
    main()
