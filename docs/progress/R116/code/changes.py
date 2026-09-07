#!/usr/bin/env python3
import json,pathlib,subprocess,sys
r=pathlib.Path('docs/progress/R116');js=json.loads((r/'sources/cros_merged_changes.txt').read_text()[4:])
selected={3310954,3335237,3339453,3343875,3246062,3298526,3292637,4677913,4685180}
for x in js:
 if x['_number'] not in selected:continue
 rev=x['current_revision'];commit=x['revisions'][rev]['commit']
 print('CHANGE',x['_number'],x['status'],x['updated'],rev,'\n'+commit['message'])
 subprocess.run([sys.executable,str(r/'code/fetch.py'),f"cros_diff_{x['_number']}",
 f'https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/{rev}^!?format=TEXT','base64'])
