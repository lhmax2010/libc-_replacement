set pagination off
set confirm off
set print thread-events on
set breakpoint pending on
handle SIGABRT stop print nopass
handle SIGSEGV stop print nopass
handle SIGILL stop print nopass
handle SIGBUS stop print nopass
run
info program
info threads
thread apply all bt full
quit
