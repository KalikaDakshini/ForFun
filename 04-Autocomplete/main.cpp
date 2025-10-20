#include <cstdlib>
#include <iostream>
#include <vector>

#include "ac.hpp"

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return EXIT_FAILURE;
  }

  // Initiate autocomplete system
  AutoComplete system(argv[1]);

  std::string word;
  while (std::cin >> word) {
    // Get best matches
    const std::vector<std::string> words = system.process(word);
    // Print best matches
    for (const auto &word : words) {
      std::cout << word << " ";
    }
    std::cout << '\n';
  }

  return EXIT_SUCCESS;
}
