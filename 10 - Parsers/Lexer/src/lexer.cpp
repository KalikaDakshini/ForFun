#include "lexer.hpp"
#include "NFA.hpp"

#include <cassert>
#include <string>
#include <string_view>

Lexer::Lexer() : core_(this->build())
{}

bool Lexer::lex(std::string_view input)
{
  return this->core_.test(input);
}

// ======= Helper Methods ======= //
NFA Lexer::build()
{
  // return this->make_epsilon();
  // return concatenate(make_char('A'), make_char('B'), make_char('C'));
  // return disjunction(make_char('A'), make_char('B'), make_char('C'));
  // Recognize natural numbers
  return disjunction(
    concatenate(char_class('1', '9'), closure(char_class('0', '9'))),
    make_char('0')
  );
}

// ======= Factory Methods ======= //
State* Lexer::make_state(std::string const& name, bool final)
{
  auto& ref = this->storage_.emplace_back(name, final);
  return &ref;
}

// Single character transition machine
NFA Lexer::make_char(char ch)
{
  State* start = this->make_state("0");
  State* end = this->make_state("1", true);

  start->add_transition(ch, end);

  return {start, end};
}

// Epsilon transition machine
NFA Lexer::make_epsilon()
{
  return this->make_char(eps);
}

// Concatenation operation
template<typename Arg, typename... Args>
requires(sizeof...(Args) >= 1)
NFA Lexer::concatenate(Arg a, Arg b, Args... args)
{
  return concatenate(concatenate(a, b), args...);
}

NFA Lexer::concatenate(NFA A, NFA B)
{
  State* a_start = A.start();
  State* a_end = A.end();
  State* b_start = B.start();
  State* b_end = B.end();

  NFA concat{a_start, b_end};
  // Toggle a_end's final status
  a_end->toggle();

  // Add an epsilon transition between a_end and b_start
  a_end->add_transition(eps, b_start);

  return concat;
}

// Concatenation operation
template<typename Arg, typename... Args>
requires(sizeof...(Args) >= 1)
NFA Lexer::disjunction(Arg a, Arg b, Args... args)
{
  return disjunction(disjunction(a, b), args...);
}

NFA Lexer::disjunction(NFA A, NFA B)
{
  State* a_start = A.start();
  State* a_end = A.end();
  State* b_start = B.start();
  State* b_end = B.end();

  // Toggle end states' final status
  a_end->toggle();
  b_end->toggle();
  State* c_start = this->make_state("c0");

  State* c_end = this->make_state("c1", true);
  NFA disjunct{c_start, c_end};

  // Add an epsilon transition between new states and old states
  c_start->add_transition(eps, a_start);
  c_start->add_transition(eps, b_start);

  a_end->add_transition(eps, c_end);
  b_end->add_transition(eps, c_end);

  return disjunct;
}

// Closure : A*
NFA Lexer::closure(NFA A)
{
  State* a_start = A.start();
  State* a_end = A.end();

  State* start = this->make_state("closure_0");
  State* end = this->make_state("closure_1", true);

  start->add_transition(eps, a_start);
  a_end->add_transition(eps, end);
  a_end->toggle();
  a_end->add_transition(eps, a_start);

  return {start, end};
}

// Plus: A+
NFA Lexer::plus(NFA A)
{
  State* a_start = A.start();
  State* a_end = A.end();

  a_end->add_transition(eps, a_start);

  return A;
}

// Optional: A?
NFA Lexer::opt(NFA A)
{
  State* a_start = A.start();
  State* a_end = A.end();

  a_start->add_transition(eps, a_end);

  return A;
}

// Character class [a-z]
NFA Lexer::char_class(char a, char b)
{
  assert(a < b);

  State* start = this->make_state("class_0");
  State* end = this->make_state("class_1", true);

  for (char ch = a; ch <= b; ch++) {
    start->add_transition(ch, end);
  }

  return {start, end};
}
