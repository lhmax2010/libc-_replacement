"""Read-only pre-push verification of the authorized squash."""
import json
import pathlib
import subprocess

out = pathlib.Path(__file__).resolve().parent
commands = []
def git(*args):
    argv = ['git', '-C', 'tmp/PRJCONF_REVIEW_1012/repo', *args]
    p = subprocess.run(argv, capture_output=True, text=True)
    commands.append(dict(argv=argv, exitcode=p.returncode, stdout=p.stdout, stderr=p.stderr))
    (out / 'VERIFY_COMMANDS_R3.json').write_text(json.dumps(commands, indent=2) + '\n')
    assert p.returncode == 0, commands[-1]
    return p.stdout

base = '0bb6ff66506574606cf4d636aaa8974ff4c779b8'
old = 'f385421f72c3daf28ddb60b5b04d051cbd2fdbad'
tree = '04f46636709cd42b7967e3c7bd40cb6098d61cfa'
target = 'TIZEN/Tizen/Tizen-Base-Toolchain/project_config'
assert git('rev-parse', 'origin/master').strip() == base
assert git('rev-parse', 'HEAD^').strip() == base
assert git('rev-list', '--count', 'origin/master..HEAD').strip() == '1'
assert git('branch', '--show-current').strip() == 'prjconf-libcxx'
assert git('status', '--porcelain') == ''
head = git('rev-parse', 'HEAD').strip()
assert git('rev-parse', 'HEAD^{tree}').strip() == tree
assert git('rev-parse', old + '^{tree}').strip() == tree
assert git('diff', '--exit-code', old, 'HEAD') == ''
assert git('diff', '--name-only', 'origin/master..HEAD').splitlines() == [target]
baseline = git('show', base + ':' + target)
assert baseline.count('-fuse-ld=lld ') == 1
expected = baseline.replace('-fuse-ld=lld ', '')
for anchor, addition in [
    ('Support: libstdc++-devel\n', 'Support: libc++-devel libc++abi-devel\n'),
    ('Keep: libstdc++-devel\n', 'Keep: libc++-devel libc++abi-devel\n'),
    ('Keep: cpp gcc libstdc++\n', 'Keep: libc++ libc++abi\n'),
]:
    assert expected.count(anchor) == 1 and addition not in expected
    expected = expected.replace(anchor, anchor + addition)
assert git('show', 'HEAD:' + target) == expected
author = git('show', '-s', '--format=%an <%ae>', 'HEAD').strip()
committer = git('show', '-s', '--format=%cn <%ce>', 'HEAD').strip()
assert author == committer == 'Hao Lin <hao.lin@samsung.com>'
message = git('show', '-s', '--format=%B', 'HEAD').rstrip('\n')
requested = (out / 'SQUASH_MESSAGE_R3.txt').read_text().rstrip('\n')
assert message == requested + '\nSigned-off-by: Hao Lin <hao.lin@samsung.com>'
assert message.count('Change-Id: ') == 1
assert 'Change-Id: I0403eb61b5c412c7ed53dc6f862c92765ead121f' in message.splitlines()
git('diff', '--check', 'origin/master..HEAD')
(out / 'GIT_SHOW_R3.txt').write_text(git('show', 'HEAD'))
(out / 'FINAL_DIFF_R3.patch').write_text(git('diff', 'origin/master..HEAD'))
result = dict(result='PASS', baseline=base, old_head=old, new_head=head,
              tree=tree, tree_unchanged=True, only_expected_config_edits=True,
              author=author, committer=committer, signed_off_by=author,
              change_id='I0403eb61b5c412c7ed53dc6f862c92765ead121f',
              commit_count=1, clean=True)
(out / 'VERIFY_RESULT_R3.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
