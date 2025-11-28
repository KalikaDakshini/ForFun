#include <array>
#include <iostream>

#include "lexer.hpp"

int main()
{
  Lexer lexer;

  std::array<char const*, 4> const inputs{"0", "19", "002", "ABCD"};

  for (auto const& input : inputs) {
    std::cout << input << ": "
              << (lexer.lex(input) ? "Accepted" : "Rejected") << '\n';
  }
}
