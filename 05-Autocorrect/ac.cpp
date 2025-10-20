#include <algorithm>
#include <cstdlib>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

#include "ac.hpp"
#include "bk_tree.hpp"

AutoCorrect::AutoCorrect(const std::string &freq_path)
{
  // Load dictionary files
  std::ifstream freq_file(freq_path);
  if (!freq_file) {
    throw std::runtime_error(fmt::format("File {} not found", freq_path));
  }

  // Add words to file
  std::string word;
  while (freq_file >> word) {
    // Split csv on ','
    auto view =
      word | std::views::split(',') | std::views::transform([](auto view) {
        return std::string(view.begin(), view.end());
      });
    std::vector<std::string> fields(view.begin(), view.end());
    if (fields.size() < 2) {
      throw std::runtime_error("Improper file format. Expected: <word>,<freq>");
    }
    // Insert word into tree
    const Word word{.entry = fields[0], .freq = std::stoul(fields[1])};
    this->wordtree_.insert(word);
  }
}

void AutoCorrect::correct(const std::string &word, std::size_t distance)
{
  auto words = this->wordtree_.search(word, distance);

  std::cout << "Suggestions: ";
  // Sort the words
  std::ranges::sort(words, [](const auto &w1, const auto &w2) {
    return w1.freq > w2.freq;
  });

  // Print the top 3 words in terms of frequency
  for (const auto &word : words | std::views::take(3)) {
    std::cout << word.entry << ", ";
  }

  std::cout << '\n';
}
