#!/usr/bin/env python3
"""Authenticated, credential-safe QuickBuild log discovery and download tool."""

from __future__ import annotations

import argparse
import hashlib
import html
import http.client
import http.cookiejar
import json
import os
import re
import stat
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from dataclasses import asdict, dataclass
from html.parser import HTMLParser
from pathlib import Path
from typing import Iterable


BASE_URL = "https://quickbuild.tizen.org"
USER_AGENT = "quickbuild-logs/1.0"
LOGIN_MARKERS = ("Login to QuickBuild", 'id="sign-in-page"', "name=\"userName\"")
DOWNLOAD_HINTS = (
    "log",
    "report",
    "download",
    "builddata",
    "gbs_reports",
    "error",
    "failure",
)
DOWNLOAD_SUFFIXES = (
    ".log",
    ".txt",
    ".xml",
    ".json",
    ".yaml",
    ".yml",
    ".gz",
    ".bz2",
    ".xz",
    ".zip",
    ".tar",
    ".html",
)


class ToolError(RuntimeError):
    """Expected, user-actionable tool error."""


@dataclass(frozen=True)
class Link:
    source_url: str
    url: str
    label: str
    kind: str
    package_hint: str
    status: str = ""
    architecture: str = ""
    build_ref: str = ""


class FormAndLinkParser(HTMLParser):
    def __init__(self) -> None:
        super().__init__(convert_charrefs=True)
        self.forms: list[dict[str, object]] = []
        self.links: list[tuple[str, str]] = []
        self.iframes: list[str] = []
        self._form: dict[str, object] | None = None
        self._anchor_href: str | None = None
        self._anchor_text: list[str] = []

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        values = dict(attrs)
        if tag == "form":
            self._form = {
                "action": values.get("action", ""),
                "method": (values.get("method") or "get").lower(),
                "inputs": {},
            }
        elif tag == "input" and self._form is not None:
            name = values.get("name")
            if name:
                inputs = self._form["inputs"]
                assert isinstance(inputs, dict)
                inputs[name] = values.get("value") or ""
        elif tag == "a":
            self._anchor_href = values.get("href")
            self._anchor_text = [values.get("title") or ""]
        elif tag == "iframe" and values.get("src"):
            self.iframes.append(values["src"] or "")

    def handle_data(self, data: str) -> None:
        if self._anchor_href is not None:
            self._anchor_text.append(data)

    def handle_endtag(self, tag: str) -> None:
        if tag == "form" and self._form is not None:
            self.forms.append(self._form)
            self._form = None
        elif tag == "a" and self._anchor_href is not None:
            label = " ".join("".join(self._anchor_text).split())
            self.links.append((self._anchor_href, label))
            self._anchor_href = None
            self._anchor_text = []


