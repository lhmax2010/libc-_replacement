#!/usr/bin/env bash
set -u
set -o pipefail

out_dir="${1:?usage: fetch_external_links.sh OUT_DIR}"
mkdir -p "${out_dir}"

urls=(
  "https://libcxx.llvm.org/DesignDocs/NoexceptPolicy.html"
  "https://gcc.gnu.org/pipermail/libstdc++/2021-December/053608.html"
  "https://gcc.gnu.org/pipermail/libstdc++/2021-December/053631.html"
  "https://chromium.googlesource.com/chromium/src.git/+/refs/heads/main/base/threading/thread.cc"
  "https://android.googlesource.com/platform/bionic/+/refs/heads/android11-dev/docs/status.md"
)
names=(
  "libcxx_noexcept"
  "gcc_discussion"
  "gcc_commit"
  "chromium_thread"
  "android_bionic_status"
)

printf 'name\turl\thttp_code\texit_code\n' > "${out_dir}/INDEX_RETRY.tsv"

for index in "${!urls[@]}"; do
  name="${names[${index}]}"
  url="${urls[${index}]}"
  command_file="${out_dir}/${name}.retry.command.txt"
  stderr_file="${out_dir}/${name}.retry.stderr"
  headers_file="${out_dir}/${name}.retry.headers"
  body_file="${out_dir}/${name}.retry.body"
  printf '%s\n' "curl --location --silent --show-error --user-agent Mozilla/5.0 --dump-header ${headers_file} --output ${body_file} --write-out %{http_code} ${url}" > "${command_file}"
  set +e
  http_code="$(curl --location --silent --show-error \
    --user-agent 'Mozilla/5.0' \
    --dump-header "${headers_file}" \
    --output "${body_file}" \
    --write-out '%{http_code}' \
    "${url}" 2> "${stderr_file}")"
  rc=$?
  set -e
  printf '%s\t%s\t%s\t%s\n' "${name}" "${url}" "${http_code}" "${rc}" >> "${out_dir}/INDEX_RETRY.tsv"
done
