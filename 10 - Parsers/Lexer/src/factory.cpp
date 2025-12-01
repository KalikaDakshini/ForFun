#include "factory.hpp"
#include "nfa.hpp"
#include "processor.hpp"

#include <cassert>
#include <string_view>
#include <vector>

namespace Kalika
{
  NFA Factory::get(std::string_view regex)
  {
    NFA nfa = this->build(regex);
    return nfa;
  }

  NFAState* Factory::make_state(bool final)
  {
    auto& ref = this->storage_.emplace_back(final);
    return &ref;
  }

  NFA Factory::build(std::string_view regex)
  {
    auto token_stack = process(regex);
    return eval(token_stack);
  }

  NFA Factory::eval(std::vector<Token> const& token_stack)
  {
    std::vector<NFA> nfa_stack;

    for (auto const& token : token_stack) {
      // Push chars onto the stack
      if (token.kind == TokenKind::CHAR) {
        nfa_stack.push_back(make_char(token.val));
        continue;
      }
      // Create machine for operand
      switch (token.kind) {
      case TokenKind::ALTER: {
        auto nfa_b = nfa_stack.back();
        nfa_stack.pop_back();
        auto nfa_a = nfa_stack.back();
        nfa_stack.pop_back();
        nfa_stack.push_back(alteration(nfa_a, nfa_b));
        break;
      }
      case TokenKind::CONCAT: {
        auto nfa_b = nfa_stack.back();
        nfa_stack.pop_back();
        auto nfa_a = nfa_stack.back();
        nfa_stack.pop_back();
        nfa_stack.push_back(concatenate(nfa_a, nfa_b));
        break;
      }
      case TokenKind::IF: {
        auto nfa_a = nfa_stack.back();
        nfa_stack.pop_back();
        nfa_stack.push_back(one_or_none(nfa_a));
        break;
      }
      case TokenKind::PLUS: {
        auto nfa_a = nfa_stack.back();
        nfa_stack.pop_back();
        nfa_stack.push_back(plus(nfa_a));
        break;
      }
      case TokenKind::KLEENE: {
        auto nfa_a = nfa_stack.back();
        nfa_stack.pop_back();
        nfa_stack.push_back(kleene(nfa_a));
        break;
      }
      default:
        break;
      }
    }

    return nfa_stack.back();
  }

  NFA Factory::make_char(char ch)
  {
    auto* start = this->make_state();
    auto* end = this->make_state(true);

    start->add_transition(ch, end);

    NFA nfa{start, end};
    nfa.add_alphabet(ch);

    return nfa;
  }

  NFA Factory::make_epsilon()
  {
    return make_char(eps);
  }

  NFA Factory::concatenate(const NFA& A, const NFA& B)
  {
    auto* a_start = A.start();
    auto* a_end = A.end();
    auto* b_start = B.start();
    auto* b_end = B.end();

    // Add transitions
    a_end->add_transition(eps, b_start);

    // Toggle final state
    a_end->toggle();

    NFA nfa{a_start, b_end};
    nfa.add_alphabet(A);
    nfa.add_alphabet(B);

    return nfa;
  }

  NFA Factory::alteration(const NFA& A, const NFA& B)
  {
    auto* a_start = A.start();
    auto* a_end = A.end();
    auto* b_start = B.start();
    auto* b_end = B.end();

    auto* start = make_state();
    auto* end = make_state(true);

    // Add transitions
    start->add_transition(eps, a_start);
    start->add_transition(eps, b_start);

    a_end->add_transition(eps, end);
    b_end->add_transition(eps, end);

    // Remove final state flags
    a_end->toggle();
    b_end->toggle();

    NFA nfa{start, end};
    nfa.add_alphabet(A);
    nfa.add_alphabet(B);

    return nfa;
  }

  // Kleene-Closure: A*
  NFA Factory::kleene(const NFA& A)
  {
    auto* a_start = A.start();
    auto* a_end = A.end();

    a_start->add_transition(eps, a_end);
    a_end->add_transition(eps, a_start);

    return A;
  }

  // One or More: A+
  NFA Factory::plus(const NFA& A)
  {
    auto* a_start = A.start();
    auto* a_end = A.end();

    a_end->add_transition(eps, a_start);

    return A;
  }

  // Or: A?
  NFA Factory::one_or_none(const NFA& A)
  {
    auto* a_start = A.start();
    auto* a_end = A.end();

    a_start->add_transition(eps, a_end);

    return A;
  }

  // Character class: [a-b]
  NFA Factory::char_class(char a, char b)
  {
    auto* start = make_state();
    auto* end = make_state(true);

    assert(a < b);
    NFA nfa{start, end};

    for (char ch = a; ch <= b; ch++) {
      start->add_transition(ch, end);
      nfa.add_alphabet(ch);
    }

    return nfa;
  }

}  //namespace Kalika
