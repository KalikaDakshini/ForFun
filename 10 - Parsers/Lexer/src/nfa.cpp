#include "nfa.hpp"

#include <set>
#include <string_view>

namespace Kalika
{
  namespace internal
  {
    // ======== NFAState methods ======== //
    // Add a transition (ch -> state)
    void NFAState::add_transition(char ch, NFAState* state_idx)
    {
      this->transitions_[ch].insert(state_idx);
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
    bool NFAState::test(std::string_view input, size_t idx) const
    {
      // Accept only if input fully consumed
      if (final && idx == input.size()) {
        return true;
      }

      // Try epsilon transitions
      for (auto* s : get_transition(EPS)) {
        if (s->test(input, idx)) {
          return true;
        }
      }

      // Try consuming one character
      if (idx < input.size()) {
        for (auto* s : get_transition(input[idx])) {
          if (s->test(input, idx + 1)) {
            return true;
          }
        }
      }

      return false;
    }

    // std::set<std::size_t> NFAState::closure()
    // {
    //   // Avoid repeated computations
    //   if (!this->closure_set.empty()) {
    //     return closure_set;
    //   }

    //   std::vector<std::size_t> state_stack{this};
    //   umap<std::size_t, bool> visited = {
    //     {this, true}
    //   };

    //   // Perform DFS visiting every state reachable by EPSilon
    //   transitions while (!state_stack.empty()) {
    //     std::size_t cur_state = state_stack.back();
    //     state_stack.pop_back();
    //     visited[cur_state] = true;

    //     // Add current set to closure_set
    //     closure_set.insert(cur_state);

    //     for (std::size_t state : cur_state->get_transition(EPS)) {
    //       if (!visited[state]) {
    //         state_stack.push_back(state);
    //       }
    //     }
    //   }

    //   return closure_set;
    // }
  }  //namespace internal

  bool NFA::test(std::string_view input) const
  {
    // Do a dfs on the machine and check if input reaches a final state
    return this->storage[start].test(input, 0);
  }

}  //namespace Kalika
