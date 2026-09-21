#include <dali/devel-api/threading/conditional-wait.h>
void probe(Dali::ConditionalWait& w, const Dali::ConditionalWait::ScopedLock& l, Dali::ConditionalWait::TimePoint t) { w.WaitUntil(l,t); }
