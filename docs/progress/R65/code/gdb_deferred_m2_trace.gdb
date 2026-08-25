set pagination off
set confirm off
set breakpoint pending on
set print thread-events on
set environment LD_LIBRARY_PATH /var/tmp/r65_20260825/lib
set substitute-path /home/toolchain/development/libc++_replacement/progress/R64/code /var/tmp/r65_20260825/src
set substitute-path /home/toolchain/development/libc++_replacement/tmp/W2/build-armv7l/include/c++/v1 /var/tmp/r65_20260825/include/c++/v1
handle SIG32 nostop noprint pass
set $personality_count = 0
set $stop_count = 0

start m2
delete breakpoints

break pthread_cancel
commands
  silent
  echo === BEFORE_PTHREAD_CANCEL ===\n
  info threads
  thread apply all bt 40
  continue
end

rbreak ^_Unwind_ForcedUnwind$
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
  if $personality_count <= 30
    echo === ENTER_PERSONALITY ===\n
    print $personality_count
    print state
    bt 30
  end
  continue
end

break unwind_stop
commands
  silent
  set $stop_count = $stop_count + 1
  if $stop_count <= 30
    echo === ENTER_GLIBC_UNWIND_STOP ===\n
    print $stop_count
    print actions
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

break tmp/W2/build-armv7l/include/c++/v1/future:919
commands
  silent
  echo === ENTER_ASYNC_FORCED_HANDLER ===\n
  info threads
  thread apply all bt 40
  continue
end

break tmp/W2/build-armv7l/include/c++/v1/future:926
commands
  silent
  echo === BEFORE_COMPLETE_FORCED_UNWIND ===\n
  info threads
  thread apply all bt 40
  continue
end

break /home/toolchain/development/libc++_replacement/progress/R64/code/r64_m2_deferred.cpp:52
commands
  silent
  echo === AFTER_PTHREAD_CANCEL_CALL ===\n
  print cancel_rc
  info threads
  thread apply all bt 40
  continue
end

continue
