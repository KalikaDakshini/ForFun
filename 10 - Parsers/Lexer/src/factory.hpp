#ifndef FACTORY_H
#define FACTORY_H

#include "nfa.hpp"
#include "processor.hpp"

#include <deque>
#include <string_view>
#include <vector>

namespace Kalika
{
  struct Factory {
    NFA get(std::string_view regex);

  private:
    std::deque<NFAState> storage_;

    // ======= Helper Methods ======= //
    // Make a state for the NFA
    NFAState* make_state(bool final = false);
    // Build the NFA from regex
    NFA build(std::string_view regex);
    // Evaluate the regex RPN expression to build the NFA
    NFA eval(std::vector<Token> const& token_stack);

    // ======= Basic Machines ======= //
    NFA make_char(char ch);
    NFA make_epsilon();

    // ========= Operations ========= //
    // Concatenation AB
    NFA concatenate(const NFA& A, const NFA& B);
    // Alteration A|B
    NFA alteration(const NFA& A, const NFA& B);
    // Kleene-Closure: A*
    NFA kleene(const NFA& A);
    // One or More: A+
    NFA plus(const NFA& A);
    // Or: A?
    NFA one_or_none(const NFA& A);
    // Character class: [a-b]
    NFA char_class(char a, char b);
  };

};  //namespace Kalika

#endif
