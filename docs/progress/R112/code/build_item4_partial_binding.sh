#!/bin/bash
set -euo pipefail
root=$(cd "$(dirname "$0")/../../../.." && pwd)
code="$root/docs/progress/R112/code"
out="$root/docs/progress/R112/build/item4_partial"
mkdir -p "$out/old" "$out/new"
cc=${CC:-cc}
"$cc" -fPIC -shared "$code/item4_partial_old.c" -Wl,-soname,libpartial.so.1 \
  -o "$out/old/libpartial.so.1"
"$cc" -fPIC -shared "$code/item4_partial_new.c" -Wl,-soname,libpartial.so.1 \
  -o "$out/new/libpartial.so.1"
ln -sf libpartial.so.1 "$out/old/libpartial.so"
"$cc" "$code/item4_partial_consumer.c" -L"$out/old" -lpartial \
  -Wl,-z,lazy -o "$out/consumer_lazy_profile"
"$cc" "$code/item4_partial_consumer.c" -L"$out/old" -lpartial \
  -Wl,-z,now -o "$out/consumer_now_control"
for file in "$out/consumer_lazy_profile" "$out/consumer_now_control"; do
  echo "FILE=$file"
  readelf -d "$file" | grep -E "NEEDED|BIND_NOW|FLAGS"
  readelf -W -l "$file" | grep GNU_RELRO
done
sha256sum "$out/consumer_lazy_profile" "$out/consumer_now_control" \
  "$out/old/libpartial.so.1" "$out/new/libpartial.so.1"
