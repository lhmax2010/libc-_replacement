"""List observed old target labels not repeated under exactly the same label."""
import collections
import re
from pathlib import Path

inputs = [
    (Path('progress/BUILD_W2B_0915/raw/009_rebuild_llvm_armv7l.stdout.txt'), 7147),
    (Path('progress/BUILD_W2C_0915/build-w2c-llvm-confirmed.build.log'), 7146),
]
counts = []
for path, total in inputs:
    counts.append(collections.Counter(
        m[1] for line in path.read_text(errors='replace').splitlines()
        if (m := re.search(r'\[\d+/' + str(total) + r'\] (.*)', line))))
print('old_label_not_matched\tcount')
for label, count in sorted((counts[0] - counts[1]).items()):
    print(f'{label}\t{count}')
