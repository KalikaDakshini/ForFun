#ifndef BK_TREE_H
#define BK_TREE_H

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Word {
  std::string entry;
  std::size_t freq;
  std::size_t dist;

  // Return the Bayes' metric
  double metric(const std::string &word, double alpha = 0.0) const
  {
    double len_boost = (word.length() == entry.length()) ? 3 : 0;
    double start_boost = (entry.starts_with(word[0])) ? 5 : 0;
    return log(freq + 1) - alpha * dist + len_boost + start_boost;
  }
};

class BkTree

{
  class BkNode
  {
    Word word_;
    std::unordered_map<std::size_t, std::unique_ptr<BkNode>> children_;

  public:
    explicit BkNode(const Word &word) : word_(word), children_() {}

    // Public methods
    /**
     * @brief Insert a word into the BK-Node
     */
    void insert(const Word &word);

    void search(
      const std::string &word,
      std::size_t max_distance,
      std::vector<Word> &words
    );

    // Perform a dfs starting from node
    void dfs();

    void print_tree()
    {
      std::cout << this->word_.entry << ": ";
      // Print all the children
      for (const auto &[dist, node] : this->children_) {
        std::cout << node->word_.entry << ", ";
      }
      std::cout << '\n';

      // Print subsequent trees
      for (const auto &[dist, node] : this->children_) {
        node->print_tree();
      }
    }
  };

  std::unique_ptr<BkNode> root_;

public:
  BkTree() : root_(nullptr) {}

  // Public methods
  /**
   * @brief Insert a word into the BK-Tree
   */
  void insert(const Word &word);

  /**
   * @brief Search for the word closest to input in the dictionary in terms of
   * levenshtein distance
   *
   * @param word input word
   * @return std::string The word closest to input
   */
  std::vector<Word> search(const std::string &word, std::size_t distance);

  // Perform a dfs, listing all words in the tree
  void dfs();

  void print_tree() { this->root_->print_tree(); }
};

#endif
