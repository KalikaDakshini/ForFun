#include "trie.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

// Add node to the children
Node *Node::add_node(char ch)
{
  const unsigned int idx = std::tolower(ch) - 'a';
  if (this->children_[idx] == nullptr) {
    this->children_[idx] = std::make_unique<Node>(ch);
  }

  return this->children_[idx].get();
}

// Insert a node into trie
void Trie::insert(const std::string &word, unsigned int count)
{
  Node *cur{root_.get()};

  // Convert the word to lower case
  std::string lower{word};
  std::ranges::transform(word, lower.begin(), [](char c) {
    return std::tolower(c);
  });

  // Add words by character
  for (auto ch : lower) {
    cur = cur->add_node(ch);
  }

  // Set end of word
  cur->set_end();
  cur->set_count(count);
}

[[nodiscard]] const Node *Trie::prefix(const std::string &word) const
{
  // Convert the word to lower case
  std::string lower{word};
  std::ranges::transform(word, lower.begin(), [](char c) {
    return std::tolower(c);
  });

  Node *cur = this->root_.get();
  for (auto ch : word) {
    cur = cur->get_node(ch);
    // Encountered a null before prefix is scanned
    if (cur == nullptr) {
      return nullptr;
    }
  }

  return cur;
}

[[nodiscard]] bool Trie::search(const std::string &word) const
{
  const Node *cur = this->prefix(word);
  return cur == nullptr ? false : cur->end();
}
