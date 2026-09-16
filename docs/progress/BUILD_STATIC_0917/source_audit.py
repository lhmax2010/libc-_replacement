from probes import p, run
for label,root in [('bpftrace','tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'),('bcc','codes/R104/bcc-tools'),('llvm','tmp/BUILD_W1BC_0911/target-fetch')]:
    for what,args in [('identity',['rev-parse','HEAD']),('status',['status','--short'])]:
        if not (p/(label+'-source-'+what+'.json')).exists(): run(label+'-source-'+what,['git','-C',root]+args)
    run(label+'-cancel-search',['rg','-n','--glob','!.git/**','--glob','!*.patch','pthread_cancel|pthread_setcanceltype',root],180)
