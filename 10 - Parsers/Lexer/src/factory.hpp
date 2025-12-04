#ifndef FACTORY_H
#define FACTORY_H

#include "frag.hpp"
#include "processor.hpp"
#include "state_machine.hpp"

#include <deque>
#include <string_view>
#include <vector>

namespace Kalika
{
  namespace internal
  {
    /**
     * @brief Factory to build NFAs
     */
    struct Factory {
      std::deque<State> storage;
      Frag get(std::string_view regex);

    private:
      // ======= Helper Methods ======= //
      // Make a state for the Frag
      size_t make_state(bool final = false);
      // Build the Frag from regex
      Frag build(std::string_view regex);
      // Evaluate the regex RPN expression to build the Frag
      Frag eval(std::vector<Token> const& token_stack);
      // Assign names to states
      void assign_names();

      // ======= Basic Machines ======= //
      Frag literal(char ch);
      Frag epsilon();

      // ========= Operations ========= //
      // Concatenation AB
      Frag concatenate(Frag const& A, Frag const& B);
      // Alteration A|B
      Frag alteration(Frag const& A, Frag const& B);
      // Kleene-Closure: A*
      Frag kleene(Frag const& A);
      // One or More: A+
      Frag plus(Frag const& A);
      // Or: A?
      Frag one_or_none(Frag const& A);
      // Character class: [a-b]
      Frag char_class(char a, char b);
    };

  }  //namespace internal

  /**
   * @brief Builds an nfa matching the regular expression
   * @param regexp regular expression in general regex format
   */
  StateMachine make_nfa(std::string_view regexp);
};  //namespace Kalika

#endif
