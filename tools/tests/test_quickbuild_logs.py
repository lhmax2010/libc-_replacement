#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from tools.quickbuild_logs import (
    FormAndLinkParser,
    GBSReportParser,
    QuickBuildSession,
    ToolError,
    classify_link,
    normalize_link,
    read_credentials,
    secure_output_directory,
)


class QuickBuildLogsTests(unittest.TestCase):
    def test_login_form_parser(self) -> None:
        parser = FormAndLinkParser()
        parser.feed(
            '<form method="post" action="signin?x=1&amp;y=2">'
            '<input type="hidden" name="hidden" value="v">'
            '<input type="text" name="userName">'
            '<input type="password" name="password">'
            '</form>'
        )
        self.assertEqual(len(parser.forms), 1)
        self.assertEqual(parser.forms[0]["method"], "post")
        self.assertEqual(parser.forms[0]["inputs"]["hidden"], "v")

    def test_iframe_parser(self) -> None:
        parser = FormAndLinkParser()
        parser.feed('<iframe src="/download/123/html/report/index.html"></iframe>')
        self.assertEqual(parser.iframes, ["/download/123/html/report/index.html"])

    def test_gbs_report_parser(self) -> None:
        parser = GBSReportParser()
        parser.feed(
            "<table>"
            "<tr><td>lapack</td><td>repo/lapack-3.7.1-1</td>"
            '<td><a href="logs/fail/lapack-3.7.1-1/log.txt" class="failed">Failed</a></td></tr>'
            "<tr><td>zlib</td><td>repo/zlib-1.3-1</td>"
            '<td><a href="logs/success/zlib-1.3-1/log.txt" class="succeeded">Succeeded</a></td></tr>'
            "</table>"
        )
        self.assertEqual(
            parser.rows,
            [
                ("lapack", "repo/lapack-3.7.1-1", "failed", "logs/fail/lapack-3.7.1-1/log.txt"),
                ("zlib", "repo/zlib-1.3-1", "succeeded", "logs/success/zlib-1.3-1/log.txt"),
            ],
        )

    def test_url_normalization_and_host_restriction(self) -> None:
        base = "https://quickbuild.tizen.org/build/123/gbs_reports"
        self.assertEqual(
            normalize_link(base, "/download/123/html/GBS Reports@^@standard-armv7l/index.html"),
            "https://quickbuild.tizen.org/download/123/html/GBS%20Reports@%5E@standard-armv7l/index.html",
        )
        self.assertIsNone(normalize_link(base, "https://example.invalid/log.txt"))

    def test_link_classification(self) -> None:
        self.assertEqual(
            classify_link(
                "https://quickbuild.tizen.org/wicket/page?x-logLink", "View step log"
            ),
            "step_log",
        )
        self.assertEqual(
            classify_link(
                "https://quickbuild.tizen.org/wicket/page?x-download", "Save full log as text file"
            ),
            "build_full_log",
        )

    def test_credentials_require_exact_mode_and_two_lines(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "credentials"
            path.write_text("test-user\ntest-password\n", encoding="utf-8")
            path.chmod(0o644)
            with self.assertRaises(ToolError):
                read_credentials(path)
            path.chmod(0o600)
            self.assertEqual(read_credentials(path), ("test-user", "test-password"))

    def test_missing_credentials_are_actionable(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            with self.assertRaisesRegex(ToolError, "凭据文件不存在"):
                read_credentials(Path(directory) / "missing")

    def test_credential_byte_redaction_preserves_length(self) -> None:
        session = QuickBuildSession("test-user", "test-password", 30)
        original = b"owner=test-user secret=test-password"
        redacted, count = session._redact_bytes(original)
        self.assertEqual(count, 2)
        self.assertEqual(len(redacted), len(original))
        self.assertNotIn(b"test-user", redacted)
        self.assertNotIn(b"test-password", redacted)

    def test_output_directory_permissions(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            path = secure_output_directory(Path(directory), "123")
            self.assertEqual(path.stat().st_mode & 0o777, 0o700)
            self.assertEqual(path.parent.stat().st_mode & 0o777, 0o700)


if __name__ == "__main__":
    unittest.main()
