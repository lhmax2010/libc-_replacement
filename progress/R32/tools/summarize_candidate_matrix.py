#!/usr/bin/env python3
import csv
import re
from pathlib import Path

root = Path(__file__).resolve().parents[3]
indir = root / "progress/R32/results/native_matrix"
out = root / "progress/R32/tables/candidate_behavior_matrix.tsv"

rows = []
for path in sorted(indir.glob("*.log")):
    text = path.read_text(errors="replace")
    m = re.match(r"(libstdcxx|libcxx)_(.+)\.log$", path.name)
    stdlib, case = m.groups()
    def one(pattern, default="NOT_OBSERVED"):
        x = re.search(pattern, text, re.M)
        return x.group(1) if x else default
    exit_code = one(r"^EXIT_CODE=(\d+)$")
    signal = one(r"^SIGNAL_NAME=(.+)$", "NOT_RECORDED")
    cxa_type = one(r"cxa_type=([^ ]+)")
    current_exception = one(r"current_exception=(\d+)")
    canceled = one(r"RESULT .*canceled=(\d+)")
    cleanup = one(r"RESULT .*cleanup=(\d+)")
    if exit_code == "0":
        result = "OBSERVED_EXIT_0"
    elif exit_code == "77":
        result = "NOT_AVAILABLE_TYPE_ABSENT"
    elif exit_code == "134":
        result = "OBSERVED_SIGABRT"
    else:
        result = "OBSERVED_OTHER_EXIT"
    events = " | ".join(re.findall(r"^EVENT (.+)$", text, re.M))
    rows.append((stdlib, case, exit_code, signal, result, canceled, cleanup,
                 cxa_type, current_exception, events, str(path.relative_to(root))))

out.parent.mkdir(parents=True, exist_ok=True)
with out.open("w", newline="") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["standard_library", "case", "exit_code", "signal", "observed_result",
                "pthread_join_canceled", "cleanup_count", "cxa_current_exception_type",
                "std_current_exception_nonempty", "events", "raw_log"])
    w.writerows(rows)

summary = root / "progress/R32/tables/candidate_direction_summary.tsv"
data = {
    "A": ("NO", "libstdc++: detect+rethrow exit 0; libc++: detect+rethrow SIGABRT/terminate",
          "real runtime_error and int both observed as cxa_type non-NULL/current_exception=1 and retained by predicate"),
    "B": ("NO", "both libraries: forced unwind bypassed std::exception catch, join=PTHREAD_CANCELED, exit 0",
          "runtime_error retained; int bypassed the specific catch and reached outer observer"),
    "C": ("NO", "both libraries: request deferred until restore; if never restored worker returned normally",
          "cancellation timing/termination semantics changed; real exception handler itself was not altered by this fixture"),
    "D": ("NO", "libstdc++: cancel exit 0; libc++: SIGABRT/terminate on rethrow",
          "runtime_error and int both propagated to outer observer instead of being swallowed"),
}
with summary.open("w", newline="") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["direction", "simultaneously_meets_all_three_observed_conditions", "forced_unwind_observation", "real_exception_semantics_observation"])
    for key in "ABCD":
        w.writerow((key,) + data[key])
