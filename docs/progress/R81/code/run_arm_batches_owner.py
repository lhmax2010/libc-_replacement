#!/usr/bin/env python3
"""Run the archived R77 batch driver with the R81 owner-identity executor."""

import importlib.util
from pathlib import Path


WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
DRIVER = WORKSPACE / "docs/progress/R77/code/run_arm_batches.py"
EXECUTOR = WORKSPACE / "progress/R81/code/sdb_executor_phase2_owner_baseline.sh"

spec = importlib.util.spec_from_file_location("r77_batch_driver", DRIVER)
if spec is None or spec.loader is None:
    raise RuntimeError(f"cannot load batch driver: {DRIVER}")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
module.EXECUTOR = EXECUTOR

raise SystemExit(module.main())
