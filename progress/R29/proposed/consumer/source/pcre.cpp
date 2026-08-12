#include <pcrecpp.h>
#include <string>

int main() {
  pcrecpp::RE expression("r29");
  return expression.FullMatch(std::string("r29")) ? 0 : 1;
}
