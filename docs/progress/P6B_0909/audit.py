#!/usr/bin/env python3
"""P6B 的只读核验与生成证据；每个 Git 子命令独立记录。"""
import csv
import hashlib
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path.cwd()
OUT = ROOT / 'docs/progress/P6B_0909'
SRC = ROOT / 'tmp/IMPL_0908/source'
BASE = 'c3f8578a4db871d9d6de96d751f4c2ea7b1638fa'
HEAD = 'c68f376fbeb1bc0cbb93f2569bb1eedb22e90d13'
OLD = 'refs/heads/sandbox/lhmax2025/libcxx-noexcept-relief'
NEW = 'refs/heads/sandbox/lhmax2025/libcxx-ehabi-backport'
EXPECTED = ['16a73b0af4d7f9194ae3a12d4934965d2f4005b7',
            '72e893320c94db78b8661441628dfe9ba1e2cb02',
            '8e8335839aad10eb1ac876c49afe32355ff51cf5',
            '4c5ba1fa3a40340a8687b0f4a76326ca41826d7f', HEAD]
PHASE = sys.argv[1]
SEQ = 0

def cmd(*args):
    global SEQ
    SEQ += 1
    stem = OUT / 'raw' / PHASE / f'{SEQ:03d}'
    subprocess.run([sys.executable, str(OUT / 'record.py'), str(stem), *args], check=True)
    return pathlib.Path(str(stem) + '.stdout').read_bytes()

def git(*args):
    return cmd('git', '-C', str(SRC), *args)

def tsv(name, fields, rows):
    with (OUT / name).open('w', newline='') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(fields)
        w.writerows(rows)

def heads(stem):
    return dict((ref, sha) for sha, ref in (l.split() for l in (OUT / 'raw' / (stem + '.stdout')).read_text().splitlines()))

