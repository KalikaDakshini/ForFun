#include "factory.hpp"
#include "frag.hpp"
#include "processor.hpp"
#include "state_machine.hpp"

#include <cassert>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace Kalika
{
  namespace internal
  {
    Frag Factory::get(std::string_view regex)
    {
      return this->build(regex);
    }

    size_t Factory::make_state(bool final)
    {
      auto index = this->storage.size();
      this->storage.emplace_back(index, final);
      return index;
    }

    // Build an NFA from regex
    Frag Factory::build(std::string_view regex)
    {
      // Create frag from regex
      auto token_stack = process(regex);
      auto frag = eval(token_stack);

      return frag;
    }

    // Evaluate the RPN expression
    Frag Factory::eval(std::vector<Token> const& token_stack)
    {
      std::vector<Frag> frag_stack;

      for (auto const& token : token_stack) {
        // Push chars onto the stack
        if (token.kind == TokenKind::CHAR) {
          frag_stack.push_back(literal(token.val));
          continue;
        }
        // Create machine for operand
        switch (token.kind) {
        case TokenKind::ALTER: {
          auto frag_b = frag_stack.back();
          frag_stack.pop_back();
          auto frag_a = frag_stack.back();
          frag_stack.pop_back();
          frag_stack.push_back(alteration(frag_a, frag_b));
          break;
        }
        case TokenKind::CONCAT: {
          auto frag_b = frag_stack.back();
          frag_stack.pop_back();
          auto frag_a = frag_stack.back();
          frag_stack.pop_back();
          frag_stack.push_back(concatenate(frag_a, frag_b));
          break;
        }
        case TokenKind::IF: {
          auto frag_a = frag_stack.back();
          frag_stack.pop_back();
          frag_stack.push_back(one_or_none(frag_a));
          break;
        }
        case TokenKind::PLUS: {
          auto frag_a = frag_stack.back();
          frag_stack.pop_back();
          frag_stack.push_back(plus(frag_a));
          break;
        }
        case TokenKind::KLEENE: {
          auto frag_a = frag_stack.back();
          frag_stack.pop_back();
          frag_stack.push_back(kleene(frag_a));
          break;
        }
        default:
          break;
        }
      }

      return frag_stack.back();
    }

    Frag Factory::literal(char ch)
    {
      auto start_idx = this->make_state();
      auto end_idx = this->make_state(true);

      this->storage[start_idx].add_transition(ch, &this->storage[end_idx]);

      Frag frag{start_idx, end_idx};
      frag.add_alphabet(ch);

      return frag;
    }

    Frag Factory::epsilon()
    {
      return literal(EPS);
    }

    Frag Factory::concatenate(Frag const& A, Frag const& B)
    {
      auto& a_end = this->storage[A.end()];
      auto& b_start = this->storage[B.start()];

      // Add transitions
      a_end.add_transition(EPS, &b_start);

      // Toggle final state
      a_end.toggle();

      Frag frag{A.start(), B.end()};
      frag.add_alphabet(A);
      frag.add_alphabet(B);

      return frag;
    }

    Frag Factory::alteration(Frag const& A, Frag const& B)
    {
      auto& a_start = this->storage[A.start()];
      auto& b_start = this->storage[B.start()];
      auto& a_end = this->storage[A.end()];
      auto& b_end = this->storage[B.end()];

      auto start_idx = make_state();
      auto end_idx = make_state(true);

      auto& f_start = this->storage[start_idx];
      auto& f_end = this->storage[end_idx];

      // Add transitions
      f_start.add_transition(EPS, &a_start);
      f_start.add_transition(EPS, &b_start);

      a_end.add_transition(EPS, &f_end);
      b_end.add_transition(EPS, &f_end);

      // Remove final state flags
      a_end.toggle();
      b_end.toggle();

      Frag frag{start_idx, end_idx};
      frag.add_alphabet(A);
      frag.add_alphabet(B);

      return frag;
    }

    // Kleene-Closure: A*
    Frag Factory::kleene(Frag const& A)
    {
      auto a_plus = plus(A);

      auto start_idx = make_state();
      auto end_idx = make_state(true);

      auto& a_start = this->storage[a_plus.start()];
      auto& a_end = this->storage[a_plus.end()];
      auto& f_start = this->storage[start_idx];
      auto& f_end = this->storage[end_idx];

      a_end.toggle();

      f_start.add_transition(EPS, &a_start);
      f_start.add_transition(EPS, &f_end);
      a_end.add_transition(EPS, &f_end);

      Frag frag{start_idx, end_idx};
      frag.add_alphabet(A);

      return frag;
    }

    // One or More: A+
    Frag Factory::plus(Frag const& A)
    {
      auto& a_start = this->storage[A.start()];
      auto& a_end = this->storage[A.end()];

      a_end.add_transition(EPS, &a_start);

      return A;
    }

    // Or: A?
    Frag Factory::one_or_none(Frag const& A)
    {
      auto& a_start = this->storage[A.start()];
      auto& a_end = this->storage[A.end()];

      a_start.add_transition(EPS, &a_end);

      return A;
    }

    // Character class: [a-b]
    Frag Factory::char_class(char a, char b)
    {
      auto start_idx = make_state();
      auto end_idx = make_state(true);

      auto& f_start = this->storage[start_idx];
      auto& f_end = this->storage[end_idx];

      assert(a < b);
      Frag frag{start_idx, end_idx};

      for (char ch = a; ch <= b; ch++) {
        f_start.add_transition(ch, &f_end);
        frag.add_alphabet(ch);
      }

      return frag;
    }

  }  //namespace internal

  std::variant<NFA, DFA> make_nfa(std::string_view regexp, bool is_dfa)
  {
    internal::Factory f;
    auto frag = f.get(regexp);

    NFA nfa{
      .storage = std::move(f.storage),
      .alphabet = std::move(frag.alphabet),
      .start = frag.start(),
      .end = frag.end(),
    };

    nfa.print_stats();

    if (nfa.storage.size() > 100 || is_dfa) {
      auto dfa = DFA(nfa);
      dfa.print_stats();
      return dfa;
    }

    return nfa;
  }
}  //namespace Kalika
