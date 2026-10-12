"""Stop only the first of the two approved commits for message amendment."""
import pathlib
import sys

todo = pathlib.Path(sys.argv[1])
lines = todo.read_text().splitlines(keepends=True)
picks = [(i, line.split()) for i, line in enumerate(lines) if line.startswith('pick ')]
assert len(picks) == 2, picks
assert 'db671becfe0f2826dac7f479d091d5a23feccb47'.startswith(picks[0][1][1])
assert '0e9fb65ce82b6ca54a5b80a93aa6bcd26c40db2a'.startswith(picks[1][1][1])
i = picks[0][0]
lines[i] = lines[i].replace('pick ', 'edit ', 1)
todo.write_text(''.join(lines))
