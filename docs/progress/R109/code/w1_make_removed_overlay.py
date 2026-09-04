#!/usr/bin/env python3
import argparse
import hashlib
from pathlib import Path


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--header", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    target_header = args.output / "include/__condition_variable/condition_variable.h"
    target_source = args.output / "condition_variable_removed.cpp"
    target_header.parent.mkdir(parents=True, exist_ok=True)
    header = args.header.read_text()
    source = args.source.read_text()
    header_replacements = 0
    source_replacements = 0
    for old, new in (
        ("void wait(unique_lock<mutex>& __lk) _NOEXCEPT;",
         "void wait(unique_lock<mutex>& __lk);"),
        ("__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<chrono::system_clock, chrono::nanoseconds>) _NOEXCEPT;",
         "__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<chrono::system_clock, chrono::nanoseconds>);"),
        ("__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<chrono::steady_clock, chrono::nanoseconds>) _NOEXCEPT;",
         "__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<chrono::steady_clock, chrono::nanoseconds>);"),
        ("__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<_Clock, chrono::nanoseconds>) _NOEXCEPT;",
         "__do_timed_wait(unique_lock<mutex>& __lk, chrono::time_point<_Clock, chrono::nanoseconds>);"),
        ("chrono::time_point<chrono::steady_clock, chrono::nanoseconds> __tp) _NOEXCEPT {",
         "chrono::time_point<chrono::steady_clock, chrono::nanoseconds> __tp) {"),
        ("chrono::time_point<_Clock, chrono::nanoseconds> __tp) _NOEXCEPT {",
         "chrono::time_point<_Clock, chrono::nanoseconds> __tp) {"),
    ):
        count = header.count(old)
        header_replacements += count
        header = header.replace(old, new)
    for old, new in (
        ("void condition_variable::wait(unique_lock<mutex>& lk) noexcept {",
         "void condition_variable::wait(unique_lock<mutex>& lk) {"),
        ("chrono::time_point<chrono::system_clock, chrono::nanoseconds> tp) noexcept {",
         "chrono::time_point<chrono::system_clock, chrono::nanoseconds> tp) {"),
    ):
        count = source.count(old)
        source_replacements += count
        source = source.replace(old, new)
    if header_replacements != 6 or source_replacements != 2:
        raise SystemExit(f"unexpected replacement counts: header={header_replacements} source={source_replacements}")
    target_header.write_text(header)
    target_source.write_text(source)
    print(f"header_replacements={header_replacements}")
    print(f"source_replacements={source_replacements}")
    print(f"input_header_sha256={digest(args.header)}")
    print(f"output_header_sha256={digest(target_header)}")
    print(f"input_source_sha256={digest(args.source)}")
    print(f"output_source_sha256={digest(target_source)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
