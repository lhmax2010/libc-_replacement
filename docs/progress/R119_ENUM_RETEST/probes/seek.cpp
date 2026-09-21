#include <podofo/base/PdfInputDevice.h>
// Qualified call deliberately suppresses virtual dispatch to expose the real declaration's symbol.
void probe_direct(PoDoFo::PdfInputDevice& d, std::streamoff off, std::ios_base::seekdir dir) {
  d.PoDoFo::PdfInputDevice::Seek(off,dir);
}
// Ordinary virtual call: do not infer the callee's name from this function's own symbol.
void probe_virtual(PoDoFo::PdfInputDevice& d, std::streamoff off, std::ios_base::seekdir dir) {
  d.Seek(off,dir);
}
