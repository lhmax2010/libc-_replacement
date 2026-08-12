#!/usr/bin/env bash
set -u

input=${1:?input TSV required}
output=${2:?output TSV required}

printf 'producer_name\theader\tinstalled_public_header\tconsumer_name\tconsumer_directory\tfile_line\tinclude_text\tcurrent_text\tstatus\n' >"$output"
tail -n +2 "$input" | while IFS=$'\t' read -r producer header installed consumer directory file_line expected; do
    path=${file_line%:*}
    line=${file_line##*:}
    normalized_expected=$expected
    if [[ $normalized_expected == \"*\" ]]; then
        normalized_expected=${normalized_expected:1:${#normalized_expected}-2}
        normalized_expected=${normalized_expected//\"\"/\"}
    fi
    current=$(sed -n "${line}p" "$path" 2>/dev/null || true)
    status=MATCH
    if [[ ! -f "$path" ]]; then
        status=FILE_NOT_FOUND
    elif [[ "$current" != "$normalized_expected" ]]; then
        status=MISMATCH
    fi
    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' \
        "$producer" "$header" "$installed" "$consumer" "$directory" \
        "$file_line" "$normalized_expected" "$current" "$status" >>"$output"
done

awk -F '\t' 'NR > 1 { count[$9]++ } END { for (k in count) print k "\t" count[k] }' "$output" | sort
