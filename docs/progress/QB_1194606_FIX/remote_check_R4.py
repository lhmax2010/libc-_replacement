"""Read-only verification of every published R5 row, with complete command receipts."""
import csv, json, pathlib, subprocess, sys
E=pathlib.Path(__file__).resolve().parent
rows=list(csv.DictReader((E/'QUICKBUILD_BRANCHES_R5.tsv').open(),delimiter='\t'))
results=[]
suffix='_adjudicated' if '--adjudicated' in sys.argv else ''
for i,row in enumerate(rows,1):
    label=f'R4_remote{suffix}_{i:02d}'
    argv=['git','ls-remote','ssh://lhmax2025@review.tizen.org:29418/'+row['repo'],'refs/heads/'+row['branch']]
    p=subprocess.run([sys.executable,str(E/'run.py'),label,*argv],capture_output=True,text=True)
    output=(E/'raw'/f'{label}.stdout.txt').read_text()
    got=output.split()[0] if output.split() else None
    results.append(dict(**row,observed=got,exitcode=p.returncode,result='MATCH' if p.returncode==0 and got==row['commit'] else 'MISMATCH'))
    print(row['package'],results[-1]['result'],flush=True)
(E/('REMOTE_CHECK_R4'+suffix+'.json')).write_text(json.dumps(results,indent=2)+'\n')
assert all(r['result']=='MATCH' for r in results)
