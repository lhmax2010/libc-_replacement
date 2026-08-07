#define _GNU_SOURCE
#include <elf.h>
#include <link.h>
#include <stdint.h>

static size_t append(char *buffer, size_t offset, size_t capacity,
                     const char *text) {
  if (!text)
    text = "<null>";
  while (*text && offset < capacity)
    buffer[offset++] = *text++;
  return offset;
}

static int equal(const char *left, const char *right) {
  while (*left && *right && *left == *right) {
    ++left;
    ++right;
  }
  return *left == *right;
}

static int observed_symbol(const char *name) {
  return equal(name, "_Unwind_RaiseException") ||
         equal(name, "_Unwind_Resume") ||
         equal(name, "_Unwind_DeleteException") ||
         equal(name, "_Unwind_ForcedUnwind");
}

static void raw_write(const char *buffer, size_t length) {
  register long syscall_number __asm__("rax") = 1;
  register long file_descriptor __asm__("rdi") = 2;
  register const char *data __asm__("rsi") = buffer;
  register size_t count __asm__("rdx") = length;
  __asm__ volatile("syscall"
                   : "+r"(syscall_number)
                   : "r"(file_descriptor), "r"(data), "r"(count)
                   : "rcx", "r11", "memory");
}

static void emit(const char *kind, uintptr_t *refcook, uintptr_t *defcook,
                 const char *symbol) {
  if (!observed_symbol(symbol))
    return;
  const struct link_map *referrer = (const struct link_map *)*refcook;
  const struct link_map *provider = (const struct link_map *)*defcook;
  const char *referrer_name =
      referrer && referrer->l_name && referrer->l_name[0]
          ? referrer->l_name
          : "<main>";
  const char *provider_name =
      provider && provider->l_name && provider->l_name[0]
          ? provider->l_name
          : "<main>";
  char buffer[4096];
  size_t length = 0;
  length = append(buffer, length, sizeof(buffer), "AUDIT_");
  length = append(buffer, length, sizeof(buffer), kind);
  length = append(buffer, length, sizeof(buffer), "\treferrer=");
  length = append(buffer, length, sizeof(buffer), referrer_name);
  length = append(buffer, length, sizeof(buffer), "\tprovider=");
  length = append(buffer, length, sizeof(buffer), provider_name);
  length = append(buffer, length, sizeof(buffer), "\tsymbol=");
  length = append(buffer, length, sizeof(buffer), symbol);
  if (length < sizeof(buffer))
    buffer[length++] = '\n';
  raw_write(buffer, length);
}

unsigned int la_version(unsigned int version) {
  (void)version;
  return LAV_CURRENT;
}

unsigned int la_objopen(struct link_map *map, Lmid_t lmid,
                        uintptr_t *cookie) {
  (void)lmid;
  *cookie = (uintptr_t)map;
  return LA_FLG_BINDTO | LA_FLG_BINDFROM;
}

uintptr_t la_symbind64(Elf64_Sym *symbol, unsigned int index,
                       uintptr_t *refcook, uintptr_t *defcook,
                       unsigned int *flags, const char *name) {
  (void)index;
  (void)flags;
  emit("BIND", refcook, defcook, name);
  return symbol->st_value;
}

Elf64_Addr la_x86_64_gnu_pltenter(Elf64_Sym *symbol, unsigned int index,
                                  uintptr_t *refcook, uintptr_t *defcook,
                                  La_x86_64_regs *registers,
                                  unsigned int *flags, const char *name,
                                  long int *frame_size) {
  (void)index;
  (void)registers;
  (void)flags;
  (void)frame_size;
  emit("CALL", refcook, defcook, name);
  return symbol->st_value;
}
