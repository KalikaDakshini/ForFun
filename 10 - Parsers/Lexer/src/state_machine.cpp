#include "state_machine.hpp"

#include <set>
#include <string_view>
#include <vector>

namespace Kalika
{
  namespace internal
  {
    // ======== State methods ======== //
    // Add a transition (ch -> state)
    void State::add_transition(char ch, State* state_idx)
    {
      this->transitions_[ch].insert(state_idx);
    }

    // Get transitions
    std::set<State*> State::get_transition(char ch) const
    {
      if (auto it = this->transitions_.find(ch);
          it != this->transitions_.end()) {
        return it->second;
      }

      return {};
    }

    // Test if input passes the machine starting from the state
    bool State::match(std::string_view input, size_t idx) const
    {
      // Accept only if input fully consumed
      if (final && idx == input.size()) {
        return true;
      }

      // Try epsilon transitions
      for (auto* s : get_transition(EPS)) {
        if (s->match(input, idx)) {
          return true;
        }
      }

      // Try consuming one character
      if (idx < input.size()) {
        for (auto* s : get_transition(input[idx])) {
          if (s->match(input, idx + 1)) {
            return true;
          }
        }
      }

      return false;
    }

    std::set<State*> State::closure()
    {
      // Avoid repeated computations
      if (!this->closure_set_.empty()) {
        return closure_set_;
      }

      std::vector<State*> state_stack{this};

      // Perform DFS visiting every state reachable by EPSilon
      while (!state_stack.empty()) {
        State* cur_state = state_stack.back();
        state_stack.pop_back();

        // Add current set to closure_set_
        closure_set_.insert(cur_state);

        for (auto* state : cur_state->get_transition(EPS)) {
          state_stack.push_back(state);
        }
      }

      return closure_set_;
    }
  }  //namespace internal

  bool StateMachine::match(std::string_view input) const
  {
    // Do a dfs on the machine and check if input reaches a final state
    return this->storage[start].match(input, 0);
  }

}  //namespace Kalika