def pre():
    refs = heads('003_heads_before')
    assert refs[NEW] == BASE and refs[OLD] == HEAD
    tsv('NAMESPACE_BEFORE.tsv', ['ref', 'sha'], sorted((r,s) for r,s in refs.items() if r.startswith('refs/heads/sandbox/lhmax2025/')))
    assert not git('status', '--porcelain').strip()
    assert git('rev-parse', 'HEAD').decode().strip() == HEAD
    git('merge-base', '--is-ancestor', BASE, HEAD)
    assert git('merge-base', BASE, HEAD).decode().strip() == BASE
    commits = git('rev-list', '--reverse', BASE + '..' + HEAD).decode().splitlines()
    assert commits == EXPECTED
    old_history = git('rev-list', BASE).decode().splitlines()
    new_history = set(git('rev-list', HEAD).decode().splitlines())
    assert set(old_history) <= new_history
    assert new_history - set(old_history) == set(EXPECTED)
    tsv('EXISTING_COMMITS_BEFORE.tsv', ['commit', 'retained_in_candidate'], [(s, 'YES') for s in old_history])
    objects = OUT / 'commit_objects'
    objects.mkdir(exist_ok=True)
    mapping = []
    parent = BASE
    ids = []
    for sha in commits:
        obj = git('cat-file', 'commit', sha)
        (objects / (sha + '.commit')).write_bytes(obj)
        tree = git('rev-parse', sha + '^{tree}').decode().strip()
        assert git('rev-parse', sha + '^').decode().strip() == parent
        change_ids = re.findall(rb'^Change-Id: (I[0-9a-f]{40})$', obj, re.M)
        assert len(change_ids) == 1
        ids.append(change_ids[0])
        mapping.append([sha, parent, tree, change_ids[0].decode(), hashlib.sha256(obj).hexdigest()])
        parent = sha
    assert len(set(ids)) == 5
    tsv('FIVE_COMMITS_BEFORE.tsv', ['commit', 'parent', 'tree', 'change_id', 'commit_object_sha256'], mapping)
    paths = git('diff', '--name-only', BASE, HEAD).decode().splitlines()
    rows = []
    for path in paths:
        before = git('ls-tree', BASE, '--', path).decode().strip()
        after = git('ls-tree', HEAD, '--', path).decode().strip()
        rows.append([path, before, after, 'FAST_FORWARD_NO_DIVERGENCE'])
    tsv('FILE_CONFLICT_CHECK.tsv', ['path', 'base_entry', 'candidate_entry', 'result'], rows)
    git('diff', '--check', BASE, HEAD)
    summary = dict(base=BASE, candidate=HEAD, remote_head_count=len(refs), existing_commit_count=len(old_history), added_commits=len(commits), changed_files=len(paths), fast_forward=True)
    (OUT / 'PRECHECK.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary))

def verify():
    before_stem, after_stem = sys.argv[2:4]
    before, after = heads(before_stem), heads(after_stem)
    deleting = PHASE == 'post_delete'
    if deleting:
        assert OLD not in after and before[OLD] == HEAD
        assert {r:s for r,s in before.items() if r != OLD} == after
    else:
        assert before[NEW] == BASE and after[NEW] == HEAD
        assert {r:s for r,s in before.items() if r != NEW} == {r:s for r,s in after.items() if r != NEW}
    assert after[NEW] == HEAD
    rows = [(r, before.get(r, 'ABSENT'), after.get(r, 'ABSENT'), 'UNCHANGED' if before.get(r) == after.get(r) else ('DELETED' if r not in after else 'FAST_FORWARD')) for r in sorted(set(before) | set(after))]
    tsv(PHASE.upper() + '_BRANCH_COMPARISON.tsv', ['ref', 'before', 'after', 'result'], rows)
    local = git('rev-parse', 'refs/heads/' + NEW.removeprefix('refs/heads/')).decode().strip()
    assert local == after[NEW]
    retained = set(git('rev-list', local).decode().splitlines())
    with (OUT / 'EXISTING_COMMITS_BEFORE.tsv').open() as f:
        old = [r['commit'] for r in csv.DictReader(f, delimiter='\t')]
    assert set(old) <= retained and retained - set(old) == set(EXPECTED)
    tsv(PHASE.upper() + '_EXISTING_COMMITS.tsv', ['commit', 'retained'], [(s, 'YES') for s in old])
    with (OUT / 'FIVE_COMMITS_BEFORE.tsv').open() as f:
        five = list(csv.DictReader(f, delimiter='\t'))
    result = []
    for row in five:
        sha = row['commit']
        git('merge-base', '--is-ancestor', sha, local)
        tree = git('rev-parse', sha + '^{tree}').decode().strip()
        assert tree == row['tree']
        # 同一提交对象在新分支祖先链上的精确位置再次解析，避免只比较预设常量。
        position = 4 - EXPECTED.index(sha)
        resolved = git('rev-parse', local + '~' + str(position)).decode().strip()
        assert resolved == sha
        assert git('diff', '--exit-code', row['tree'], resolved + '^{tree}') == b''
        obj = git('cat-file', 'commit', resolved)
        assert hashlib.sha256(obj).hexdigest() == row['commit_object_sha256']
        assert obj == (OUT / 'commit_objects' / (sha + '.commit')).read_bytes()
        result.append([sha, resolved, row['tree'], tree, row['change_id'], 'IDENTICAL', '0'])
    tsv(PHASE.upper() + '_FIVE_COMMITS.tsv', ['before_commit', 'after_commit', 'before_tree', 'after_tree', 'change_id', 'object_bytes', 'diff_exit'], result)
    assert not git('status', '--porcelain').strip()
    print(json.dumps(dict(remote_head=after[NEW], local_head=local, retained_existing=len(old), added=5, other_refs_unchanged=len(after) if deleting else len(after)-1, deleted=deleting)))

if PHASE == 'pre':
    pre()
elif PHASE in ('post_merge', 'post_delete'):
    verify()
else:
    raise SystemExit('unknown phase')
