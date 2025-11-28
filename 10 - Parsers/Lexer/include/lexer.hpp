#include "NFA.hpp"

#include <deque>
#include <string>

struct Lexer {
  /**
   * @brief Bulid a NFA according to rules
   */
  NFA build(/* Build according to a set of rules */);

private:
  std::deque<State> storage_;

  // ======= Helper Methods ======= //

  // ======= Factory Methods ======= //
  // Build states for NFA to use
  State* make_state(std::string const& name, bool final = false);

  // Build basic fragments
  NFA make_char(char c);
  NFA make_epsilon();

  // Build basic operations

  // Concatenate : AB
  template<typename Arg, typename... Args>
  requires(sizeof...(Args) >= 1)
  NFA concatenate(Arg a, Arg b, Args... args);
  NFA concatenate(NFA a, NFA b);
  // Disjunction : A | B
  template<typename Arg, typename... Args>
  requires(sizeof...(Args) >= 1)
  NFA disjunction(Arg a, Arg b, Args... args);
  NFA disjunction(NFA a, NFA b);
  // Closure : A*
  NFA closure(NFA A);
  // Plus: A+
  NFA plus(NFA A);
  // Optional: A?
  NFA opt(NFA A);
  // Character class [a-z]
  NFA char_class(char a, char b);
};
