#include "NFA.hpp"

#include <algorithm>
#include <string_view>

// ======== State Functions ======== //
void State::add_transition(char ch, State* s)
{
  this->transitions_[ch].push_back(s);
}

bool State::test(std::string_view in_str) const
{
  // String exhausted and final state reached
  if (this->final_ && in_str.empty()) {
    return true;
  }

  // Check epsilon transitions
  bool eps_result = false;
  if (this->transitions_.contains(eps)) {
    eps_result = std::ranges::any_of(
      this->transitions_.at(eps),
      [&in_str](State const* s) { return s->test(in_str); }
    );
  }

  // Final state not reached, but string exhausted
  if (!this->final_ && in_str.empty()) {
    return eps_result;
  }

  bool non_eps_result = false;
  // Search non-epsilon transitions
  auto ch = in_str.front();
  if (this->transitions_.contains(ch)) {
    non_eps_result = std::ranges::any_of(
      this->transitions_.at(ch), [&in_str](State const* s) {
        return s->test(in_str.substr(1, in_str.length()));
      }
    );
  }

  // Not final state, input not empty, continue searching
  return eps_result || non_eps_result;
}

// ======== NFA Functions ======== //
bool NFA::test(std::string_view in_str) const
{
  return this->start_->test(in_str);
}
