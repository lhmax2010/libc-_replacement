# 共用复现命令 / Shared reproduction commands

仅说明，未在本阶段执行。Instructions only; not executed in this stage.
从证据仓根目录运行，替换三个绝对路径。Run from the evidence-repository root; replace all three absolute paths.
使用新头与配套完整库，不替换系统库。Use new headers and matched complete runtimes; do not replace system libraries.

```bash
VALIDATION_CXX=/absolute/path/to/matching/clang++-wrapper
VALIDATION_HEADERS=/absolute/path/to/new/include/c++/v1
VALIDATION_RUNTIME=/absolute/path/to/matched/lib
mkdir -p tmp/self-verify
nice -n 15 ionice -c 3 "$VALIDATION_CXX" -std=c++20 -O0 -g -fno-inline \
  -fexceptions -nostdinc++ -I "$VALIDATION_HEADERS" \
  docs/progress/R111/code/condition_cancel_probe.cpp \
  docs/progress/IMPL_0908/code/runtime_identity.cpp \
  -nostdlib++ -L "$VALIDATION_RUNTIME" -Wl,-rpath-link,"$VALIDATION_RUNTIME" \
  -lc++ -lc++abi -latomic -pthread -ldl -o tmp/self-verify/cv_cancel
readelf -V tmp/self-verify/cv_cancel
readelf --dyn-syms --wide tmp/self-verify/cv_cancel
readelf -V "$VALIDATION_RUNTIME/libc++.so.1"
readelf --dyn-syms --wide "$VALIDATION_RUNTIME/libc++.so.1"
for mode in wait system_clock steady_clock custom_clock; do
  for iteration in 1 2 3 4 5; do
    nice -n 15 ionice -c 3 timeout 35 env \
      LD_LIBRARY_PATH="$VALIDATION_RUNTIME" EXPECTED_RUNTIME="$VALIDATION_RUNTIME" \
      tmp/self-verify/cv_cancel "$mode"
    validation_rc=$?
    printf 'mode=%s repeat=%s exit=%s\n' "$mode" "$iteration" "$validation_rc"
  done
done
```

循环继续输出全部五轮，最后一次退出码不代表此前各轮；必须逐轮核验值。
The loop emits all five iterations; its final status does not summarize previous iterations. Check each run's values.
