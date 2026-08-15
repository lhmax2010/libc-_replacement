# References

- [Arm Exception Handling ABI, fixed revision ee4b3c1](https://github.com/ARM-software/abi-aa/blob/ee4b3c12d57c8424ff60c2ae56e10690d0604ab6/ehabi32/ehabi32.rst): normative handler/cleanup distinction and C++ catch/cleanup helper protocols. The document does not define the GCC forced-unwind extension.
- GCC source at `49c5060508ce8a6ed6beca375233688a6bdd9140`: `libstdc++-v3/libsupc++/eh_arm.cc`, `eh_personality.cc`, and `eh_throw.cc`; implementation reference for forced-type mapping, handler/cleanup classification, and rethrow.
- [GCC 2012 ARM EHABI discussion](https://gcc.gnu.org/pipermail/gcc-patches/2012-March/336295.html): explicitly notes that forced unwinding was added beyond the official ARM EABI model. Related implementation history, not this exact libc++abi bug.
- [LLVM issue #53849](https://github.com/llvm/llvm-project/issues/53849): open ARMv7 libc++abi `forced_unwind` test issue. It concerns EHABI test-source compatibility, not the handler-as-cleanup failure diagnosed here; it is related context, not the same defect.
- R30-R42 evidence in this repository: direct runtime observations, typeinfo identity experiments, foreign rethrow diagnosis, EHABI frame tracing, and the two-architecture matrix.
