Name:       libcynara-commons
Summary:    Cynara - cynara commons library
Version:    0.26.0
Release:    1
Group:      Security/Application Privilege
License:    Apache-2.0 or MIT
Source0:    cynara-%{version}.tar.gz
Source1000:    %{name}-rpmlintrc
Source1002:    libcynara-client.manifest
Source1003:    libcynara-admin.manifest
Source1004:    cynara-commons-tests.manifest
Source1005:    libcynara-agent.manifest
Source1006:    libcynara-commons.manifest
Source1007:    libcynara-creds-commons.manifest
Source1010:    libcynara-creds-socket.manifest
Source1011:    libcynara-session.manifest
Source1012:    cynara-db-migration.manifest
Source1013:    cyad.manifest
Source1014:    cynara-db-chsgen.manifest
Source1015:    libcynara-monitor.manifest
Source1016:    libcynara-creds-self.manifest
Source1017:    libcynara-creds-pid.manifest
Source1018:    libcynara-uid.manifest
Source1019:    libcynara-uid-creds.manifest
Requires:      default-ac-domains
Requires(pre): cynara-db-migration >= %{version}
%if "%{?dev_wos}" != "1"
Requires(post):   smack
BuildRequires: pkgconfig(libsmack)
%endif
Requires(postun): cynara-db-migration >= %{version}
BuildRequires: cmake
BuildRequires: zip
Obsoletes:     libcynara-storage

# alternatives: DLOG, JOURNAL, NONE, (otherwise SYSLOG is used)
# WARNING!
# when choosing JOURNAL, make sure not to introduce cyclic dependency
# check if systemd depends on libdbus-policy!
%if !%{defined log_type}
%define log_type "DLOG"
%endif


%if %{log_type} == "DLOG"
BuildRequires: pkgconfig(dlog)
%endif

%if %{log_type} == "JOURNAL"
BuildRequires: pkgconfig(libsystemd)
%endif

%if !%{defined build_type}
%define build_type RELEASE
%endif

%if %{?build_type} == "DEBUG"

BuildRequires: libdw-devel
BuildRequires: pkgconfig(libunwind)

%endif

%if "%{build_type}" == "COVERAGE"
BuildRequires: lcov
%global coverage_dir %{_datadir}/cynara-commons-coverage
%endif

%global project_name cynara

%package -n cynara-devel
Summary:    Cynara development files
Requires:   libcynara-admin = %{version}
Requires:   libcynara-agent = %{version}
Requires:   libcynara-client = %{version}
Requires:   libcynara-commons = %{version}
Requires:   libcynara-creds-commons = %{version}
Requires:   libcynara-creds-pid = %{version}
Requires:   libcynara-creds-self = %{version}
Requires:   libcynara-creds-socket = %{version}
Requires:   libcynara-session = %{version}
Requires:   libcynara-uid = %{version}
Requires:   libcynara-uid-creds = %{version}
Requires:   libcynara-monitor = %{version}
Obsoletes:  libcynara-admin-devel
Obsoletes:  libcynara-agent-devel
Obsoletes:  libcynara-client-async-devel
Obsoletes:  libcynara-client-commons-devel
Obsoletes:  libcynara-client-devel
Obsoletes:  libcynara-commons-devel
Obsoletes:  libcynara-creds-commons-devel
Obsoletes:  libcynara-creds-socket-devel
Obsoletes:  libcynara-plugin-devel
Obsoletes:  libcynara-session-devel
Obsoletes:  libcynara-storage-devel

%description -n cynara-devel
Cynara development files

%package -n cynara-commons-tests
Summary:    Cynara - cynara-commons test binaries
BuildRequires: pkgconfig(gmock)

%description -n cynara-commons-tests
Cynara-commons tests

%package -n libcynara-client
Summary:    Cynara - client libraries
BuildRequires: pkgconfig(capi-system-resource)
Obsoletes:  libcynara-client-commons
Obsoletes:  libcynara-client-async

%description -n libcynara-client
Client libraries for checking policies: synchronous and asynchronous

%package -n libcynara-admin
Summary:    Cynara - admin client library
BuildRequires: pkgconfig(capi-system-resource)

%description -n libcynara-admin
admin client library for setting, listing and removing policies

%package -n libcynara-agent
Summary:    Cynara - agent client library

%description -n libcynara-agent
agent client library for communication with cynara service and plugins

%description -n libcynara-commons
cynara common library with common functionalities

%package -n libcynara-creds-commons
Summary:    Base library for cynara credentials helpers
Requires:   libcynara-commons = %{version}

%description -n libcynara-creds-commons
Base library for cynara credentials helpers

%package -n libcynara-creds-pid
Summary:    Cynara credentials helpers library for PID
Requires:   libcynara-creds-commons = %{version}

%description -n libcynara-creds-pid
Cynara credentials helpers library for PID

%package -n libcynara-creds-self
Summary:    Cynara credentials helpers library for current process
Requires:   libcynara-creds-commons = %{version}

%description -n libcynara-creds-self
Cynara credentials helpers library for current context process

