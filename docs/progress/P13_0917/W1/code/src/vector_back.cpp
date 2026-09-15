#include "bridge.h"
#include "provider_loader.h"
#include <vector>
#include <new>
#include <stdexcept>
#ifndef P12_VECTOR_PROVIDER_PATH
#error "Supply the explicit data-layer receiver path; there is no real callback default"
#endif
namespace {
p12_loader::State provider;
thread_local bool opening = false;
void initialize() { provider.error=p12_loader::resolve(provider,P12_VECTOR_PROVIDER_PATH,"p12_send_reply"); }
int ready() {
  if(opening)return EDEADLK;
  opening=true;
  struct Reset { ~Reset(){opening=false;} } reset;
  int rc=pthread_once(&provider.once,initialize);
  return rc ? rc : provider.error;
}
}
extern "C" int p12_vector_bytes(int32_t instance,const uint8_t* bytes,size_t length) {
  if(!bytes && length)return -EINVAL;
  try {
    std::vector<uint8_t> local;
    if(length>local.max_size())return -EOVERFLOW;
    int error=ready();if(error)return -error;
    if(length)local.assign(bytes,bytes+length);
#ifdef P12_CORRUPT
    if(!local.empty())local[0]^=0x80;
#endif
    using Sink=int(*)(int32_t,const std::vector<uint8_t>&);
    // Constructed receiver contract only. XW_Extension_SyncMessage.h:36 has a
    // void callback and real instance registration: those are not implemented.
    return reinterpret_cast<Sink>(provider.symbol)(instance,local);
  } catch(const std::bad_alloc&){return -ENOMEM;}
    catch(const std::length_error&){return -EOVERFLOW;}
    catch(const std::invalid_argument&){return -EINVAL;}
    catch(const std::exception&){return -EIO;}
    catch(...){return -EIO;}
}
