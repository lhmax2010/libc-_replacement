#!/usr/bin/env bash
set -u
set -o pipefail

root="${1:?usage: generate_material_index.sh DOCS_R74_ROOT}"
workspace="${2:?usage: generate_material_index.sh DOCS_R74_ROOT WORKSPACE}"
index="${root}/INDEX.tsv"
tmp="${root}/.INDEX.tsv.tmp"

printf 'source\toriginal_path\tupload_path\tsha256\n' > "${tmp}"
while IFS= read -r -d '' file; do
  rel="${file#${root}/}"
  if [[ "${rel}" == "INDEX.tsv" || "${rel}" == ".INDEX.tsv.tmp" ]]; then
    continue
  fi
  source_kind="R74_GENERATED_OR_CAPTURED"
  original="${workspace}/progress/R74/${rel}"
  if [[ ! -e "${original}" ]]; then
    original="NOT_APPLICABLE"
  fi
  digest="$(sha256sum "${file}")"
  digest="${digest%% *}"
  printf '%s\t%s\t%s\t%s\n' "${source_kind}" "${original}" "${file}" "${digest}" >> "${tmp}"
done < <(find "${root}" -type f -print0 | sort -z)
mv "${tmp}" "${index}"
