#ifndef NFA_H
#define NFA_H

#include <deque>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

char const eps = '\0';

struct State {
  using StateList = std::vector<State*>;

  State(std::string name, bool final) :
    final_(final), name_(std::move(name))
  {}

  /**
   * @brief Add a transition rule to the state machine
   */
  void add_transition(char ch, State* s);
  /**
   * @brief Check if the state machine accepts the string
   */
  bool test(std::string_view in_str) const;

  void toggle() { this->final_ = !(this->final_); }

private:
  bool final_;
  std::string name_;
  std::unordered_map<char, StateList> transitions_;
};

struct NFA {
  NFA(State* start, State* end) : start_(start), end_(end) {}

  /**
   * @brief Return true if the NFA accepts the string
   */
  [[nodiscard]] bool test(std::string_view in_str) const;

  // Accessor methods
  State* start() { return this->start_; }

  State* end() { return this->end_; }

private:
  State* start_;
  State* end_;
};

#endif
