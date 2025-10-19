#include <iostream>
#include <vector>

#include "ac.hpp"

int main()
{
  // Initiate autocomplete system
  AutoComplete system("./word_map.csv");

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

  return 0;
}