class GBSReportParser(HTMLParser):
    """Extract package/status/log triples from a static GBS report index."""

    def __init__(self) -> None:
        super().__init__(convert_charrefs=True)
        self.rows: list[tuple[str, str, str, str]] = []
        self._in_row = False
        self._in_cell = False
        self._cells: list[str] = []
        self._cell_text: list[str] = []
        self._row_links: list[tuple[str, str, str]] = []
        self._anchor: dict[str, str] | None = None
        self._anchor_text: list[str] = []

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        values = dict(attrs)
        if tag == "tr":
            self._in_row = True
            self._cells = []
            self._row_links = []
        elif tag == "td" and self._in_row:
            self._in_cell = True
            self._cell_text = []
        elif tag == "a" and self._in_cell and values.get("href"):
            self._anchor = {
                "href": values["href"] or "",
                "class": values.get("class") or "",
            }
            self._anchor_text = []

    def handle_data(self, data: str) -> None:
        if self._in_cell:
            self._cell_text.append(data)
        if self._anchor is not None:
            self._anchor_text.append(data)

    def handle_endtag(self, tag: str) -> None:
        if tag == "a" and self._anchor is not None:
            self._row_links.append(
                (
                    self._anchor["href"],
                    clean_label("".join(self._anchor_text)),
                    self._anchor["class"],
                )
            )
            self._anchor = None
            self._anchor_text = []
        elif tag == "td" and self._in_cell:
            self._cells.append(clean_label("".join(self._cell_text)))
            self._in_cell = False
            self._cell_text = []
        elif tag == "tr" and self._in_row:
            for href, label, css_class in self._row_links:
                if "/logs/" not in href and not href.startswith("logs/"):
                    continue
                package = self._cells[0] if self._cells else ""
                build_ref = self._cells[1] if len(self._cells) > 1 else ""
                status = "failed" if "fail" in f"{css_class} {label}".lower() else "succeeded"
                self.rows.append((package, build_ref, status, href))
            self._in_row = False
            self._cells = []
            self._row_links = []


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_credentials(path: Path) -> tuple[str, str]:
    if not path.exists():
        raise ToolError(
            f"凭据文件不存在：{path}\n"
            "请创建权限为 600 的两行文件：第一行用户名，第二行密码。"
        )
    if path.is_symlink() or not path.is_file():
        raise ToolError(f"凭据路径必须是普通文件且不能是符号链接：{path}")
    mode = stat.S_IMODE(path.stat().st_mode)
    if mode != 0o600:
        raise ToolError(f"凭据文件权限必须为 600；当前为 {mode:o}：{path}")
    if path.stat().st_uid != os.getuid():
        raise ToolError(f"凭据文件所有者必须是当前用户：{path}")
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except UnicodeDecodeError as exc:
        raise ToolError(f"凭据文件必须是 UTF-8 文本：{path}") from exc
    if len(lines) != 2 or not lines[0] or not lines[1]:
        raise ToolError("凭据文件必须恰好包含两个非空行：第一行用户名，第二行密码。")
    return lines[0], lines[1]


def is_login_page(url: str, body: bytes) -> bool:
    text = body.decode("utf-8", errors="replace")
    return "/signin" in urllib.parse.urlparse(url).path or any(x in text for x in LOGIN_MARKERS)


def decode_html(body: bytes, content_type: str | None) -> str:
    charset = "utf-8"
    if content_type:
        match = re.search(r"charset=([^;\s]+)", content_type, re.I)
        if match:
            charset = match.group(1).strip('"\'')
    try:
        return body.decode(charset, errors="replace")
    except LookupError:
        return body.decode("utf-8", errors="replace")


