#include "bridge.h"
_Static_assert(sizeof(P12Span)==sizeof(void*)+sizeof(size_t),"span ABI");
int main(void){P12CookieState state={0};return state.caller_errno;}
