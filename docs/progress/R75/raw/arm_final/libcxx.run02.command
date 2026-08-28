COMMAND: timeout --signal=TERM --kill-after=2s 15s /home/toolchain/.local/bin/sdb shell set\ +e\;\ LD_LIBRARY_PATH=\'/opt/usr/home/owner/share/tmp/r75_20260828/lib\'\ \'/opt/usr/home/owner/share/tmp/r75_20260828/bin/shrink_to_fit_reproducer.libcxx\'\;\ rc=\$\?\;\ printf\ \'\\n__R75_REMOTE_RC__=%s\\n\'\ \"\$rc\"\;\ exit\ 0
TRANSPORT_EXIT_CODE: 0
REMOTE_EXIT_CODE: 134
