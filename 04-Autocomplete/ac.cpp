#include "ac.hpp"

#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "trie.hpp"

// Load the database into memory
AutoComplete::AutoComplete(const std::string &path)
{
  std::ifstream in_file(path);

  std::string line;
  while (in_file >> line) {
    // Split using ranges
    auto split_view =
      line | std::views::split(';') | std::views::transform([](auto r) {
        return std::string(r.begin(), r.end());
      });
    // Store data in a vector
    auto fields = std::vector(split_view.begin(), split_view.end());
    this->trie_.insert(fields[0], std::stoi(fields[1]));
  }
}

void AutoComplete::_get_words(
  const Node *node,
  std::string &word,
  std::vector<std::pair<std::string, unsigned int>> &word_map
)
{
  if (node == nullptr) {
    return;
  }
  // Push characters into word
  word.push_back(node->ch());

  // End of the word, update the map
  if (node->end()) {
    word_map.emplace_back(word, node->count());
  }

  // Repeat for every child
  for (const auto &child : node->get_children()) {
    _get_words(child.get(), word, word_map);
  }

  // Remove the word
  word.pop_back();
}

std::vector<std::string>
AutoComplete::process(const std::string &word, unsigned int count)
{
  const Node *node = this->trie_.prefix(word);

  if (node == nullptr) {
    return std::vector(3, std::string());
  }

  // Get the mapping of words with their frequency
  std::vector<std::pair<std::string, unsigned int>> word_map;
  std::string buffer;

  for (const auto &child : node->get_children()) {
    _get_words(child.get(), buffer, word_map);
  }

  // Sort and get the top count words
  std::ranges::sort(word_map, [](const auto &p1, const auto &p2) {
    return p2.second < p1.second;
  });

  std::vector<std::string> top_words{count};
  std::transform(
    word_map.begin(),
    word_map.begin() + count,
    top_words.begin(),
    [&word](const auto &p) { return (word + p.first); }
  );

  return top_words;
}
