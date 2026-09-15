import json
import sys
from pathlib import Path

before_path, after_path, target, expected = sys.argv[1:]
def read(path):
    return {ref: sha for line in Path(path).read_text().splitlines() for sha, ref in [line.split()]}
before, after = read(before_path), read(after_path)
assert before.keys() == after.keys(), 'Remote branch set changed'
assert target in before and after[target] == expected
changes = [{"ref": ref, "before": before[ref], "after": after[ref]}
           for ref in sorted(before) if before[ref] != after[ref]]
assert [row['ref'] for row in changes] == [target], changes
print(json.dumps(dict(status='PASS', branch_count=len(before), changes=changes,
    unchanged_existing_branches=len(before)-1, added_branches=0, removed_branches=0)))
