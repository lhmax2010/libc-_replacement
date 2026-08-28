// codes/gcc/libstdc++-v3/libsupc++/eh_throw.cc:123-131

#ifdef __USING_SJLJ_EXCEPTIONS__
      _Unwind_SjLj_Resume_or_Rethrow (&header->unwindHeader);
#else
#if defined(_LIBUNWIND_STD_ABI)
      _Unwind_RaiseException (&header->unwindHeader);
#else
      _Unwind_Resume_or_Rethrow (&header->unwindHeader);    // ← 我们平台走的是这一支
#endif
#endif
