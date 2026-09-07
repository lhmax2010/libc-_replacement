#!/usr/bin/env python3
import argparse
import hashlib
from pathlib import Path


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--header", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    target = args.output / "__locale_dir/wbuffer_convert.h"
    target.parent.mkdir(parents=True, exist_ok=True)
    text = args.header.read_text()
    old = "wbuffer_convert<_Codecvt, _Elem, _Tr>::~wbuffer_convert() {\n  __close();\n  delete __cv_;"
    new = "wbuffer_convert<_Codecvt, _Elem, _Tr>::~wbuffer_convert() {\n  delete __cv_;"
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"unexpected destructor match count={count}")
    target.write_text(text.replace(old, new))
    print(f"replacement_count={count}")
    print(f"input_sha256={sha(args.header)}")
    print(f"output_sha256={sha(target)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
