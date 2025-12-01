#include "nfa.hpp"

#include <cstddef>
#include <set>
#include <string_view>
#include <utility>

// ======== NFA methods ======== //
bool NFA::test(std::string_view input) const
{
  std::set<std::pair<NFAState const*, std::size_t>> visited;
  return this->start_->test(input, 0, visited);
}

void NFA::add_alphabet(const NFA& nfa)
{
  this->alphabet_.insert(nfa.alphabet_.begin(), nfa.alphabet_.end());
}

void NFA::add_alphabet(char ch)
{
  if (ch != '\0') {
    this->alphabet_.insert(ch);
  }
}

void NFA::build_table()
{}

// ======== NFAState methods ======== //
// Add a transition (ch -> state)
void NFAState::add_transition(char ch, NFAState* state)
{
  this->transitions_[ch].insert(state);
}

// Get transitions
std::set<NFAState*> NFAState::get_transition(char ch) const
{
  if (auto it = this->transitions_.find(ch);
      it != this->transitions_.end()) {
    return it->second;
  }

  return {};
}

// Test if input passes the machine starting from the state
bool NFAState::test(
  std::string_view input,
  std::size_t idx,
  std::set<std::pair<NFAState const*, std::size_t>>& visited
) const
{
  // Detect repeated (state, idx)
  if (!visited.insert({this, idx}).second) {
    return false;
  }

  // Accept only if input fully consumed
  if (final && idx == input.size()) {
    return true;
  }

  // Try epsilon transitions
  for (auto* s : get_transition(eps)) {
    if (s->test(input, idx, visited)) {
      return true;
    }
  }

  // Try consuming one character
  if (idx < input.size()) {
    for (auto* s : get_transition(input[idx])) {
      if (s->test(input, idx + 1, visited)) {
        return true;
      }
    }
  }

  return false;
}
