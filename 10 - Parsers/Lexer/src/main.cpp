#include <iostream>
#include <string>
#include <variant>

#include "factory.hpp"

int main(int argc, char const* argv[])
{
  //TODO(kalika): Implement text searching (Booyer-Moore?)
  std::string const regexp{(argc < 2) ? "(ab)+c" : argv[1]};
  auto machine = Kalika::make_nfa(regexp);

  std::cout << "Regex: " << regexp << '\n';

  // Set up a stream
  std::istream& in(std::cin);
  std::string input;
  while (std::getline(in, input)) {
    std::cout << (std::visit(
                    [&input](auto const& arg) { return arg.match(input); },
                    machine
                  )
                    ? "Accepted"
                    : "Rejected")
              << '\n';
  }

  return 0;
}
