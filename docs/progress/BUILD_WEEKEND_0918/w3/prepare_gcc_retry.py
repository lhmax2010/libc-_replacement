import json
from pathlib import Path
p=Path('progress/BUILD_WEEKEND_0918/cell_commands')
(p/'bpf-aarch64-gcc-retry.json').write_bytes((p/'bpf-aarch64-gcc.json').read_bytes())
