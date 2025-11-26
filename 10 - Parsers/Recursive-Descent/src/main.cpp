#include "Lexer.hpp"
#include "Parser.hpp"

#include <iostream>

int main()
{
  // Parser test
  // std::stringstream ss{"2 + 3\n 2.0 + (3 * 5.0)\n"};
  Lexer lexer{std::cin};
  Parser parser(lexer);

  while (true) {
    if (!parser.parse()) {
      std::cout << "Invalid Expression\n";
      break;
    }
    std::cout << "Valid Expression\n";
  }
}
