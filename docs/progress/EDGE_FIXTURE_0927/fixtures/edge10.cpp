#include "identity.h"
#include <delta/delta_parser.h>
#include <filesystem>
// This is an explicitly partial, error-path-only probe. It does not claim a
// successful delta-manifest parse without a version-matched valid data sample.
int main(int argc,char**argv){identity(argc,argv);Lifetime c;
 {Owned<delta::DeltaParser> parser(c);const std::filesystem::path input="not-present-中文-delta.xml";assert(!std::filesystem::exists(input));
 bool result=parser->ParseManifest(input);assert(!result);const auto error=parser->GetErrorMessage();assert(!error.empty());
 std::cout<<"VALUES path="<<input.string()<<" parsed="<<result<<" error="<<error<<'\n';}
 lifecycle(c,1);std::cout<<"NOT_AVAILABLE valid delta document and expected parsed data are not available; error path only\n";return 77;}