%package -n libcynara-creds-socket
Summary:    Cynara credentials helpers library for socket clients
Requires:   libcynara-creds-commons = %{version}

%description -n libcynara-creds-socket
Cynara credentials helpers library for socket clients

%package -n libcynara-session
Summary:    Cynara helper client session string creation library
Requires:   libcynara-commons = %{version}

%description -n libcynara-session
Cynara helper client session string creation library

%package -n libcynara-uid
Summary:    Cynara helpers for puid extraction and generation in UID sandboxing
Requires:   libcynara-commons = %{version}

%description -n libcynara-uid
Cynara helpers for puid extraction and generation in UID sandboxing

%package -n libcynara-uid-creds
Summary:    Cynara helpers library for encoding/decoding credentials in UID sandboxing
Requires:   libcynara-uid = %{version}
Requires:   libcynara-creds-commons = %{version}

%description -n libcynara-uid-creds
Cynara helpers library for encoding/decoding credentials in UID sandboxing

%package -n cynara-db-migration
Summary:    Migration tools for Cynara's database
Requires:   findutils

%description -n cynara-db-migration
Migration tools for Cynara's database

%package -n cyad
Summary: Cynara's command-line tool
Requires:   libcynara-admin = %{version}
Requires:   libcynara-commons = %{version}

%description -n cyad
Command-line tool to manage Cynara's database

%package -n libcynara-monitor
Summary:    Cynara - monitor client library

%description -n libcynara-monitor
Monitor client library for obtaining check logs

%prep
%setup -q -n cynara-%{version}
cp -a %{SOURCE1002} .
cp -a %{SOURCE1003} .
cp -a %{SOURCE1004} .
cp -a %{SOURCE1005} .
cp -a %{SOURCE1006} .
cp -a %{SOURCE1007} .
cp -a %{SOURCE1010} .
cp -a %{SOURCE1011} .
cp -a %{SOURCE1012} .
cp -a %{SOURCE1013} .
cp -a %{SOURCE1014} .
cp -a %{SOURCE1015} .
cp -a %{SOURCE1016} .
cp -a %{SOURCE1017} .
cp -a %{SOURCE1018} .
cp -a %{SOURCE1019} .

%build
export CXXFLAGS+=" -Wl,-z,relro,-z,now"

%if %{?build_type} == "DEBUG" || %{?build_type} == "COVERAGE"
export CXXFLAGS+=" -Wp,-U_FORTIFY_SOURCE"
%endif

export LDFLAGS+=" -Wl,--rpath=%{_libdir}"

%cmake . \
        -DBUILD_TESTS=ON \
        -DBUILD_WITH_SYSTEMD_DAEMON=OFF \
        -DLOG_TYPE=%{log_type} \
        -DCMAKE_BUILD_TYPE=%{?build_type} \
        -DCMAKE_VERBOSE_MAKEFILE=ON \
        -DLIB_DIR:PATH=%{_libdir} \
        -DBIN_DIR:PATH=%{_bindir} \
        -DSBIN_DIR:PATH=%{_sbindir} \
        -DSYS_CONFIG_DIR:PATH=%{_sysconfdir} \
        -DINCLUDE_DIR:PATH=%{_includedir} \
        -DLOCAL_STATE_DIR:PATH=%{_localstatedir} \
        -DDATA_ROOT_DIR:PATH=%{_datadir} \
        -DSYSTEMD_UNIT_DIR:PATH=%{_unitdir} \
        -DSOCKET_DIR:PATH=/run/%{project_name} \
%if "%{?dev_wos}" != "1"
        -DCYNARA_CLIENT_METHOD_DEFAULT=smack \
        -DCYNARA_USER_METHOD_DEFAULT=uid \
        -DDB_FILES_SMACK_LABEL="System" \
        -DSMACK_ENABLED="ON" \
%endif
%if "%{?dev_wos}" == "1" || "%{?uid_app_sandboxing}" == "1"
        -DCYNARA_CLIENT_METHOD_DEFAULT=context \
        -DCYNARA_USER_METHOD_DEFAULT=owner \
        -DUID_SANDBOXING_ENABLED="ON" \
%endif
%if "%{build_type}" == "COVERAGE"
        -DCOVERAGE_DIR=%{coverage_dir} \
%endif
        -DMONITORING=ON
make %{?jobs:-j%jobs}

%install
#rm -rf %{buildroot}
%make_install

%post -n libcynara-client -p /sbin/ldconfig

%postun -n libcynara-client -p /sbin/ldconfig

%post -n libcynara-admin -p /sbin/ldconfig

%postun -n libcynara-admin -p /sbin/ldconfig

%post -n libcynara-agent -p /sbin/ldconfig

%postun -n libcynara-agent -p /sbin/ldconfig

%pre -n libcynara-commons

if [ $1 -gt 1 ] ; then
    # upgrade
    %{_sbindir}/cynara-db-migration upgrade -f 0.0.0 -t %{version}
else
    # install
    %{_sbindir}/cynara-db-migration install -t %{version}
fi

%post -n libcynara-commons -p /sbin/ldconfig

