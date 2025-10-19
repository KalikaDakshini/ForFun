#ifndef TRIE_HPP
#define TRIE_HPP

/**
 * @file trie.hpp
 * @author KalikaDakshini
 * @brief
 * @version 0.1
 * @date 2025-10-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cctype>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

class Node
{
private:
  char ch_;
  bool end_;
  unsigned int count_;
  std::vector<std::unique_ptr<Node>> children_;

public:
  explicit Node() : Node('\0') {}

  explicit Node(char ch) : ch_(ch), end_(false), count_(0), children_(26) {}

  // Get the node
  [[nodiscard]] Node *get_node(char ch)
  {
    return this->children_[std::tolower(ch) - 'a'].get();
  }

  // Add a node to the trie
  [[nodiscard]] Node *add_node(char ch);

  // Get the character of the node
  char ch() const { return ch_; }

  // True if node is end
  [[nodiscard]] bool end() const { return this->end_; }

  // Set it as end node
  void set_end() { this->end_ = true; }

  // Get children
  auto get_children() const
  {
    return this->children_ |
           std::views::filter([](const auto &ptr) { return ptr != nullptr; });
  }

  // Get count of the word
  unsigned int count() const { return this->count_; }

  // Return count of word
  void set_count(unsigned int count) { this->count_ = count; }
};

class Trie
{
private:
  std::unique_ptr<Node> root_;

public:
  Trie() : root_(std::make_unique<Node>('\0')) {}

  // Public methods
  /**
   * @brief Add a word to the trie
   *
   * @param word Word to be inserted
   */
  void insert(const std::string &word, unsigned int count);
  /**
   * @brief Search for the word in the trie
   * @param word Word to be searched for
   * @return True if word is in the trie
   */
  [[nodiscard]] bool search(const std::string &word) const;
  /**
   * @brief Search the trie for the prefix
   *
   * @param word Word to be searched for
   * @return const Node* Position of the node in the trie
   */
  [[nodiscard]] const Node *prefix(const std::string &word) const;
};

#endif
