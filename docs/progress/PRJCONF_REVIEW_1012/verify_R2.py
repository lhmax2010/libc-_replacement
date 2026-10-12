"""Read-only checks for the message-only Gerrit patchset update."""
import json
import pathlib
import subprocess

root = pathlib.Path(__file__).resolve().parent
repo = pathlib.Path('tmp/PRJCONF_REVIEW_1012/repo')
commands = []
def git(*args):
    argv = ['git', '-C', str(repo), *args]
    p = subprocess.run(argv, capture_output=True, text=True)
    commands.append(dict(argv=argv, exitcode=p.returncode, stdout=p.stdout, stderr=p.stderr))
    (root / 'VERIFY_COMMANDS_R2.json').write_text(json.dumps(commands, indent=2) + '\n')
    assert p.returncode == 0, commands[-1]
    return p.stdout

base = '0bb6ff66506574606cf4d636aaa8974ff4c779b8'
old = ['db671becfe0f2826dac7f479d091d5a23feccb47', '0e9fb65ce82b6ca54a5b80a93aa6bcd26c40db2a']
ids = ['I0b3f8e75e3664d9e044eeba972d7d4669c59a342', 'I0403eb61b5c412c7ed53dc6f862c92765ead121f']
assert git('rev-parse', 'origin/master').strip() == base
assert git('branch', '--show-current').strip() == 'prjconf-libcxx'
assert git('status', '--porcelain') == ''
new = git('rev-list', '--reverse', 'origin/master..HEAD').splitlines()
assert len(new) == 2
rows = []
for i, (before, after) in enumerate(zip(old, new)):
    before_tree = git('rev-parse', before + '^{tree}').strip()
    after_tree = git('rev-parse', after + '^{tree}').strip()
    assert before_tree == after_tree
    assert git('diff', '--exit-code', before, after) == ''
    expected_parent = base if i == 0 else new[0]
    assert git('rev-parse', after + '^').strip() == expected_parent
    author = git('show', '-s', '--format=%an <%ae>', after).strip()
    committer = git('show', '-s', '--format=%cn <%ce>', after).strip()
    assert author == committer == 'Hao Lin <hao.lin@samsung.com>'
    message = git('show', '-s', '--format=%B', after).rstrip('\n')
    assert message.count('Change-Id: ') == 1
    assert 'Change-Id: ' + ids[i] in message.splitlines()
    assert message.count('Signed-off-by: ') == 1
    assert 'Signed-off-by: Hao Lin <hao.lin@samsung.com>' in message.splitlines()
    if i == 0:
        assert message == (root / 'REVERT_MESSAGE_R2.txt').read_text().rstrip('\n')
    else:
        assert message == git('show', '-s', '--format=%B', before).rstrip('\n')
    rows.append(dict(old=before, new=after, tree=after_tree, diff_empty=True,
                     author=author, committer=committer, change_id=ids[i],
                     signed_off_by='Hao Lin <hao.lin@samsung.com>'))
git('diff', '--check', 'origin/master..HEAD')
result = dict(result='PASS', baseline=base, commits=rows, second_message_unchanged=True,
              first_message_equals_requested=True, clean=True)
(root / 'VERIFY_RESULT_R2.json').write_text(json.dumps(result, indent=2) + '\n')
(root / 'COMMITS_R2.diff').write_text(git('log', '-2', '-p', '--format=fuller'))
print(json.dumps(result, indent=2))
