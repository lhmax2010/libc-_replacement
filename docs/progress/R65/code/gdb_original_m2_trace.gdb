set pagination off
set confirm off
set breakpoint pending on
set print thread-events on
set environment LD_LIBRARY_PATH /var/tmp/r65_20260825/lib
set substitute-path /home/toolchain/development/libc++_replacement/progress/W1/src /var/tmp/r65_20260825/src
handle SIG32 nostop noprint pass
set $personality_count = 0

break pthread_cancel
commands
  silent
  echo === BEFORE_PTHREAD_CANCEL ===\n
  info threads
  thread apply all bt 40
  continue
end

break _Unwind_ForcedUnwind
commands
  silent
  echo === ENTER_UNWIND_FORCED_UNWIND ===\n
  info threads
  thread apply all bt 40
  continue
end

break __GI___pthread_unwind
commands
  silent
  echo === ENTER_PTHREAD_UNWIND ===\n
  info threads
  thread apply all bt 40
  continue
end

break __gxx_personality_v0
commands
  silent
  set $personality_count = $personality_count + 1
  if $personality_count <= 20
    echo === ENTER_PERSONALITY ===\n
    print $personality_count
    print state
    bt 30
  end
  continue
end

break __cxa_rethrow
commands
  silent
  echo === ENTER_CXA_RETHROW ===\n
  info threads
  thread apply all bt 40
  continue
end

break /home/toolchain/development/libc++_replacement/progress/W1/src/r61_m2_matrix.cpp:52
commands
  silent
  echo === AFTER_PTHREAD_CANCEL_CALL ===\n
  print cancel_rc
  info threads
  thread apply all bt 40
  continue
end

run m2
