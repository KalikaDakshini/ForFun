#include "state_machine.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
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

    // ======== DFAState methods ======== //
    // Add a transition (ch -> state)
    void DFAState::add_transition(char ch, size_t state)
    {
      this->transitions_[ch] = state;
    }

    // Get transitions
    size_t DFAState::get_transition(char ch) const
    {
      // Every state in DFA has a transition for every alphabet
      return this->transitions_.at(ch);
    }
  }  //namespace internal

  // ======== NFA methods ======== //
  bool NFA::match(std::string_view input) const
  {
    // Do a dfs on the machine and check if input reaches a final state
    return this->storage[start].match(input, 0);
  }

  // Return the epsilon closure of the state
  [[nodiscard]] std::set<internal::State*> NFA::closure(size_t state_idx)
  {
    return this->storage[state_idx].closure();
  }

  void NFA::print_stats() const
  {
    std::cout << "NFA States: " << this->storage.size() << '\n';
  }

  // ======== DFA methods ======== //
  // Construct from NFA an equivalent DFA
  DFA::DFA(NFA& nfa)
  {
    // Copy alphabet
    this->alphabet_ = nfa.alphabet;
    this->start_ =
      this->make_state(nfa.storage[nfa.start].closure()).second;

    std::vector<size_t> states{this->start_};

    // Build DFA States
    while (!states.empty()) {
      // Get the top most state
      auto& cur_state = this->storage_[states.back()];
      states.pop_back();

      // Build transitions for current state
      for (char const ch : nfa.alphabet) {
        std::set<internal::State*> closure_set;
        // Gather all transitions corresponding to an alphabet
        for (size_t const state_idx : cur_state.indices()) {
          auto next_states = nfa.storage[state_idx].get_transition(ch);
          // If there are no transitions from current state
          if (next_states.empty()) {
            continue;
          }

          // [Invariant] Assumes a single transition from a state
          auto step_idx = (*(next_states.begin()))->pos;
          closure_set.merge(nfa.closure(step_idx));
        }

        // Make a new state
        auto [new_state, next_idx] = make_state(closure_set);

        // Map the transition
        cur_state.add_transition(ch, next_idx);

        // If the generated state is new, push it for further processing
        if (new_state) {
          states.push_back(next_idx);
        }
      }
    }

    // Make a trap state to gather all missed transitions
    auto [_, trap_idx] = this->make_state({});
    for (char const ch : this->alphabet_) {
      this->storage_[trap_idx].add_transition(ch, trap_idx);
    }

    for (auto& state : this->storage_) {
      for (auto const ch : this->alphabet_) {
        if (!state.is_mapped(ch)) {
          state.add_transition(ch, trap_idx);
        }
      }
    }
  }

  // Perform match using DFA
  bool DFA::match(std::string_view input) const
  {
    auto cur_state = this->storage_[this->start_];
    // Traverse the graph till the input is exhausted
    for (char const ch : input) {
      size_t const next_idx = cur_state.get_transition(ch);
      cur_state = this->storage_[next_idx];
    }

    return cur_state.final;
  }

  // Print the DFA table
  void DFA::print() const
  {
    // --- Alphabet ---
    std::cout << "Alphabet:\n";
    for (char const c : alphabet_) {
      std::cout << c << ' ';
    }
    std::cout << "\n\n";

    // --- Start state ---
    std::cout << "Start state:\n";
    std::cout << storage_[start_].name << "\n\n";

    // --- Transition table ---
    std::cout << "Transition table:\n";

    // Header
    std::cout << std::setw(15) << "State";
    for (char const c : alphabet_) {
      std::cout << std::setw(15) << c;
    }
    std::cout << '\n';

    // Rows
    for (auto const& state : storage_) {
      std::cout << std::setw(15) << state.name;

      for (char const c : alphabet_) {
        auto to = state.get_transition(c);
        std::cout << std::setw(15) << storage_[to].name;
      }

      std::cout << '\n';
    }
  }

  void DFA::print_stats() const
  {
    std::cout << "DFA States: " << this->storage_.size() << '\n';
  }

  // Return the state corresponding to name
  size_t DFA::get_state(std::string& name)
  {
    return this->state_map_[name];
  }

  // Make a new state from a set of NFA states
  std::pair<bool, size_t>
  DFA::make_state(std::set<internal::State*> const& states)
  {
    // Check fi state already exists
    auto state_name = build_name(states);
    if (auto it = this->state_map_.find(state_name);
        it != state_map_.end()) {
      return {false, it->second};
    }

    // Generate indices set
    auto state_view =
      states |
      std::views::transform([](auto const* state) { return state->pos; });
    std::set<size_t> const state_ids{state_view.begin(), state_view.end()};

    // Build a set and return its index
    auto state_idx = this->storage_.size();

    // Check if the state is to be marked final
    auto state_final =
      std::ranges::any_of(states, [](internal::State const* state) {
        return state->final;
      });

    // Add state to storage
    this->storage_.emplace_back(state_name, state_ids, state_final);

    // Register state for future lookup
    this->state_map_[state_name] = state_idx;
    return {true, state_idx};
  }

  // ======== Helper methods ======== //
  std::string build_name(std::set<internal::State*> const& states)
  {
    if (states.empty()) {
      return "0";
    }

    std::stringstream name;
    char sep = '{';

    for (auto* state : states) {
      name << sep << state->pos;
      sep = ',';
    }

    name << '}';

    return name.str();
  }

}  //namespace Kalika
