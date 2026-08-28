set pagination off
set confirm off
set breakpoint pending on
handle SIGABRT stop print nopass
handle SIGSEGV stop print nopass
break std::terminate()
break __gnu_cxx::__verbose_terminate_handler()
break abort
run
printf "R75_STAGE_1\n"
thread apply all bt
continue
printf "R75_STAGE_2\n"
thread apply all bt
continue
printf "R75_STAGE_3\n"
thread apply all bt
info registers
x/16i $pc-32
quit
