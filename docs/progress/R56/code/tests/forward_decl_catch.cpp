namespace __cxxabiv1 {
class __forced_unwind;
}
int probe_forward_declaration() {
  try {
    throw 7;
  } catch (const __cxxabiv1::__forced_unwind&) {
    throw;
  } catch (...) {
    return 0;
  }
}
