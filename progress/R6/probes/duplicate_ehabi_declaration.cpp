#include <unwind.h>

extern "C" _Unwind_Reason_Code __gnu_unwind_frame(_Unwind_Exception*, _Unwind_Context*);

int duplicate_declaration_probe() { return 0; }
