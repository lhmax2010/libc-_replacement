#!/bin/bash
set -euo pipefail

root=${1:-.}

rows=(
  'abseil-cpp|7a4f04dad6d30ba09c406837cdd23ad779a69210|packaging/abseil-cpp.spec'
  'boost|4168e873584ae50a12733262026c007dce18ec89|packaging/boost.spec'
  'icu|dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330|packaging/icu.spec'
  'jsoncpp|9cbab147f44097d5b8da2b3ef38f75376a282d01|packaging/jsoncpp.spec'
  'libsigc++|4f2f592d2f88eb6a3a949cec8970a4cd5d40165b|packaging/libsigc++.spec'
  'pcre|09a5bfc9a96c4235628ce800db3b95bf28ef2f23|packaging/pcre.spec'
  'taglib|8b552bdf27c913a93bca664c41c3fafb214962a1|packaging/taglib.spec'
  'tensorflow2|6d76139029ca4162249667f0db65a73387a94cfe|packaging/tensorflow2.spec'
)

printf 'package\tcommit\tspec\tdefinition_guard\tlibcxx_payload_guard\tlibcxx_token_lines\n'
for row in "${rows[@]}"; do
  IFS='|' read -r package commit spec <<<"$row"
  repo="$root/codes/R104/$package"
  content=$(git -C "$repo" show "$commit:$spec")

  definition_guard=FAIL
  if printf '%s\n' "$content" | awk '
    /%if %\{defined _toolchain\}/ {seen_toolchain=NR}
    /%if %\{toolchain_is clang\}/ && seen_toolchain && NR-seen_toolchain<=2 {seen_clang=NR}
    /%define build_with_libcxx 1/ && seen_clang && NR-seen_clang<=2 {ok=1}
    END {exit(ok ? 0 : 1)}'; then
    definition_guard=PASS
  fi

  token_lines=$(printf '%s\n' "$content" | awk '
    /-stdlib=libc\+\+|-lc\+\+abi|BuildRequires:.*libc\+\+(-devel|abi-devel)|Requires:.*libc\+\+-devel/ {
      if (lines != "") lines=lines ",";
      lines=lines NR
    }
    END {print lines}')

  payload_guard=PASS
  if ! printf '%s\n' "$content" | awk '
    BEGIN {depth=0; guarded=0; bad=0}
    /^%if / {
      depth++
      cond[depth]=$0
      if ($0 ~ /0%\{\?build_with_libcxx\}/) guarded++
      next
    }
    /^%else/ {next}
    /^%endif/ {
      if (cond[depth] ~ /0%\{\?build_with_libcxx\}/) guarded--
      delete cond[depth]
      depth--
      next
    }
    /-stdlib=libc\+\+|-lc\+\+abi|BuildRequires:.*libc\+\+(-devel|abi-devel)|Requires:.*libc\+\+-devel/ {
      if (guarded < 1) bad=1
    }
    END {exit(bad ? 1 : 0)}'; then
    payload_guard=FAIL
  fi

  printf '%s\t%s\t%s\t%s\t%s\t%s\n' \
    "$package" "$commit" "$spec" "$definition_guard" "$payload_guard" "$token_lines"
done
