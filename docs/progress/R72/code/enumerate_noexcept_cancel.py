#!/usr/bin/env python3
"""Conservative source enumerator for noexcept-to-cancellation-point paths.

The script is intentionally a candidate generator, not an automatic verdict.
It uses Universal Ctags to delimit C++ function definitions, lexical call
extraction to build an over-approximated call graph, and breadth-first search
from explicitly non-throwing functions to POSIX/glibc cancellation-point
names. Every result requires source review because overload resolution,
conditional compilation and virtual dispatch are not resolved here.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from collections import defaultdict, deque
from dataclasses import dataclass
from pathlib import Path


POSIX_REQUIRED = {
    "accept", "aio_suspend", "clock_nanosleep", "close", "connect", "creat",
    "fcntl", "fdatasync", "fsync", "getmsg", "getpmsg", "lockf", "mq_receive",
    "mq_send", "mq_timedreceive", "mq_timedsend", "msgrcv", "msgsnd", "msync",
    "nanosleep", "open", "openat", "pause", "poll", "pread", "pselect",
    "pthread_cond_timedwait", "pthread_cond_wait", "pthread_join",
    "pthread_testcancel", "putmsg", "putpmsg", "pwrite", "read", "readv", "recv",
    "recvfrom", "recvmsg", "select", "sem_timedwait", "sem_wait", "send", "sendmsg",
    "sendto", "sigpause", "sigsuspend", "sigtimedwait", "sigwait", "sigwaitinfo",
    "sleep", "system", "tcdrain", "usleep", "wait", "waitid", "waitpid", "write",
    "writev",
}

POSIX_MAY = {
    "access", "asctime", "asctime_r", "catclose", "catgets", "catopen", "chmod",
    "chown", "closedir", "closelog", "ctermid", "ctime", "ctime_r", "dbm_close",
    "dbm_delete", "dbm_fetch", "dbm_nextkey", "dbm_open", "dbm_store", "dlclose",
    "dlopen", "dprintf", "endgrent", "endhostent", "endnetent", "endprotoent",
    "endpwent", "endservent", "endutxent", "faccessat", "fchmod", "fchmodat",
    "fchown", "fchownat", "fclose", "fflush", "fgetc", "fgetpos", "fgets", "fgetwc",
    "fgetws", "fmtmsg", "fopen", "fpathconf", "fprintf", "fputc", "fputs", "fputwc",
    "fputws", "fread", "freopen", "fscanf", "fseek", "fseeko", "fsetpos", "fstat",
    "fstatat", "ftell", "ftello", "ftw", "futimens", "fwprintf", "fwrite", "fwscanf",
    "getaddrinfo", "getc", "getc_unlocked", "getchar", "getchar_unlocked", "getcwd",
    "getdate", "getdelim", "getgrent", "getgrgid", "getgrgid_r", "getgrnam",
    "getgrnam_r", "gethostbyaddr", "gethostbyname", "gethostent", "gethostid",
    "gethostname", "getline", "getlogin", "getlogin_r", "getnameinfo", "getnetbyaddr",
    "getnetbyname", "getnetent", "getopt", "getprotobyname", "getprotobynumber",
    "getprotoent", "getpwent", "getpwnam", "getpwnam_r", "getpwuid", "getpwuid_r",
    "gets", "getservbyname", "getservbyport", "getservent", "getutxent", "getutxid",
    "getutxline", "getwc", "getwchar", "getwd", "glob", "iconv_close", "iconv_open",
    "ioctl", "link", "linkat", "lio_listio", "localtime", "localtime_r", "lseek",
    "lstat", "mkdir", "mkdirat", "mkdtemp", "mkfifo", "mkfifoat", "mknod", "mknodat",
    "mkstemp", "mktime", "nftw", "opendir", "openlog", "pathconf", "pclose", "perror",
    "popen", "posix_fadvise", "posix_fallocate", "posix_madvise", "posix_openpt",
    "posix_spawn", "posix_spawnp", "printf", "psiginfo", "psignal",
    "pthread_rwlock_rdlock", "pthread_rwlock_timedrdlock", "pthread_rwlock_timedwrlock",
    "pthread_rwlock_wrlock", "putc", "putc_unlocked", "putchar", "putchar_unlocked",
    "puts", "pututxline", "putwc", "putwchar", "readdir", "readdir_r", "readlink",
    "readlinkat", "remove", "rename", "renameat", "rewind", "rewinddir", "scandir",
    "scanf", "seekdir", "semop", "setgrent", "sethostent", "setnetent", "setprotoent",
    "setpwent", "setservent", "setutxent", "stat", "strerror", "strerror_r", "strftime",
    "symlink", "symlinkat", "sync", "syslog", "tmpfile", "tmpnam", "ttyname",
    "ttyname_r", "tzset", "ungetc", "ungetwc", "unlink", "unlinkat", "utime",
    "utimensat", "utimes", "vdprintf", "vfprintf", "vfwprintf", "vprintf", "vwprintf",
    "wcsftime", "wordexp", "wprintf", "wscanf",
}

# Linux/glibc names observed in addition to the POSIX spelling set. These are
# conservative lexical seeds; the report distinguishes source-confirmed names
# from names that are merely included to avoid false negatives.
GLIBC_EXTENSION_SEEDS = {
    "accept4", "epoll_pwait", "epoll_pwait2", "epoll_wait", "open64", "openat64",
    "ppoll", "pread64", "pwrite64", "recvmmsg", "sendmmsg", "wait3", "wait4",
}

CALL_RE = re.compile(r"(?<![A-Za-z0-9_])([A-Za-z_][A-Za-z0-9_:]*)\s*\(")
CONTROL_NAMES = {"if", "for", "while", "switch", "catch", "sizeof", "alignof", "decltype", "requires"}


@dataclass(frozen=True)
class Function:
    ident: int
    name: str
    scope: str
    path: str
    line: int
    end: int
    header: str
    noexcept_kind: str
    calls: tuple[str, ...]

    @property
    def qualified(self) -> str:
        return f"{self.scope}::{self.name}" if self.scope else self.name


def classify_noexcept(name: str, header: str) -> str:
    normalized = re.sub(r"\s+", " ", header)
    if re.search(r"\bnoexcept\s*\(\s*false\s*\)", normalized):
        return "NOEXCEPT_FALSE"
    if re.search(r"\bnoexcept\b|\b_LIBCPP_NOEXCEPT\b|\b_NOEXCEPT\b", normalized):
        return "EXPLICIT"
    if name.startswith("~"):
        return "IMPLICIT_DESTRUCTOR_REVIEW"
    return "NOT_MARKED"


def load_functions(root: Path) -> list[Function]:
    command = [
        "ctags", "--output-format=json", "--fields=+neKSt", "--kinds-C++=+fp",
        "--extras=-F", "-R", "-o", "-", str(root / "include"), str(root / "src"),
    ]
    proc = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if proc.returncode != 0:
        print(proc.stderr, file=sys.stderr)
        raise SystemExit(10)

    file_cache: dict[str, list[str]] = {}
    functions: list[Function] = []
    for raw in proc.stdout.splitlines():
        try:
            tag = json.loads(raw)
        except json.JSONDecodeError:
            continue
        if tag.get("_type") != "tag" or tag.get("kind") not in {"function", "method"}:
            continue
        if "end" not in tag or "line" not in tag:
            continue
        path = tag["path"]
        if path not in file_cache:
            try:
                file_cache[path] = Path(path).read_text(errors="replace").splitlines()
            except OSError:
                continue
        lines = file_cache[path]
        start = int(tag["line"])
        end = min(int(tag["end"]), len(lines))
        if start < 1 or end < start:
            continue
        body_lines = lines[start - 1 : end]
        joined = "\n".join(body_lines)
        brace = joined.find("{")
        header = joined if brace < 0 else joined[:brace]
        body = "" if brace < 0 else joined[brace + 1 :]
        calls = []
        for match in CALL_RE.finditer(body):
            called = match.group(1).split("::")[-1]
            if called not in CONTROL_NAMES:
                calls.append(called)
        functions.append(
            Function(
                ident=len(functions),
                name=tag["name"],
                scope=tag.get("scope", ""),
                path=path,
                line=start,
                end=end,
                header=re.sub(r"\s+", " ", header).strip(),
                noexcept_kind=classify_noexcept(tag["name"], header),
                calls=tuple(sorted(set(calls))),
            )
        )
    return functions


def shortest_path(functions: list[Function], start: Function, max_depth: int) -> tuple[list[Function], str] | None:
    all_points = POSIX_REQUIRED | POSIX_MAY | GLIBC_EXTENSION_SEEDS
    by_name: dict[str, list[Function]] = defaultdict(list)
    for function in functions:
        by_name[function.name].append(function)
    queue: deque[tuple[Function, list[Function]]] = deque([(start, [start])])
    seen = {start.ident}
    while queue:
        current, path = queue.popleft()
        for called in current.calls:
            if called in all_points:
                return path, called
            if len(path) >= max_depth:
                continue
            for target in by_name.get(called, []):
                if target.ident not in seen:
                    seen.add(target.ident)
                    queue.append((target, path + [target]))
    return None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--libcxx-root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--direct-output", required=True, type=Path)
    parser.add_argument("--points-output", required=True, type=Path)
    parser.add_argument("--max-depth", type=int, default=8)
    args = parser.parse_args()

    functions = load_functions(args.libcxx_root)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.direct_output.parent.mkdir(parents=True, exist_ok=True)
    args.points_output.parent.mkdir(parents=True, exist_ok=True)
    points = POSIX_REQUIRED | POSIX_MAY | GLIBC_EXTENSION_SEEDS

    with args.points_output.open("w") as stream:
        stream.write("name\tcategory\tsource_status\n")
        for name in sorted(POSIX_REQUIRED):
            stream.write(f"{name}\tPOSIX_REQUIRED\tpthreads(7), POSIX.1-2001/2008\n")
        for name in sorted(POSIX_MAY):
            stream.write(f"{name}\tPOSIX_MAY\tpthreads(7), POSIX.1-2001/2008\n")
        for name in sorted(GLIBC_EXTENSION_SEEDS):
            stream.write(f"{name}\tGLIBC_EXTENSION_SEED\tconservative Linux/glibc seed; source verification required\n")

    with args.direct_output.open("w") as stream:
        stream.write("function\tlocation\tnoexcept_kind\tcancellation_point\tcategory\n")
        for function in functions:
            for called in sorted(set(function.calls) & points):
                category = (
                    "POSIX_REQUIRED" if called in POSIX_REQUIRED else
                    "POSIX_MAY" if called in POSIX_MAY else "GLIBC_EXTENSION_SEED"
                )
                stream.write(
                    f"{function.qualified}\t{function.path}:{function.line}-{function.end}\t"
                    f"{function.noexcept_kind}\t{called}\t{category}\n"
                )

    with args.output.open("w") as stream:
        stream.write("function\tlocation\tnoexcept_kind\tpath_depth\tcancellation_point\tpath\n")
        for function in functions:
            if function.noexcept_kind not in {"EXPLICIT", "IMPLICIT_DESTRUCTOR_REVIEW"}:
                continue
            result = shortest_path(functions, function, args.max_depth)
            if result is None:
                continue
            path, point = result
            rendered = " -> ".join(f"{item.qualified}@{item.path}:{item.line}" for item in path)
            stream.write(
                f"{function.qualified}\t{function.path}:{function.line}-{function.end}\t"
                f"{function.noexcept_kind}\t{len(path)}\t{point}\t{rendered} -> {point}\n"
            )

    print(f"functions={len(functions)}")
    print(f"direct_rows={sum(1 for _ in args.direct_output.open()) - 1}")
    print(f"candidate_rows={sum(1 for _ in args.output.open()) - 1}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
