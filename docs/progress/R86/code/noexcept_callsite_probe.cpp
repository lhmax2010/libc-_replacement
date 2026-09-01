#ifndef TARGET_NOEXCEPT
#  define TARGET_NOEXCEPT
#endif

extern void target() TARGET_NOEXCEPT;
extern void cleanup(int*);

struct guard {
  int value;
  ~guard() { cleanup(&value); }
};

void caller() {
  guard g{};
  target();
}
