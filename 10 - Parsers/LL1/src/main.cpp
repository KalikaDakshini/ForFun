#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"

int main(int argc, char const* argv[])
{
  Lexer lexer{std::cin};
  Parser parser{lexer};

  auto const* gr_file = (argc < 2) ? "./grammars/grammar1.bnf" : argv[1];

  parser.load_grammar(gr_file);
}
