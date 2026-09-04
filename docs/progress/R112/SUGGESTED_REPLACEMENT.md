# Suggested replacement reply

Hi Dongkyun,

While reconstructing the calculation behind “50 distinct sites,” we found
that the number in last week's report was not a valid count of distinct catch
sites. Below are the exact definitions, scope, and corrected counts.

---

**1. Definitions and counting units**

A **recognition clause** is a source-level `catch` (or libstdc++ `__catch`)
clause that catches `abi::__forced_unwind` or
`__cxxabiv1::__forced_unwind`.

A **propagating clause** additionally re-propagates from that handler, either
with a bare

```cpp
throw;
```

or with libstdc++'s

```cpp
__throw_exception_again;
```

We use two different counting units and do not compare them as if they were
the same:

- a **source occurrence** is identified by source RPM instance, normalized
  path inside the package, and source line;
- a **logical site** is obtained after merging duplicate source RPM Release
  variants, using package name, normalized package path, source line, and the
  exact clause/re-propagation form as the key.

---

**2. Scope of this correction**

The original scan covered 371 successfully extracted source RPMs. The strict
audit performed for this correction did not rerun an unrestricted matcher
over all 371 RPMs. It first selected the 11 source RPM instances for which the
original location table already contained an explicit target catch clause,
and then rescanned all text files in those 11 instances without a filename-
extension whitelist. The frozen input records in our archive are identified
by these SHA-256 values:

```text
extraction_results.tsv  5498c430baecd81e689a44d8266d5597a92779680105ede0c320f9b1ea013031
corpus_hits.tsv         cd2a91832f5a548b4e762c9cc42c4349c50d4f82e559058760f1cf6f83a6c7c2
scan_status.tsv         5c15fe0097f5dc6b37a755ce25cb6d01846050fa653a4ca5fe8e74803ed85723
```

Documentation paths were excluded; source, tests, examples, and build files
were retained. This is a source-occurrence count, so conditionally inactive
source is still counted.

For each candidate, we confirmed that the text was an actual catch clause,
located its balanced handler body, and checked for either propagation form.
Main source-archive prefixes were normalized. Distinct paths inside embedded
architecture assets were retained. Duplicate source RPM Release variants
were then merged with the logical-site key above.

These details are necessary to reproduce 104. The number is therefore a
corrected count for the audited historical-positive subset, not a certified
current full-platform total.

---

**3. Correction of the historical result**

The original heuristic selected 50 source-occurrence token rows. Of those:

- 46 rows were actual target catch clauses;
- 4 rows were `key-manager` log strings inside real handlers, between the
  target catch clause and the following bare `throw;`.

The old heuristic did not require the current token line itself to be a catch
clause, which produced those four false rows. It also recognized only bare
`throw;`, so it omitted all 126 source occurrences using
`__throw_exception_again`.

The strict audit of the scoped historical source set produced:

| Measure | Source occurrences | Logical sites after RPM-variant deduplication |
|---|---:|---:|
| Rows selected by the old heuristic | 50 | 45 |
| Actual target catch clauses among those rows | 46 | 41 |
| Propagating clauses after strict audit | 172 | **104** |

The 172 propagating source occurrences consist of 46 bare-`throw;`
occurrences and 126 macro-form occurrences. After deduplication, those become
41 and 63 logical sites respectively.

There are 175 recognition-clause occurrences in total. Three additional
clauses in `dali2-csharp-binder` do not re-propagate, so the propagating count
is 172.

The propagating set spans 10 source RPM instances and 8 package names. The
duplicate instances are different Release variants of GCC and TensorFlow2.
The broader recognition set spans 11 source RPM instances and 9 package
names because it also includes `dali2-csharp-binder`.

The original filename whitelist also omitted extensionless libstdc++ headers
such as `future`, `ostream`, and `condition_variable`. In this audited set,
those account for 32 source occurrences, or 16 logical sites, and are a subset
of the macro-form count above; they are not an additional count to add to 126.

---

**4. Reproduction checklist and possible reasons for different results**

To align the result, please use the same frozen source-RPM input and audited
subset, search all text files rather than only conventional extensions,
confirm actual catch syntax, recognize both propagation forms, exclude
documentation, normalize package-internal paths, and apply the exact logical-
site key stated above.

Numbers can otherwise differ because of:

- a different source snapshot or source-package universe;
- scanning all packages versus this historical-positive subset;
- omission of the `__throw_exception_again` form;
- omission of extensionless C++ headers;
- treatment of strings, comments, documentation, tests, examples, build
  files, or conditionally inactive source;
- normalization of main archives and paths inside embedded architecture
  assets;
- deduplication of source RPM Release variants;
- aliases, generated catch macros, or project-specific indirect propagation
  that are not visible as the two explicit source forms above.

I will attach the 50-row audit table and the 172-row propagating-occurrence
table. The latter contains package, source RPM instance, normalized path,
line, actual code, context, and propagation form; applying the stated
logical-site key to it yields 104 rows.

We have not run the same unrestricted strict scan over the complete current
platform source corpus. Current full-platform additions outside the
historically positive packages are therefore not observed by this audit.

Best regards,
Hao
