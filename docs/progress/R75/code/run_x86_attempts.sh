#!/usr/bin/env bash
set -u
set -o pipefail

workspace=/home/toolchain/development/libc++_replacement
source_dir="${workspace}/progress/R75/code"
out="${workspace}/progress/R75/raw/x86_attempts"
bin="${workspace}/progress/R75/bin/x86_64"
gnu_cxx=/usr/bin/g++
libcxx_cxx="${workspace}/progress/R33/tools/tizen-clang++"
libcxx_include="${workspace}/tmp/R69/build-x86_64/include/c++/v1"
libcxx_lib="${workspace}/tmp/R69/build-x86_64/lib"
mkdir -p "${out}" "${bin}"

attempts=(attempt1_no_delivery_point attempt2_ungated_allocator shrink_to_fit_reproducer)
printf 'attempt\tstdlib\tcompile_rc\trun_rc\n' > "${out}/matrix.tsv"

for attempt in "${attempts[@]}"; do
  source="${source_dir}/${attempt}.cpp"
  for stdlib in libstdcxx libcxx; do
    binary="${bin}/${attempt}.${stdlib}"
    compile_log="${out}/${attempt}.${stdlib}.compile.log"
    if [[ "${stdlib}" == libstdcxx ]]; then
      compile=("${gnu_cxx}" -std=c++17 -O0 -g -fexceptions -pthread "${source}" -o "${binary}")
    else
      compile=("${libcxx_cxx}" -std=c++17 -O0 -g -fexceptions -pthread -stdlib=libc++ -nostdinc++ -I "${libcxx_include}" "${source}" -L "${libcxx_lib}" -Wl,-rpath,"${libcxx_lib}" -Wl,-z,now -lc++ -lc++abi -o "${binary}")
    fi
    {
      printf 'COMMAND:'
      printf ' %q' "${compile[@]}"
      printf '\n'
    } > "${compile_log}"
    set +e
    nice -n 15 ionice -c 3 "${compile[@]}" >> "${compile_log}" 2>&1
    compile_rc=$?
    set -e
    printf 'EXIT_CODE: %s\n' "${compile_rc}" >> "${compile_log}"

    run_rc=125
    if [[ "${compile_rc}" -eq 0 ]]; then
      stdout="${out}/${attempt}.${stdlib}.stdout"
      stderr="${out}/${attempt}.${stdlib}.stderr"
      run_log="${out}/${attempt}.${stdlib}.run.log"
      if [[ "${stdlib}" == libstdcxx ]]; then
        run=(timeout --signal=TERM --kill-after=2s 5s "${binary}")
      else
        run=(timeout --signal=TERM --kill-after=2s 5s env "LD_LIBRARY_PATH=${libcxx_lib}" "${binary}")
      fi
      {
        printf 'COMMAND:'
        printf ' %q' "${run[@]}"
        printf '\n'
      } > "${run_log}"
      set +e
      nice -n 15 ionice -c 3 "${run[@]}" > "${stdout}" 2> "${stderr}"
      run_rc=$?
      set -e
      printf 'EXIT_CODE: %s\n' "${run_rc}" >> "${run_log}"
    fi
    printf '%s\t%s\t%s\t%s\n' "${attempt}" "${stdlib}" "${compile_rc}" "${run_rc}" >> "${out}/matrix.tsv"
  done
done

cat "${out}/matrix.tsv"
