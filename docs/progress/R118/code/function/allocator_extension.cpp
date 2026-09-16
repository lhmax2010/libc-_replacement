#include <functional>
#include <memory>
#include <cstdio>
int main(){std::function<int()> f(std::allocator_arg,std::allocator<int>(),[]{return 17;});std::printf("value=%d\n",f());}
