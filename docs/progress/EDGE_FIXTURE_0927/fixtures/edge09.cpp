#include "identity.h"
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
static Lifetime counts;
class Formatting : public ::testing::Test {
 public: Formatting(){++counts.constructed;} ~Formatting()override{++counts.destroyed;}
};
TEST_F(Formatting,ThroughPublicAssertion){
 ::testing::TestPartResultArray results;
 {::testing::ScopedFakeTestPartResultReporter reporter(::testing::ScopedFakeTestPartResultReporter::INTERCEPT_ONLY_CURRENT_THREAD,&results);EXPECT_DOUBLE_EQ(1.25,2.5);}
 assert(results.size()==1);const auto&result=results.GetTestPartResult(0);assert(result.type()==::testing::TestPartResult::kNonFatalFailure);
 const std::string message=result.message();const std::string expected="Expected equality of these values:\n  1.25\n  2.5\n";
 std::cout<<"VALUES nonfatal="<<results.size()<<" diagnostic="<<message<<'\n'<<std::flush;assert(message==expected);
}
int main(int argc,char**argv){identity(argc,argv);int n=1;::testing::InitGoogleTest(&n,argv);int rc=RUN_ALL_TESTS();assert(rc==0);lifecycle(counts,1);return rc;}