class QuickBuildSession:
    def __init__(self, username: str, password: str, timeout: int) -> None:
        self._username = username
        self._password = password
        self.timeout = timeout
        self.cookies = http.cookiejar.CookieJar()
        self.opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(self.cookies))
        self._authenticated = False
        self._credential_bytes = tuple(
            value.encode("utf-8") for value in (username, password) if value.encode("utf-8")
        )

    def redact_text(self, value: str) -> str:
        redacted = value
        for secret in (self._username, self._password):
            if secret:
                redacted = redacted.replace(secret, "[REDACTED]")
        return redacted

    def _redact_bytes(self, value: bytes) -> tuple[bytes, int]:
        redacted = value
        count = 0
        for secret in self._credential_bytes:
            occurrences = redacted.count(secret)
            if occurrences:
                redacted = redacted.replace(secret, b"*" * len(secret))
                count += occurrences
        return redacted, count

    def file_contains_credentials(self, path: Path) -> bool:
        hold = max((len(secret) for secret in self._credential_bytes), default=1) - 1
        pending = b""
        with path.open("rb") as source:
            for chunk in iter(lambda: source.read(1024 * 1024), b""):
                pending += chunk
                if any(secret in pending for secret in self._credential_bytes):
                    return True
                pending = pending[-hold:] if hold else b""
        return False

    def _request(
        self, url: str, data: bytes | None = None, headers: dict[str, str] | None = None
    ) -> tuple[str, dict[str, str], bytes]:
        request_headers = {"User-Agent": USER_AGENT, "Accept-Encoding": "identity"}
        if headers:
            request_headers.update(headers)
        request = urllib.request.Request(url, data=data, headers=request_headers)
        try:
            with self.opener.open(request, timeout=self.timeout) as response:
                return response.geturl(), dict(response.headers.items()), response.read()
        except urllib.error.HTTPError as exc:
            raise ToolError(f"HTTP 请求失败：{exc.code} {exc.reason}；URL={url}") from exc
        except urllib.error.URLError as exc:
            raise ToolError(f"网络请求失败：{exc.reason}；URL={url}") from exc
        except (http.client.InvalidURL, ValueError) as exc:
            raise ToolError(f"URL 无效，工具未发出请求：{url}") from exc

    def _login(self, login_url: str, login_body: bytes, content_type: str | None) -> None:
        parser = FormAndLinkParser()
        parser.feed(decode_html(login_body, content_type))
        form = next(
            (
                item
                for item in parser.forms
                if "userName" in item["inputs"] and "password" in item["inputs"]
            ),
            None,
        )
        if form is None:
            raise ToolError("登录页中未找到预期的用户名/密码表单；站点登录流程可能已变化。")
        if form["method"] != "post":
            raise ToolError("登录表单不是 POST；为避免误送凭据，工具已停止。")
        fields = dict(form["inputs"])
        fields["userName"] = self._username
        fields["password"] = self._password
        fields.pop("remember", None)
        action = urllib.parse.urljoin(login_url, html.unescape(str(form["action"])))
        encoded = urllib.parse.urlencode(fields).encode("utf-8")
        final_url, headers, body = self._request(
            action,
            data=encoded,
            headers={
                "Content-Type": "application/x-www-form-urlencoded",
                "Referer": login_url,
            },
        )
        if is_login_page(final_url, body):
            raise ToolError("QuickBuild 登录失败；请核对凭据或账号权限。工具未记录服务端响应体。")
        self._authenticated = True

    def get(self, url: str) -> tuple[str, dict[str, str], bytes]:
        final_url, headers, body = self._request(url)
        if is_login_page(final_url, body):
            if self._authenticated:
                raise ToolError("已认证会话被重定向回登录页；会话可能过期或无权访问该资源。")
            self._login(final_url, body, headers.get("Content-Type"))
            final_url, headers, body = self._request(url)
            if is_login_page(final_url, body):
                raise ToolError("登录后仍无法访问目标资源；请确认账号权限。")
        return final_url, headers, body

    def download(self, url: str, destination: Path, resume: bool) -> tuple[int, str, int, str]:
        destination.parent.mkdir(parents=True, exist_ok=True)
        part = destination.with_name(destination.name + ".part")
        part.touch(mode=0o600, exist_ok=True)
        part.chmod(0o600)
        start = part.stat().st_size if resume and part.exists() else 0
        headers = {"User-Agent": USER_AGENT, "Accept-Encoding": "identity"}
        if start:
            headers["Range"] = f"bytes={start}-"
        request = urllib.request.Request(url, headers=headers)
        try:
            response = self.opener.open(request, timeout=self.timeout)
        except urllib.error.HTTPError as exc:
            raise ToolError(f"日志下载失败：HTTP {exc.code} {exc.reason}；URL={url}") from exc
        except urllib.error.URLError as exc:
            raise ToolError(f"日志下载失败：{exc.reason}；URL={url}") from exc
        except (http.client.InvalidURL, ValueError) as exc:
            raise ToolError(f"日志 URL 无效，工具未发出请求：{url}") from exc
        with response:
            final_url = response.geturl()
            content_type = response.headers.get("Content-Type", "")
            prefix = response.read(8192)
            if is_login_page(final_url, prefix):
                raise ToolError("下载请求返回登录页；会话可能过期或账号无权访问。")
            append = start > 0 and response.status == 206
            transfer_mode = "resumed" if append else "fresh"
            if start > 0 and response.status != 206:
                transfer_mode = "restarted_range_unsupported"
                start = 0
            mode = "ab" if append else "wb"
            digest = hashlib.sha256()
            if append:
                if self.file_contains_credentials(part):
                    raise ToolError("现有 .part 文件含凭据字符串；为安全起见拒绝续传，请人工安全删除该文件。")
                with part.open("rb") as existing:
                    for chunk in iter(lambda: existing.read(1024 * 1024), b""):
                        digest.update(chunk)
            pending = b""
            redaction_count = 0
            hold = max((len(secret) for secret in self._credential_bytes), default=1) - 1
            with part.open(mode) as output:
                total = start
                chunk = prefix
                while chunk:
                    pending += chunk
                    pending, found = self._redact_bytes(pending)
                    redaction_count += found
                    emit_size = max(0, len(pending) - hold)
                    emitted, pending = pending[:emit_size], pending[emit_size:]
                    output.write(emitted)
                    digest.update(emitted)
                    total += len(emitted)
                    chunk = response.read(1024 * 1024)
                pending, found = self._redact_bytes(pending)
                redaction_count += found
                output.write(pending)
                digest.update(pending)
                total += len(pending)
        part.replace(destination)
        return total, digest.hexdigest(), redaction_count, transfer_mode


