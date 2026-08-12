#include <flatbuffers/idl.h>

int main() {
  flatbuffers::Parser parser;
  return parser.error_.empty() ? 0 : 1;
}
