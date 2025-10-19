#ifndef AC_HPP
#define AC_HPP

#include <vector>

#include "trie.hpp"

class AutoComplete
{
private:
  Trie trie_;

  void _get_words(
    const Node *node,
    std::string &word,
    std::vector<std::pair<std::string, unsigned int>> &word_map
  );

public:
  AutoComplete(const std::string &path);

  unsigned int count(const std::string &word)
  {
    const Node *node = this->trie_.prefix(word);
    if (node == nullptr) {
      return 0;
    }
    return node->count();
  }

  /**
   * @brief Finds the best matches for a given word.
   *
   * This function returns the top `count` matches that are most similar to
   * the input `word`
   *
   * @param word  The word to find matches for.
   * @param count The number of matches to return.
   * @return A vector containing the best-matched strings.
   */
  std::vector<std::string>
  process(const std::string &word, unsigned int count = 3);
};

#endif
