#ifndef FRAG_H
#define FRAG_H

#include <concepts>
#include <cstddef>
#include <deque>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace Kalika::internal
{
  /**
   * @brief Frag with a single start state and a single end state
   */
  struct Frag {
    std::set<char> alphabet;

    Frag(std::size_t start, std::size_t end) : start_(start), end_(end) {}

    // Accessor methods
    std::size_t start() const { return this->start_; }

    std::size_t end() const { return this->end_; }

    /**
     * @brief Add alphabet from nfa to current machine
     */
    void add_alphabet(Frag const& frag);
    /**
     * @brief Update the current nfa's alphabet
     */
    void add_alphabet(char ch);

  private:
    std::size_t start_;
    std::size_t end_;
  };

}  //namespace Kalika::internal

#endif
