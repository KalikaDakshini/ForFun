#ifndef FACTORY_H
#define FACTORY_H

#include "nfa.hpp"

#include <deque>
#include <string_view>

namespace Kalika
{
  struct Factory {
    NFA get(std::string_view regex);

  private:
    std::deque<NFAState> storage_;

    // ======= Helper Methods ======= //
    NFA build(std::string_view regex);
    NFAState* make_state(bool final = false);

    // ======= Basic Machines ======= //
    NFA make_char(char ch);
    NFA make_epsilon();

    // ========= Operations ========= //
    // Concatenation AB
    template<typename Arg, typename... Args>
    requires(sizeof...(Args) >= 1)
    NFA concatenate(Arg a, Arg b, Args... args);
    NFA concatenate(const NFA& A, const NFA& B);
    // Alteration A|B
    template<typename Arg, typename... Args>
    requires(sizeof...(Args) >= 1)
    NFA alteration(Arg a, Arg b, Args... args);
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