def clean_label(value: str) -> str:
    return " ".join(value.replace("\t", " ").replace("\n", " ").split())


def package_hint(url: str, label: str) -> str:
    candidates = re.findall(r"[A-Za-z0-9][A-Za-z0-9+_.-]{1,100}", f"{label} {url}")
    ignored = {
        "http",
        "https",
        "quickbuild.tizen.org",
        "build",
        "download",
        "log",
        "logs",
        "report",
        "reports",
        "gbs_reports",
    }
    for candidate in candidates:
        if candidate.lower() not in ignored and not candidate.isdigit():
            return candidate
    return ""


def classify_link(url: str, label: str) -> str:
    parsed = urllib.parse.urlparse(url)
    text = f"{parsed.path} {parsed.query} {label}".lower()
    if "save full log" in label.lower() or "buildtab-panel-download" in text:
        return "build_full_log"
    if "view step log" in label.lower() or "loglink" in text:
        return "step_log"
    if "gbs_reports" in text:
        return "gbs_report_page"
    if parsed.path.lower().endswith(DOWNLOAD_SUFFIXES) or any(hint in text for hint in DOWNLOAD_HINTS):
        return "download_candidate"
    if parsed.path.startswith("/build/"):
        return "build_navigation"
    return "other"


def normalize_link(source_url: str, href: str) -> str | None:
    href = html.unescape(href.strip())
    if not href or href.startswith(("#", "javascript:", "mailto:")):
        return None
    url = urllib.parse.urljoin(source_url, href)
    parsed = urllib.parse.urlparse(url)
    if parsed.scheme not in ("http", "https") or parsed.netloc != "quickbuild.tizen.org":
        return None
    encoded_path = urllib.parse.quote(urllib.parse.unquote(parsed.path), safe="/:@")
    return urllib.parse.urlunparse(parsed._replace(path=encoded_path, fragment=""))


def report_architecture(url: str) -> str:
    decoded = urllib.parse.unquote(url)
    match = re.search(r"GBS Reports@\^@([^/]+)/", decoded)
    return match.group(1) if match else ""


