#ifndef AC_H
#define AC_H

#include <string>

#include "bk_tree.hpp"

class AutoCorrect
{
  BkTree wordtree_;

public:
  explicit AutoCorrect(const std::string &freq_path);

  /**
   * @brief Correct the word to the closest match in dictionary
   */
  void correct(
    const std::string &word, std::size_t distance = 3, std::size_t count = 5
  );
};

#endif
