#include <array>
#include <iostream>

#include "NFA.hpp"
#include "lexer.hpp"

int main()
{
  std::array<char const*, 4> const inputs{"0", "19", "002", "ABCD"};

  // Create machine
  Lexer factory;
  NFA lexer = factory.build();

  for (auto const& input : inputs) {
    std::cout << input << ": "
              << (lexer.test(input) ? "Accepted" : "Rejected") << '\n';
  }
}
