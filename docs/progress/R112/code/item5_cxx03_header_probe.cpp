#include <condition_variable>
std::condition_variable* cv_pointer = 0;
int main() { return cv_pointer != 0; }
