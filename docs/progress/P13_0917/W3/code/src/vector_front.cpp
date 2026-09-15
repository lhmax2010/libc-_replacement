#include "bridge.h"
#include <vector>
#include <cerrno>
#include <new>
#include <stdexcept>
P12_EXPORT int p12_send_reply(int32_t instance, const std::vector<uint8_t>& bytes) {
  try { return p12_vector_bytes(instance, bytes.data(), bytes.size()); }
  catch(const std::bad_alloc&){return -ENOMEM;}
  catch(const std::length_error&){return -EOVERFLOW;}
  catch(const std::invalid_argument&){return -EINVAL;}
  catch(const std::exception&){return -EIO;}
  catch(...){return -EIO;}
}
