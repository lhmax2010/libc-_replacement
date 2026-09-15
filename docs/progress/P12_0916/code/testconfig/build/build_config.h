#pragma once
// Standalone x86_64 non-TV forwarding-target test configuration, NOT a platform
// generated GN header and NOT part of the production patch. TV branches excluded.
#define BUILDFLAG(flag) P12_TEST_BUILDFLAG_##flag
#define P12_TEST_BUILDFLAG_IS_TIZEN_TV 0