%postun -n libcynara-commons

/sbin/ldconfig
if [ $1 = 0 ]; then
    %{_sbindir}/cynara-db-migration uninstall -f %{version}
    systemctl daemon-reload
fi

%post -n libcynara-creds-commons -p /sbin/ldconfig

%postun -n libcynara-creds-commons -p /sbin/ldconfig

%post -n libcynara-creds-pid -p /sbin/ldconfig

%postun -n libcynara-creds-pid -p /sbin/ldconfig

%post -n libcynara-creds-self -p /sbin/ldconfig

%postun -n libcynara-creds-self -p /sbin/ldconfig

%post -n libcynara-creds-socket -p /sbin/ldconfig

%postun -n libcynara-creds-socket -p /sbin/ldconfig

%post -n libcynara-session -p /sbin/ldconfig

%postun -n libcynara-session -p /sbin/ldconfig

%post -n libcynara-uid -p /sbin/ldconfig

%postun -n libcynara-uid -p /sbin/ldconfig

%post -n libcynara-uid-creds -p /sbin/ldconfig

%postun -n libcynara-uid-creds -p /sbin/ldconfig

%post -n libcynara-monitor -p /sbin/ldconfig

%postun -n libcynara-monitor -p /sbin/ldconfig

%files -n cynara-devel
%license LICENSE LICENSE.MIT
%{_includedir}/cynara/*.h
%{_includedir}/cynara/attributes/*.h
%{_includedir}/cynara/config/*.h
%{_includedir}/cynara/containers/*.h
%{_includedir}/cynara/cpu-priority/*.h
%{_includedir}/cynara/error/*.h
%{_includedir}/cynara/exceptions/*.h
%{_includedir}/cynara/log/*.h
%{_includedir}/cynara/lock/*.h
%{_includedir}/cynara/no_smack/*.h
%{_includedir}/cynara/plugin/*.h
%{_includedir}/cynara/protocol/*.h
%{_includedir}/cynara/request/*.h
%{_includedir}/cynara/response/*.h
%{_includedir}/cynara/storage/*.h
%{_includedir}/cynara/types/*.h
%{_includedir}/cynara/utils/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so

%files -n cynara-commons-tests
%manifest cynara-commons-tests.manifest
%attr(755,root,root) %{_bindir}/cynara-commons-tests
%attr(755,root,root) %{_bindir}/cynara-db-migration-tests
%attr(755,root,root) %{_bindir}/cynara-check-cmd
%attr(755,root,root) %{_datarootdir}/%{project_name}/tests

%files -n libcynara-client
%manifest libcynara-client.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-client.so.*
%{_libdir}/libcynara-client-async.so.*
%{_libdir}/libcynara-client-commons.so.*
%dir %attr(755,cynara,cynara) %{_libdir}/%{project_name}/plugin/client

%files -n libcynara-admin
%manifest libcynara-admin.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-admin.so.*

%files -n libcynara-agent
%manifest libcynara-agent.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-agent.so.*

%files -n libcynara-commons
%manifest libcynara-commons.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-commons.so.*
%{_libdir}/libcynara-storage.so.*
%dir %attr(700,cynara,cynara) %{_localstatedir}/cynara

%files -n libcynara-creds-commons
%manifest libcynara-creds-commons.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-creds-commons.so.*
%{_sysconfdir}/%{project_name}/creds.conf

%files -n libcynara-creds-pid
%manifest libcynara-creds-pid.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-creds-pid.so.*

%files -n libcynara-creds-self
%manifest libcynara-creds-self.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-creds-self.so.*

%files -n libcynara-creds-socket
%manifest libcynara-creds-socket.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-creds-socket.so.*

%files -n libcynara-session
%manifest libcynara-session.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-session.so.*

%files -n libcynara-uid
%manifest libcynara-uid.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-uid.so.*

%files -n libcynara-uid-creds
%manifest libcynara-uid-creds.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-uid-creds.so.*

%files -n cynara-db-migration
%manifest cynara-db-migration.manifest
%manifest cynara-db-chsgen.manifest
%license LICENSE LICENSE.MIT
%attr(700,root,root) %{_sbindir}/cynara-db-migration
%attr(700,root,root) %{_sbindir}/cynara-db-chsgen

%files -n cyad
%manifest cyad.manifest
%license LICENSE LICENSE.MIT
%attr(700,root,root) %{_sbindir}/cyad

%files -n libcynara-monitor
%manifest libcynara-monitor.manifest
%license LICENSE LICENSE.MIT
%{_libdir}/libcynara-monitor.so.*

%if "%{build_type}" == "COVERAGE"
%package -n cynara-commons-coverage
Summary:    Cynara code coverage data
Group:      Security/Testing
Requires:   cynara-commons-tests = %{version}
Requires:   libcynara-commons-debugsource = %{version}
Requires:   lcov
Requires:   gcc

%description -n cynara-commons-coverage
Cynara code coverage data

%files -n cynara-commons-coverage
%manifest cynara-commons-tests.manifest
%{_bindir}/cynara-commons-coverage.sh
%coverage_dir

%endif
