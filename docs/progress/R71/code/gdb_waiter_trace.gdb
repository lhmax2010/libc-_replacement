set pagination off
set confirm off
set breakpoint pending on
set print thread-events on
set width 0

break pthread_cancel
commands
  silent
  printf "R71_EVENT=BEFORE_PTHREAD_CANCEL\n"
  info threads
  thread apply all bt 40
  continue
end

break _Unwind_ForcedUnwind
commands
  silent
  printf "R71_EVENT=FORCED_UNWIND_STARTED\n"
  info threads
  thread apply all bt 40
  continue
end

break __libc_fatal
commands
  silent
  printf "R71_EVENT=LIBC_FATAL\n"
  info threads
  thread apply all bt 40
  continue
end

run waiting-side-cancel

printf "R71_EVENT=INFERIOR_STOPPED_OR_EXITED\n"
info program
thread apply all bt 40
