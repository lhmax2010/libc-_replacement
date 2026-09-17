"""No disk logging before deadline signal; only the already verified owned group."""
import os
import signal
import time
PID=659012
START='20791051'
DEADLINE=1789567765
def alive():
    try:
        with open(f'/proc/{PID}/stat') as f:
            fields=f.read().rsplit(')',1)[1].split()
        return fields[19]==START and os.getpgid(PID)==PID
    except FileNotFoundError:
        return False
while alive() and time.time()<DEADLINE-10:
    time.sleep(1)
if alive():
    os.killpg(PID,signal.SIGTERM)
    os.killpg(PID,signal.SIGCONT)
    sent=time.time()
    while alive() and time.time()<DEADLINE:
        time.sleep(0.2)
    if alive():
        os.killpg(PID,signal.SIGKILL)
    # Emit after signals so delayed journal/filesystem writes cannot defer them.
    print(f'owned_pgid={PID} terminate_at={sent} deadline={DEADLINE}',flush=True)
else:
    print(f'owned_pgid={PID} already ended before deadline; no signal sent',flush=True)
