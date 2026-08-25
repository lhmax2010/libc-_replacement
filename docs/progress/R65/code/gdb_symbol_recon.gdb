set pagination off
set confirm off
set breakpoint pending on
set environment LD_LIBRARY_PATH /var/tmp/r65_20260825/lib
set substitute-path /home/toolchain/development/libc++_replacement/docs/progress/W2/code/tests /var/tmp/r65_20260825/src
start m2
echo === SHARED_LIBRARIES ===\n
info sharedlibrary
echo === KEY_ADDRESSES ===\n
info address pthread_cancel
info address _Unwind_ForcedUnwind
info address _Unwind_Resume
info address _Unwind_Resume_or_Rethrow
info address __gxx_personality_v0
info address __cxa_rethrow
echo === UNWIND_FUNCTIONS ===\n
info functions Unwind
echo === PTHREAD_UNWIND_FUNCTIONS ===\n
info functions pthread.*unwind
kill
quit
