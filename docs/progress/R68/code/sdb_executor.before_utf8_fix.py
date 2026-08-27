#!/usr/bin/env python3
"""Run one libc++ lit test directory on the registered armv7l SDB target."""

import argparse
import hashlib
import os
import re
import shlex
import subprocess
import sys
import tarfile
import tempfile


SDB = "/home/toolchain/.local/bin/sdb"
SERIAL = "192.168.108.26:26101"
REMOTE_ROOT = "/var/tmp/r68_lit_20260827"
SHARED_ROOT = "/opt/usr/home/owner/share/tmp"
EXIT_MARKER = "__R68_SDB_EXIT__="


def run_host(command, *, check=True, **kwargs):
    if os.environ.get("R68_SDB_EXECUTOR_VERBOSE") == "1":
        print(f"HOST_COMMAND={shlex.join(command)}", file=sys.stderr)
    return subprocess.run(command, check=check, **kwargs)


def sdb_command(*arguments):
    return [SDB, "-s", SERIAL, *arguments]


def build_environment(env_args, prepend_args):
    components = []
    for assignment in env_args:
        key, value = assignment.split("=", 1)
        components.append(f"export {key}={shlex.quote(value)}; ")
    for assignment in prepend_args:
        key, value = assignment.split("=", 1)
        components.append(
            f"export {key}={shlex.quote(value)}${{{key}:+:${key}}}; "
        )
    components.append(
        f"export LD_LIBRARY_PATH={shlex.quote(REMOTE_ROOT + '/runtime')}"
        "${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}; "
    )
    return "".join(components)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--execdir", required=True)
    parser.add_argument("--env", nargs="*", default=[])
    parser.add_argument("--prepend_env", nargs="*", default=[])
    parser.add_argument("command", nargs=argparse.ONE_OR_MORE)
    args = parser.parse_args()

    local_dir = os.path.abspath(args.execdir)
    if not os.path.isdir(local_dir):
        print(f"ERROR: execdir is not a directory: {local_dir}", file=sys.stderr)
        return 10

    digest = hashlib.sha256(local_dir.encode()).hexdigest()[:16]
    invocation = f"run-{digest}-{os.getpid()}"
    remote_dir = f"{REMOTE_ROOT}/runs/{invocation}"
    shared_tar = f"{SHARED_ROOT}/{invocation}.tar"
    remote_tar = f"{REMOTE_ROOT}/{invocation}.tar"
    local_tar = None
    local_tar_bytes = 0

    def map_argument(argument):
        prefix = local_dir + os.sep
        if argument.startswith(prefix):
            return remote_dir + "/" + argument[len(prefix) :]
        return argument

    try:
        with tempfile.NamedTemporaryFile(suffix=".tar", delete=False) as temporary:
            local_tar = temporary.name
        with tarfile.open(local_tar, "w") as archive:
            archive.add(local_dir, arcname=os.path.basename(local_dir))
        local_tar_bytes = os.path.getsize(local_tar)

        run_host(
            sdb_command("push", local_tar, shared_tar),
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )

        setup = (
            "set -e; "
            f"mkdir -p {shlex.quote(REMOTE_ROOT + '/runs')}; "
            f"cp {shlex.quote(shared_tar)} {shlex.quote(remote_tar)}; "
            f"rm -f {shlex.quote(shared_tar)}; "
            f"mkdir {shlex.quote(remote_dir)}; "
            f"tar -xf {shlex.quote(remote_tar)} -C {shlex.quote(remote_dir)} "
            "--strip-components 1; "
            f"rm -f {shlex.quote(remote_tar)}; "
            f"chmod -R u+rwX {shlex.quote(remote_dir)}"
        )
        run_host(
            sdb_command("shell", setup),
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )

        remote_command = [map_argument(argument) for argument in args.command]
        shell_command = (
            "set +e; "
            + build_environment(args.env, args.prepend_env)
            + f"cd {shlex.quote(remote_dir)}; "
            + shlex.join(remote_command)
            + "; rc=$?; "
            + f"printf '\n{EXIT_MARKER}%d\n' \"$rc\"; exit 0"
        )
        completed = run_host(
            sdb_command("shell", shell_command),
            check=False,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        telemetry_path = os.environ.get("R68_SDB_TELEMETRY")
        if telemetry_path:
            usage = run_host(
                sdb_command("shell", f"du -sk {shlex.quote(remote_dir)}"),
                check=False,
                stdin=subprocess.DEVNULL,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
            )
            with open(telemetry_path, "a", encoding="utf-8") as telemetry:
                telemetry.write(
                    f"{local_dir}\t{local_tar_bytes}\t"
                    f"{usage.returncode}\t{usage.stdout.strip()}\n"
                )
        if completed.stderr:
            sys.stderr.write(completed.stderr)
        if completed.returncode != 0:
            sys.stderr.write(
                f"ERROR: SDB carrier exited with {completed.returncode}\n"
            )
            sys.stderr.write(completed.stdout)
            return 90

        output = completed.stdout.replace("\r\n", "\n")
        match = re.search(rf"\n{re.escape(EXIT_MARKER)}([0-9]+)\n?$", output)
        if match is None:
            sys.stderr.write("ERROR: remote exit marker was not observed\n")
            sys.stderr.write(output)
            return 91
        sys.stdout.write(output[: match.start()])
        return int(match.group(1))
    finally:
        if local_tar is not None:
            try:
                os.remove(local_tar)
            except FileNotFoundError:
                pass
        cleanup = (
            f"rm -rf {shlex.quote(remote_dir)}; "
            f"rm -f {shlex.quote(remote_tar)} {shlex.quote(shared_tar)}"
        )
        run_host(
            sdb_command("shell", cleanup),
            check=False,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )


if __name__ == "__main__":
    sys.exit(main())