def discover_links(session: QuickBuildSession, start_url: str, build_id: str, max_pages: int) -> list[Link]:
    queue = [
        start_url,
        f"{start_url}/overview",
        f"{start_url}/step_status",
        f"{start_url}/log",
        f"{start_url}/gbs_reports",
    ]
    visited: set[str] = set()
    discovered: dict[str, Link] = {}
    build_prefix = f"/build/{build_id}"
    while queue:
        if len(visited) >= max_pages:
            raise ToolError(f"页面数量超过安全上限 {max_pages}；请缩小范围或显式提高 --max-pages。")
        url = queue.pop(0)
        if url in visited:
            continue
        visited.add(url)
        final_url, headers, body = session.get(url)
        content_type = headers.get("Content-Type", "")
        if "html" not in content_type.lower():
            discovered[url] = Link(url, final_url, Path(urllib.parse.urlparse(final_url).path).name, "download_candidate", "")
            continue
        parser = FormAndLinkParser()
        parser.feed(decode_html(body, content_type))
        for href, raw_label in parser.links:
            linked = normalize_link(final_url, href)
            if linked is None:
                continue
            label = clean_label(raw_label)
            kind = classify_link(linked, label)
            link = Link(final_url, linked, label, kind, package_hint(linked, label))
            discovered.setdefault(linked, link)
            parsed = urllib.parse.urlparse(linked)
            if (
                parsed.path.startswith(build_prefix)
                and kind in {"build_navigation", "gbs_report_page"}
                and linked not in visited
                and linked not in queue
            ):
                queue.append(linked)
        for raw_src in parser.iframes:
            report_url = normalize_link(final_url, raw_src)
            if report_url is None or "/download/" not in report_url:
                continue
            architecture = report_architecture(report_url)
            report_link = Link(
                final_url,
                report_url,
                f"GBS report index {architecture}".strip(),
                "gbs_report_index",
                "",
                architecture=architecture,
            )
            discovered.setdefault(report_url, report_link)
            report_final, report_headers, report_body = session.get(report_url)
            if "html" not in report_headers.get("Content-Type", "").lower():
                continue
            report_parser = GBSReportParser()
            report_parser.feed(decode_html(report_body, report_headers.get("Content-Type")))
            for package, build_ref, status, href in report_parser.rows:
                package_url = normalize_link(report_final, href)
                if package_url is None:
                    continue
                discovered.setdefault(
                    package_url,
                    Link(
                        report_final,
                        package_url,
                        f"{package} {status}",
                        "package_log",
                        package,
                        status=status,
                        architecture=architecture,
                        build_ref=build_ref,
                    ),
                )
    return sorted(discovered.values(), key=lambda item: (item.kind, item.url))


def filter_links(links: Iterable[Link], package: str | None, mode: str) -> list[Link]:
    selected = []
    needle = package.lower() if package else None
    for link in links:
        haystack = f"{link.package_hint} {link.label} {link.url}".lower()
        if needle and needle not in haystack:
            continue
        if mode == "gbs-reports" and link.kind not in {"gbs_report_index", "package_log"}:
            continue
        if mode == "logs" and link.kind not in {"build_full_log", "step_log", "package_log"}:
            continue
        selected.append(link)
    return selected


def safe_filename(link: Link, index: int) -> str:
    parsed = urllib.parse.urlparse(link.url)
    name = Path(parsed.path.rstrip("/")).name or f"item-{index:04d}.html"
    if parsed.query:
        query_hash = hashlib.sha256(parsed.query.encode("utf-8")).hexdigest()[:10]
        stem, suffix = os.path.splitext(name)
        name = f"{stem}-{query_hash}{suffix}"
    name = re.sub(r"[^A-Za-z0-9+_.-]", "_", name)
    prefix = "_".join(value for value in (link.architecture, link.package_hint, link.status) if value)
    prefix = re.sub(r"[^A-Za-z0-9+_.-]", "_", prefix)
    return f"{index:04d}_{prefix + '_' if prefix else ''}{name}"


def print_links(links: Iterable[Link], session: QuickBuildSession) -> None:
    print("kind\tarchitecture\tstatus\tpackage\tbuild_ref\tlabel\turl\tsource_url")
    for link in links:
        print(
            "\t".join(
                session.redact_text(clean_label(value))
                for value in (
                    link.kind,
                    link.architecture,
                    link.status,
                    link.package_hint,
                    link.build_ref,
                    link.label,
                    link.url,
                    link.source_url,
                )
            )
        )
    sys.stdout.flush()


def write_manifest(
    path: Path, build_id: str, records: list[dict[str, object]], session: QuickBuildSession
) -> None:
    safe_records = []
    for record in records:
        safe_records.append(
            {
                key: session.redact_text(value) if isinstance(value, str) else value
                for key, value in record.items()
            }
        )
    payload = {
        "build_id": build_id,
        "created_epoch": int(time.time()),
        "records": safe_records,
    }
    path.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    path.chmod(0o600)


