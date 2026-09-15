#!/usr/bin/env python3
"""离线自检、派生归档及 SHA256 封存；不执行实验、不修改既有材料。"""
import collections
import datetime
import hashlib
import json
import pathlib
import re
import shutil
import subprocess
import sys

HERE = pathlib.Path(__file__).resolve().parent
OUT = HERE.parent
ROOT = OUT.parents[2]
E = OUT / 'experiment1'
DOCS = ['adaptor_proposal_for_review.md', 'adaptor_proposal_for_review_en.md']


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(4 * 1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def dump(path, value):
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')


def load(name):
    return json.loads((E / name).read_text())


def files():
    roots = [E, OUT / 'experiment3', HERE]
    paths = [p for d in roots for p in d.rglob('*') if p.is_file() and '__pycache__' not in p.parts]
    paths += [OUT / n for n in DOCS + ['REPORT.md', 'STATUS.md', 'QUESTIONS.md', 'REVIEW_DISPOSITION.md']]
    return sorted(set(paths))


def audit():
    assert subprocess.check_output(['git', 'branch', '--show-current'], cwd=ROOT).decode().strip() == 'codex/runtime-validation'
    scripts = HERE / 'snapshots'
    scripts.mkdir(exist_ok=True)
    # 可复核的新旧稿差异；原材料以只读方式取自原提交。
    base = 'b73baed1f22d06613cc798f1293c13fdf3c2f0b5'
    for name in DOCS:
        original = subprocess.check_output(['git', 'show', base + ':' + name], cwd=ROOT)
        (scripts / ('before_' + name)).write_bytes(original)
        text = (ROOT / name).read_text()
        (OUT / name).write_text(text.replace('](docs/', '](../../../docs/'))
        assert (OUT / name).read_text().replace('](../../../docs/', '](docs/') == text
    english = (ROOT / DOCS[1]).read_text()
    assert not re.search(r'[\u3400-\u9fff]', english), '英文正文存在中文残留'
    zh = (ROOT / DOCS[0]).read_text()
    for token in ['0x434c4e47432b2b00', '0x474e5543432b2b00', '480', '240', '120', '80', '15', '25',
                  '5/5 SIGABRT', 'old:ping', 'LIBCXX_CXX_ABI=libsupc++', '5.1', '5.2', '5.3', '5.4']:
        assert token in zh and token in english, token
    # 新增节的外部证据链接必须中英一致；正文不使用内部任务代号。
    for text in [zh, english]:
        section = text[text.index('### 5.1'):text.index('## 六、') if '## 六、' in text else text.index('## 6.')]
        body = re.sub(r'\]\([^)]*\)', ']', section)
        assert not re.search(r'\b(?:R14b|R78|P11|P10)\b', body)
    a = set(re.findall(r'\]\((docs/progress/P11_0916/[^)]*)\)', zh))
    b = set(re.findall(r'\]\((docs/progress/P11_0916/[^)]*)\)', english))
    assert a == b, (a-b, b-a)

    observations = load('R78_OBSERVATIONS.json')
    assert len(observations) == 10
    for r in observations:
        assert r['inferior_exit_12']
        rows = r['trace']
        assert all('libc++abi' in x['object'] for x in rows if x['event'] in ('__cxa_throw', '__gxx_personality_v0'))
        assert all(x['exception_class'] == '0x434c4e47432b2b00' for x in rows if 'exception_class' in x)
        matches = [x for x in rows if 'can_catch' in x['event']]
        assert len(matches) == 1
        m = matches[0]
        assert m['catch_type']['name'] == m['thrown_type']['name'] == '13BoundaryError'
        for key in ['address', 'name_address']:
            assert m['catch_type'][key] != m['thrown_type'][key]
        if r['architecture'] == 'armv7l':
            assert any('"value": "false"' in t for t in r['type_match_returns'])

    repeats = load('R14B_REPEATS.json')
    assert len(repeats) == 480
    groups = collections.defaultdict(list)
    for r in repeats:
        groups[r['case']].append(r['repeat'])
        if r['exit_code'] == 0:
            assert 'type=PASS\tcleanup_count=PASS\torder=PASS' in r['stdout']
        elif r['exit_code'] == 1:
            assert 'cleanup_count=PASS' in r['stdout'] and 'type=FAIL' in r['stdout']
        else:
            assert r['exit_code'] in [-6, -11]
    assert len(groups) == 96 and all(sorted(v) == [1, 2, 3, 4, 5] for v in groups.values())
    assert collections.Counter(r['exit_code'] for r in repeats) == {0: 320, 1: 120, -6: 15, -11: 25}
    assert sum(r['exit_code'] != r['historical_return'] for r in repeats) == 5
    assert len(load('R14B_TRACES.json')) == 40
    config_groups = []
    for arch, name in [('x86_64 native', 'ABI_CONFIG_RESULTS.json'), ('armv7l QEMU user mode', 'ARM_ABI_CONFIG_RESULTS.json')]:
        data = load(name)
        assert len(data) == 40
        cells = collections.defaultdict(list)
        for r in data:
            cells[(r['abi'], r['unwinder_setting'], r['scenario'])].append(r['repeat'])
            if r['scenario'] == 'normal':
                assert r['exit'] == 0 and 'LENGTH=8 OUTPUT=old:ping OLD_GUARD=1 ASSERT=PASS' in r['stdout']
            elif r['abi'] == 'libcxxabi':
                assert r['exit'] == 12 and 'WRONG_EXCEPTION_TYPE' in r['stderr']
            elif arch.startswith('arm') and r['unwinder_setting'] == 'LLVM-preload':
                assert r['exit'] == -6 and 'personality function returned unknown result 5' in r['stderr']
            else:
                assert r['exit'] == 0 and 'CAUGHT=77 OLD_GUARD=1 ASSERT=PASS' in r['stdout']
        assert len(cells) == 8 and all(sorted(v) == [1, 2, 3, 4, 5] for v in cells.values())
        config_groups.append({'architecture': arch, 'cells': len(cells), 'program_runs': len(data)})
    corrected = load('ARM_CORRECTED_TRACES.json')
    assert len(corrected) == 10
    for r in corrected:
        p = [x for x in r['trace'] if x['event'] == '__gxx_personality_v0']
        assert p
        expected = '474e5543432b2b00' if r['abi'] == 'libsupc++' else '002b2b43474e4c43'
        assert all(x['header_bytes'] == expected for x in p)
        assert all('class_read' in x for x in r['trace'] if x['event'] == '_Unwind_RaiseException')

    identities = []
    for name in ['IDENTITY.json', 'ARM_IDENTITY.json']:
        for entry in load(name):
            p = pathlib.Path(entry['source'])
            assert sha(p) == entry['sha256'], str(p)
            if 'snapshot' in entry:
                assert sha(E / entry['snapshot']) == entry['sha256']
            identities.append(entry['source'])
    old_review = ROOT / 'docs/progress/P10_0915/REVIEW_DISPOSITION.md'
    assert sha(old_review) == 'e6bbc60ed8425c37d44f0fd95c60237a664a71a67f51c6e71f789eab29f2492f'
    assert not subprocess.check_output(['git', 'diff', '--name-only', '--', 'docs/progress/P11_0916/experiment2'], cwd=ROOT)
    preserved = {
        'docs/progress/R115/W3/raw/033_angle_files.stderr': '94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c',
        'docs/progress/R115/W3/raw/033_angle_files.stdout': 'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c',
    }
    for name, digest in preserved.items():
        assert sha(ROOT / name) == digest

    # 仅为本轮新文档检查链接，不改历史快照中的原相对引用。
    check_docs = [ROOT / n for n in DOCS] + [OUT / n for n in DOCS]
    check_docs += [OUT / n for n in ['REPORT.md', 'STATUS.md', 'QUESTIONS.md', 'REVIEW_DISPOSITION.md']]
    check_docs += [E / 'REPORT.md', OUT / 'experiment3/REPORT.md', HERE / 'SELF_CHECK.md']
    links = []
    for p in check_docs:
        for target in re.findall(r'\]\(([^)]+)\)', p.read_text()):
            if target.startswith(('https://', 'http://', '#')):
                continue
            target = target.split('#')[0]
            resolved = (p.parent / target).resolve()
            if resolved in [OUT / 'SHA256SUMS', HERE / 'CHECKS.json']:
                continue  # 本次核对/封存阶段生成。
            assert resolved.exists(), (str(p), target)
            links.append({'file': str(p.relative_to(ROOT)), 'target': target})
    for d in [E, OUT / 'experiment3']:
        for p in d.glob('*'):
            if p.is_file() and p.suffix in {'.py', '.sh', '.gdb', '.c'}:
                dest = d / 'snapshots' / ('final_' + p.name)
                dest.parent.mkdir(exist_ok=True)
                shutil.copyfile(p, dest)
                assert sha(p) == sha(dest)
    shutil.copyfile(__file__, scripts / 'audit.py')
    for p in files():
        with p.open('rb') as f:
            assert f.read(4) != b'\x7fELF', str(p)
    result = {'completed_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'result': 'PASS', 'r78_observer_runs': 10, 'r14b_cells': 96, 'r14b_program_runs': 480,
              'r14b_trace_runs': 40, 'abi_configurations': config_groups, 'arm_corrected_trace_runs': 10,
              'identity_entries_verified': len(identities), 'local_links_checked': len(links),
              'english_CJK': 0, 'matching_new_evidence_links': len(a), 'preserved_user_files': preserved,
              'bilingual_review': '新增事实、数值、架构、推论标签人工对照；自动核对核心数值与证据链接，非自动语义证明',
              'old_experiment2_tracked_files': 'UNCHANGED', 'platform_changes': 'NONE',
              'limits': ['ARM LLVM 初始 RaiseException 头读取无效，改用 personality 可靠字节',
                         '真实反向 EWK 未运行', '源码/脚本快照与运行产物身份均核对；未宣称全平台覆盖']}
    dump(HERE / 'CHECKS.json', result)
    print(json.dumps(result, ensure_ascii=False, indent=2))


def seal():
    paths = files()
    # seal 自身命令仍在执行，单独作为后续交付记录保存，不纳入自引用摘要。
    paths = [p for p in paths if '/raw/007_seal.' not in str(p)]
    commands = [p for p in paths if p.name.endswith('.command.txt')]
    for p in commands:
        stem = str(p)[:-len('.command.txt')]
        for suffix in ['.stdout', '.stderr', '.exitcode', '.time.json']:
            assert pathlib.Path(stem + suffix).is_file(), stem + suffix
        int(pathlib.Path(stem + '.exitcode').read_text())
    (OUT / 'SHA256SUMS').write_text(''.join(sha(p) + '  ' + str(p.relative_to(ROOT)) + '\n' for p in paths))
    print('SEALED_FILES', len(paths), 'COMPLETE_COMMAND_RECORDS', len(commands))


if __name__ == '__main__':
    {'audit': audit, 'seal': seal}[sys.argv[1]]()
