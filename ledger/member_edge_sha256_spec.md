# `member_edge_sha256` canonical serialization

## Canonical payload

The digest covers both the member set and the typed edge set. An edge is the
four-field tuple `(src_pkg, dst_pkg, edge_class, stl_kind)`; `edge_class` is
mandatory and therefore distinguishes `CPP_ABI`, `CPP_NOSTL`,
`A8_LAYOUT_PROMOTED`, `A9_POINTEE_PROMOTED`, and any later approved class.
For TIER1 the implementation serializes ordinary CPP_ABI edges as
`D2_CPP_ABI`; for TIER2 it serializes the original edge class.

The canonical byte string is:

```python
json.dumps(
    {
        "members": sorted(members),
        "edges": sorted(
            (src_pkg, dst_pkg, edge_class, stl_kind)
            for src_pkg, dst_pkg, edge_class, stl_kind in edges
        ),
    },
    separators=(",", ":"),
    ensure_ascii=False,
).encode("utf-8")
```

Rules:

1. Package strings and each tuple field are compared by Python's Unicode
   code-point ordering; tuple sorting is lexicographic.
2. JSON object insertion order is exactly `members`, then `edges`.
3. JSON string escaping is the standard `json.dumps` escaping; non-ASCII
   code points remain UTF-8 because `ensure_ascii=False`.
4. There is no whitespace and no trailing newline in the hashed bytes.
5. Duplicate members and duplicate edges are rejected before serialization;
   they are not silently deduplicated.
6. SHA-256 is lowercase hexadecimal over the canonical UTF-8 bytes.

## Cross-class negative control

`member_edge_sha256_cross_class_fixture.tsv` holds two payloads that differ
only in `edge_class`. Their hashes must differ. This is a permanent validation
row and prevents a `CPP_ABI`/`CPP_NOSTL` cross-class collision from being
mistaken for component identity.
