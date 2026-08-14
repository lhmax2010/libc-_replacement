#!/bin/sh
set +e
echo PACKAGE_IDENTITY_BEGIN
rpm -qa --qf '%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}\n' 2>&1 | sort | wc -l
echo "RPM_COUNT_RC=$?"
rpm -qa --qf '%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}\n' 2>&1 | sort | sha256sum
echo "RPM_SET_SHA_RC=$?"
echo PACKAGE_IDENTITY_END
