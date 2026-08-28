// codes/gcc/libstdc++-v3/libsupc++/eh_personality.cc:563-573

#if __cpp_rtti
      // During forced unwinding, match a magic exception type.
      if (actions & _UA_FORCE_UNWIND)
        {
          throw_type = &typeid(abi::__forced_unwind);
        }
      // With a foreign exception class, there's no exception type.
      else if (foreign_exception)
        {
          throw_type = &typeid(abi::__foreign_exception);
        }
      else
