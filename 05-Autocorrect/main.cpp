#include <cstdlib>
#include <iostream>

#include "ac.hpp"

int main(int argc, const char *argv[1])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return EXIT_FAILURE;
  }

  AutoCorrect ac{argv[1]};

  while (true) {
    std::string word;
    std::cout << "Input: ";
    std::cin >> word;
    ac.correct(word);
  }

  return EXIT_SUCCESS;
}
