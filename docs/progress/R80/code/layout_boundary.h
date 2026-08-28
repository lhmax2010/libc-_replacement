#pragma once

#include <cstddef>
#include <deque>

// Return types do not participate in Itanium C++ name mangling.  The producer
// and consumer therefore agree on the symbol name while compiling std::deque
// with different standard-library object layouts.
std::deque<int>* make_payload();

extern "C" std::size_t provider_size(const void* payload);
extern "C" void destroy_payload(void* payload);
