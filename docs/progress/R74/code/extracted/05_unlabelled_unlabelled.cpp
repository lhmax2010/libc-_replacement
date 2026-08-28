// codes/llvm/libcxx/src/ios.cpp

void ios_base::__set_badbit_and_consider_rethrow() {
    __rdstate_ |= badbit;
    if (__exceptions_ & badbit)
        throw;
}
