try { ... }
catch (abi::__forced_unwind&) { throw; }   // 识别出来，原样放行
catch (...) { ... }                         // 处理真正的异常
