# Reproducer

`forced_unwind_reproducer.cpp` is the reduced, project-neutral form. The exact full matrix source is `s1_rethrow_probe.cpp`; the reduced file was derived after the run and is not presented as a separately executed artifact.

Representative build after applying the proposed libc++abi changes:

```sh
clang++ -O0 -g -pthread -stdlib=libc++ forced_unwind_reproducer.cpp \
  -L/path/to/experimental/lib -Wl,-rpath,/path/to/experimental/lib \
  -lc++ -lc++abi -o forced_unwind_reproducer
```

Run:

```sh
LD_LIBRARY_PATH=/path/to/experimental/lib ./forced_unwind_reproducer
```

Expected output ordering includes `cleanup:body`, `forced-handler`, `cleanup:handler`, and `join=PTHREAD_CANCELED`; exit status is 0. The exact cross commands, target flags, loaders, and raw outputs used in R42 are archived under `docs/progress/R42/code/`.
