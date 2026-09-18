# Test harness only: establish the absent-toolchain configuration at spec parse
# time, including rpmbuild's target-specific reparse. The candidate is unchanged.
%{echo:HARNESS_BEFORE_UNDEFINE=%{defined _toolchain}|%{?_toolchain}}
%undefine _toolchain
%if %{defined _toolchain}
%{error:Test harness failed to remove _toolchain}
%endif
%{echo:HARNESS_AFTER_UNDEFINE=%{defined _toolchain}|%{?_toolchain}}
%include /tmp/task/specs/bpftrace.spec
%if %{defined _toolchain}
%{error:_toolchain was reintroduced while parsing the candidate}
%endif
%{echo:HARNESS_AFTER_INCLUDE=%{defined _toolchain}|%{?build_with_libcxx}|%{__cc}|%{__cxx}}
