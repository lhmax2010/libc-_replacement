#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
llvm=$workspace/codes/llvm
gcc=$workspace/codes/gcc

printf '=== libc++abi personality scan and foreign/native selection ===\n'
nl -ba "$llvm/libcxxabi/src/cxa_personality.cpp" | sed -n '630,875p;1028,1095p'
printf '=== libc++abi begin-catch, current type, and rethrow ===\n'
nl -ba "$llvm/libcxxabi/src/cxa_exception.cpp" | sed -n '430,500p;590,665p'
printf '=== libc++abi public ABI header current state ===\n'
nl -ba "$llvm/libcxxabi/include/cxxabi.h" | sed -n '45,105p'
printf '=== libstdc++ personality mapping ===\n'
nl -ba "$gcc/libstdc++-v3/libsupc++/eh_personality.cc" | sed -n '535,625p'
printf '=== libstdc++ type token ===\n'
nl -ba "$gcc/libstdc++-v3/libsupc++/cxxabi_forced.h" | sed -n '35,65p'
printf '=== libstdc++ rethrow ===\n'
nl -ba "$gcc/libstdc++-v3/libsupc++/eh_throw.cc" | sed -n '95,140p'
printf '=== libstdc++ current-exception-type ===\n'
nl -ba "$gcc/libstdc++-v3/libsupc++/eh_type.cc" | sed -n '25,60p'
printf '=== local llvm history searches ===\n'
git -C "$llvm" grep -n -E '__forced_unwind|pthread_cancel' HEAD -- libcxxabi || true
printf '=== experimental diff stats and content ===\n'
diff -u "$llvm/libcxxabi/include/cxxabi.h" "$workspace/tmp/R33/llvm-exp/libcxxabi/include/cxxabi.h" || true
diff -u "$llvm/libcxxabi/src/cxa_personality.cpp" "$workspace/tmp/R33/llvm-exp/libcxxabi/src/cxa_personality.cpp" || true
printf '=== symbol surface original vs experiment ===\n'
readelf --dyn-syms --wide "$workspace/tmp/R33/build-s1-r3/lib/libc++abi.so.1.0.versioned" | grep -E '__forced_unwind|__gxx_personality_v0|__cxa_rethrow' || true
printf '=== Itanium ABI sections relevant to forced/foreign and current type ===\n'
curl -fsSL https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html | grep -n -C 3 -E '__cxa_current_exception_type|_UA_FORCE_UNWIND|__foreign_exception' || true
printf '=== LLVM GitHub issue search: forced unwind ===\n'
curl -fsSL 'https://api.github.com/search/issues?q=repo%3Allvm%2Fllvm-project%20%22forced%20unwind%22%20in%3Atitle%2Cbody&per_page=100'
printf '\n=== LLVM GitHub issue search: forced_unwind ===\n'
curl -fsSL 'https://api.github.com/search/issues?q=repo%3Allvm%2Fllvm-project%20forced_unwind%20in%3Atitle%2Cbody&per_page=100'
printf '\n=== LLVM GitHub issue search: pthread_cancel libcxxabi ===\n'
curl -fsSL 'https://api.github.com/search/issues?q=repo%3Allvm%2Fllvm-project%20pthread_cancel%20libcxxabi%20in%3Atitle%2Cbody&per_page=100'
