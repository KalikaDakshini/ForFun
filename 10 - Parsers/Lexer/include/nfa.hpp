#ifndef NFA_H
#define NFA_H

#include <concepts>
#include <cstddef>
#include <deque>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

static constexpr char eps = '\0';

struct NFAState {
  bool final;
  std::string name{};
  std::unordered_map<char, std::set<NFAState*>> transitions_;

  explicit NFAState(bool state_final = false) : final(state_final) {}

  /**
   * @brief Toggle the final status of the state
   */
  void toggle() { this->final = !final; }

  /**
   * @brief Set the name object
   */
  void set_name(std::string state_name)
  {
    this->name = std::move(state_name);
  }

  /**
   * @brief Add a transition to the state
   */
  void add_transition(char ch, NFAState* state);

  /**
   * @brief Return all transitions from a state
   */
  std::set<NFAState*> get_transition(char ch) const;

  /**
   * @brief Test whether the machine accepts the input
   */
  bool test(
    std::string_view input,
    std::size_t idx,
    std::set<std::pair<NFAState const*, std::size_t>>& visited
  ) const;
};

struct NFA;
template<typename T>
concept IsNFA = std::same_as<std::remove_cvref_t<T>, NFA>;

/**
 * @brief NFA with a single start state and a single end state
 */
struct NFA {
  NFA(NFAState* start, NFAState* end) : start_(start), end_(end) {}

  /**
   * @brief Test whether the machine accepts the input
   */
  bool test(std::string_view input) const;

  // Accessor methods
  NFAState* start() const { return this->start_; }

  NFAState* end() const { return this->end_; }

  /**
   * @brief Add alphabet from nfa to current machine
   */
  void add_alphabet(const NFA& nfa);
  /**
   * @brief Update the current nfa's alphabet
   */
  void add_alphabet(char ch);
  //TODO(kalika): Implement build_table()
  /**
   * @brief Build transition table used for DFA generation
   */
  void build_table();

  /**
   * @brief Return the number of states used by NFA
   */
  std::size_t size() const { return this->storage_.size(); }

private:
  NFAState* start_;
  NFAState* end_;

  std::deque<NFAState> storage_;
  std::set<char> alphabet_;
};
#endif
