#include <boost/program_options/variables_map.hpp>

#include <cstdio>
#include <string>

int main() {
  int result = 0;
  {
    boost::program_options::variables_map values;
    std::size_t initial = values.size();
    std::printf("sizeof_variables_map=%zu initial_size=%zu\n", sizeof(values), initial);
    std::fflush(stdout);

    values.emplace(
        std::string("r82-key"),
        boost::program_options::variable_value(boost::any(82), false));
    std::size_t after = values.size();
    auto found = values.find(std::string("r82-key"));
    bool value_ok = found != values.end() && found->second.as<int>() == 82;
    std::printf("after_size=%zu value_ok=%d\n", after, value_ok ? 1 : 0);
    std::fflush(stdout);
    if (initial != 0 || after != 1 || !value_ok) result = 71;
  }  // Provider constructor initialized the object; libc++ destroys it here.
  std::printf("destruction_completed=1 result=%d\n", result);
  return result;
}
