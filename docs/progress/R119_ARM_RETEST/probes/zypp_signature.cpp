#include <zypp/parser/xml/Reader.h>
void probe(const zypp::InputStream& in, const zypp::xml::Validate& v) { zypp::xml::Reader r(in,v); }
