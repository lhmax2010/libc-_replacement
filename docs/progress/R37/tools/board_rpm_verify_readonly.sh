#!/bin/sh
set +e
echo RPM_VERIFY_BEGIN
rpm -Va 2>&1
rc=$?
echo "RPM_VA_RC=$rc"
echo RPM_VERIFY_END
exit 0