def secure_output_directory(root: Path, build_id: str) -> Path:
    logs_root = root / "logs"
    quickbuild_root = logs_root / "quickbuild"
    output_dir = quickbuild_root / f"build_{build_id}"
    logs_root.mkdir(mode=0o700, exist_ok=True)
    quickbuild_root.mkdir(mode=0o700, exist_ok=True)
    output_dir.mkdir(mode=0o700, exist_ok=True)
    quickbuild_root.chmod(0o700)
    output_dir.chmod(0o700)
    return output_dir


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="安全登录 QuickBuild，列出并下载构建日志；凭据只从仓库根目录的文件读取。"
    )
    parser.add_argument("build_id", help="QuickBuild 构建号，例如 1165448")
    parser.add_argument("--timeout", type=int, default=30, help="单次网络请求超时秒数，默认 30")
    parser.add_argument("--max-pages", type=int, default=200, help="发现阶段最多读取的 HTML 页面数")
    subparsers = parser.add_subparsers(dest="command", required=True)

    list_parser = subparsers.add_parser("list", help="只列出可见链接，不下载")
    list_parser.add_argument("--package", help="按包名或链接文本过滤")
    list_parser.add_argument(
        "--scope", choices=("all", "logs", "gbs-reports"), default="logs", help="列出范围"
    )

    download_parser = subparsers.add_parser("download", help="先发现并显示，再下载选中内容")
    selection = download_parser.add_mutually_exclusive_group(required=True)
    selection.add_argument("--package", help="下载与包名匹配的日志")
    selection.add_argument("--failed", action="store_true", help="下载页面标记为失败的包日志")
    selection.add_argument("--gbs-reports", action="store_true", help="下载 gbs_reports 内容")
    selection.add_argument("--all", action="store_true", help="下载发现到的全部日志候选")
    download_parser.add_argument("--resume", action="store_true", help="对 .part 文件使用 HTTP Range 续传")
    download_parser.add_argument("--yes", action="store_true", help="确认下载；无此选项时只显示计划")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if not re.fullmatch(r"[0-9]+", args.build_id):
        raise ToolError("构建号只能包含十进制数字。")
    root = repo_root()
    credentials = root / ".quickbuild-credentials"
    username, password = read_credentials(credentials)
    session = QuickBuildSession(username, password, args.timeout)
    start_url = f"{BASE_URL}/build/{args.build_id}"
    links = discover_links(session, start_url, args.build_id, args.max_pages)

    if args.command == "list":
        selected = filter_links(links, args.package, args.scope)
        print_links(selected, session)
        print(f"# discovered={len(links)} selected={len(selected)}", file=sys.stderr)
        return 0

    package = args.package if args.package else None
    scope = "gbs-reports" if args.gbs_reports else "logs"
    selected = filter_links(links, package, scope)
    if args.failed:
        selected = [link for link in selected if link.kind == "package_log" and link.status == "failed"]
    print_links(selected, session)
    if not selected:
        raise ToolError("没有发现符合条件的可下载项；请先使用 list --scope all 检查页面结构。")
    if not args.yes:
        print("仅显示下载计划；确认后加 --yes 执行。", file=sys.stderr)
        return 0

    output_dir = secure_output_directory(root, args.build_id)
    records: list[dict[str, object]] = []
    for index, link in enumerate(selected, start=1):
        destination = output_dir / safe_filename(link, index)
        if destination.exists():
            destination.chmod(0o600)
            if session.file_contains_credentials(destination):
                raise ToolError(
                    f"已有文件含凭据字符串，拒绝继续使用；请人工安全删除后重试：{destination}"
                )
            digest = file_sha256(destination)
            records.append(
                {
                    **asdict(link),
                    "path": str(destination),
                    "download_status": "skipped_existing",
                    "size": destination.stat().st_size,
                    "sha256": digest,
                }
            )
            print(f"SKIP {session.redact_text(str(destination))}", file=sys.stderr)
            continue
        size, digest, credential_redactions, transfer_mode = session.download(
            link.url, destination, args.resume
        )
        records.append(
            {
                **asdict(link),
                "path": str(destination),
                "download_status": "downloaded",
                "size": size,
                "sha256": digest,
                "credential_redactions": credential_redactions,
                "transfer_mode": transfer_mode,
            }
        )
        print(
            f"OK {size} {digest} mode={transfer_mode} redactions={credential_redactions} "
            f"{session.redact_text(str(destination))}",
            file=sys.stderr,
        )
    write_manifest(output_dir / "manifest.json", args.build_id, records, session)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except ToolError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(2)
