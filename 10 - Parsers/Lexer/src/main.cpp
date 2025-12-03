#include <iostream>
#include <string>

#include "factory.hpp"

int main(int argc, char const* argv[])
{
  //TODO(kalika): Implement text searching (Booyer-Moore?)
  auto nfa = Kalika::make_nfa((argc < 2) ? "(ab)+c" : argv[1]);

  // Set up a stream
  std::istream& in(std::cin);

  std::string input;
  while (std::getline(in, input)) {
    std::cout << (nfa.test(input) ? "Accepted" : "Rejected") << '\n';
  }

  return 0;
}
