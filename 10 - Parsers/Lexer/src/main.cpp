#include <iostream>
#include <string>

#include "factory.hpp"

int main(int argc, char const* argv[])
{
  //TODO(kalika): Implement text searching (Booyer-Moore?)
  auto nfa = Kalika::make_nfa((argc < 2) ? "(ab)+c" : argv[1]);

  std::cout << "Enter your string:" << '\n';

  // Set up a stream
  std::istream& in(std::cin);
  std::string input;
  while (std::getline(in, input)) {
    std::cout << (nfa.match(input) ? "Accepted" : "Rejected") << '\n';
  }

  return 0;
}
